#!/usr/bin/perl -s

my @example_list;

#simplelist
@example_list = (@example_list,qw(add fir_opt matrixmultiply qsort fft));

#dhrystone benchmark example
#@example_list = (@example_list,qw(aes gsm blowfish motion));
#@example_list = (@example_list,qw(dfadd dfmul dfdiv));
#@example_list = (@example_list,qw(mips sha));
# adpcm have an error for local memory alias analysis
# jpeg & dfsin too large
#@example_list = (example_list,qw(adpcm jpeg dfsin));

my ($fname) = @ARGV;
die "Need folder name\n" if(not defined $fname);

if($fname eq "all") {
	&do_work($_, $c, $f, $x) for(@example_list);
} else {
	$dest_folder = "template/".$fname;
	die "Folder '$dest_folder' is not exist\n" if(!-d $dest_folder);
	&do_work($fname, $c, $f, $x);
}

sub do_work {
	my $fname = $_[0];
	my $xilinx = $_[3];
	my $flag_create = 1;
	my $flag_exec = 1;
	my $flag_summary = 1;
	if($_[1]) {
		($flag_create, $flag_exec, $flag_summary) = (1,0,0);
	} elsif($_[2]) {
		($flag_create, $flag_exec, $flag_summary) = (0,0,1);
	}
		

	system("mkdir -p output") if(!-d "outout");
	my $report_name = "output/".$fname.".rpt";
	open(FRH, '>', $report_name) or die "cannot open '$report_name' $!";
	
	my $scenario_cnt = 0;
	open(FSH, '<', "scenario.txt") or die "cannot open 'scenario.txt' $!";
	while(<FSH>) {
		chomp;
		next if /^#/; #discard comments

		if(m/^\d \d \d \d \d \d$/) {
			@arg_list = split(/ /, $_, 6);
			#my ($tmr, $smode, $pmode, $lram, $pipe, $rvlram) = @arg_list; 

			# LegUp4.0 does not support complex data dependency analysis.
			# Thus, LegUp4.0 fails for pieplining.
			# - need to skip several example including qsort and fft.
			#if(($fname eq "qsort") || ($fname eq "fft")) {
			#	#if(($lram==1) || ($pipe==1)) {
			#	if($pipe==1) {
			#		print ("$fname does not support pipeline\n");
			#		next;
			#	}
			#}

			# prepare work dir
			my $cname = $fname."_".$scenario_cnt;

			if($flag_create) {
				system("rm -rf output/$cname");
				system("mkdir -p output") if(!-d "output");
				system("cp -r template/$fname output/$cname");
			}
			chdir "output/$cname";
			&change_config(@arg_list, $xilinx) if($flag_create);
			&change_makefile($xilinx) if($flag_create);
		
			# do simulation
			#FIXME
			system("make | tee make.log;") if $flag_exec;
			system("make v | tee vsim.log;") if $flag_exec;
			system("make p; make f;") if($flag_exec);
	
			&summary(($cname, @arg_list)) if $flag_summary;

			# finalize work dir
			chdir "../../";
			$scenario_cnt = $scenario_cnt + 1;
		}
	}
	close(FSH);
	close(FRH);
}

sub summary {
	my ($cname, $tmr, $smode, $pmode, $lram, $rvlram, $pipe) = @_;
	print FRH "\n#----- dir_name=$cname, TMR=$tmr, SYNC_VOTER_MODE=$smode, PART_VOTER_MODE=$pmode -----\n";
	print FRH "#-----     LOCAL_RAM=$lram, USE_REG_VOTER_FOR_LOCAL_RAMS=$rvlram, pipeline=$pipe -----\n";

	open(FH, '<', "vsim.log") or die "cannot open 'vsim.log' $!";
	while(<FH>) {
		chomp;
		if(m/^# Cycle: +\d+ Time: +\d+ +RESULT: \w+$/) {
			print FRH "\t$_\n";
			my $line2 = readline(FH);
			print FRH "\t$line2";
			my $line3 = readline(FH);
			print FRH "\t$line3";
			last;
		}
	}
	die "cannot find string in 'vsim.log'" if(eof FH);
	close(FH);

	open(FH, '<', "top.fit.summary") or die "cannot open 'top.fit.summary' $!";
	while(<FH>) {
		chomp;
		# for alter 13.0-sp1
		if((m/^Total logic elements : [\d,]+ \/ [\d,]+ /) ||
		   (m/\s+Total combinational functions : [\d,]+ \/ [\d,]+ /) ||
		   (m/\s+Dedicated logic registers : [\d,]+ \/ [\d,]+ /) ||
		   (m/^Total memory bits : [\d,]+ \/ [\d,]+ /) ||
		   (m/^Embedded Multiplier 9-bit elements : [\d,]+ \/ [\d,]+ /)) {
			print FRH "\t$_\n";
		}
		last if(m/^Embedded Multiplier 9-bit elements : [\d,]+ \/ [\d,]+ /);

		## modifed for alter 16.1
		#if((m/^Logic utilization \(in ALMs\) : [\d,]+ \/ [\d,]+ /) ||
		#   (m/^Total registers : [\d,]+/) ||
		#   (m/^Total block memory bits : [\d,]+ \/ [\d,]+ /) ||
		#   (m/^Total RAM Blocks : [\d,]+ \/ [\d,]+ /) ||
		#   (m/^Total DSP Blocks : [\d,]+ \/ [\d,]+ /)) {
		#	print FRH "\t$_\n";
		#}
		#last if(m/^Total DSP Blocks : [\d,]+ \/ [\d,]+ /);
	}
	die "cannot find string in 'top.fit.summary'" if(eof FH);
	close(FH);

	open(FH, '<', "top.sta.rpt") or die "cannot open 'top.sta.rpt' $!";
	while(<FH>) {
		chomp;
		if(m/^; Fmax\s+;/) {
			my $line;
			$line = readline(FH) for(1..2);
			print FRH "\tFMax = $line\n";
			last;
		}
	}
	die "cannot find string in 'top.sta.rpt'" if(eof FH);
	close(FH);
}

sub change_makefile {
	my ($xilinx) = @_;
	return if($xilinx==0);

	open(FH, '+<', "Makefile") or die "cannot open 'Makefile' $!";
	my $out = '';
	while(<FH>) {
		chomp;
		if(m/^XILINX/) {
			next;
		} elsif(m/^include/ && m/Makefile.common/) {
			$out .= "XILINX = 1\n";
			$out .= "$_\n";
		} else {
			$out .= "$_\n";
		}
	}
	seek(FH, 0, 0);
	print FH $out;
	truncate(FH, tell(FH));
	close(FH);
}

sub change_config {
	my ($tmr, $smode, $pmode, $lram, $pipe, $rvlram, $xilinx) = @_;
	open(FH, '+<', "config.tcl") or die "cannot open 'config.tcl' $!";
	my $out = '';
	while(<FH>) {
		chomp;
		next if(m/set_parameter TMR \d$/);
		next if(m/set_parameter SYNC_VOTER_MODE \d$/);
		next if(m/set_parameter PART_VOTER_MODE \d$/);
		next if(m/set_parameter LOCAL_RAMS \d$/);
		next if(m/set_parameter USE_REG_VOTER_FOR_LOCAL_RAMS \d$/);
		next if(m/loop_pipeline \"loop\"$/);

		$out .= "$_\n";
	}
	$out .= "\n";
	$out .= "set_parameter TMR $tmr\n";
	$out .= "set_parameter SYNC_VOTER_MODE $smode\n";
	$out .= "set_parameter PART_VOTER_MODE $pmode\n";
	$out .= "set_parameter LOCAL_RAMS $lram\n";
	$out .= "set_parameter USE_REG_VOTER_FOR_LOCAL_RAMS $rvlram\n";
	#add locam mem constrant for FMax
	#if($lram && (($tmr==0) || ($tmr==1 && $smode==2))) {
	#	$out .= "set_operation_latency local_mem_dual_port 2\n";
	#}
	$out .= "loop_pipeline \"loop\"\n" if($pipe);

	if($xilinx) {
		$out .= "\nset_project Virtex6 ML605 hw_only\n";
		$out .= "set_parameter INFERRED_RAM_FORMAT \"xilinx\"\n";
		$out .= "set_parameter DIVIDER_MODULE generic\n";
	}

	seek(FH, 0, 0);
	print FH $out;
	truncate(FH, tell(FH));
	close(FH);
}
