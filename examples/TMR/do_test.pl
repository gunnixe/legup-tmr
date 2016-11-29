#!/usr/bin/perl -s

#my @example_list = qw(add fir matrixmultiply qsort fft);

#dhrystone benchmark example
my @example_list = qw(adpcm aes gsm blowfish);# jpeg);

my ($fname) = @ARGV;
die "Need folder name\n" if(not defined $fname);

if($fname eq "all") {
	&do_work($_, $c) for(@example_list);
} else {
	$dest_folder = "template/".$fname;
	die "Folder '$dest_folder' is not exist\n" if(!-d $dest_folder);
	&do_work($fname, $c);
}

sub do_work {
	my $fname = $_[0];
	my $create_only = $_[1];

	system("mkdir -p output") if(!-d "outout");
	my $report_name = "output/".$fname.".rpt";
	open(FRH, '>', $report_name) or die "cannot open '$report_name' $!";
	
	my $scenario_cnt = 0;
	open(FSH, '<', "scenario.txt") or die "cannot open 'scenario.txt' $!";
	while(<FSH>) {
		chomp;
		next if /^#/; #discard comments

		if(m/^\d \d \d \d \d$/) {
			@arg_list = split(/ /, $_, 5);
			my ($tmr, $smode, $pmode, $lram, $pipe) = @arg_list; 

			# LegUp4.0 does not support complex data dependency analysis.
			# Thus, LegUp4.0 fails for pieplining.
			# - need to skip several example including qsort and fft.
			if(($fname eq "qsort") || ($fname eq "fft")) {
				if(($lram==1) || ($pipe==1)) {
					next;
				}
			}

			# prepare work dir
			my $cname = $fname."_".$scenario_cnt;
			system("rm -rf output/$cname");

			system("mkdir -p output") if(!-d "output");
			system("cp -r template/$fname output/$cname");
			chdir "output/$cname";

			&change_config(@arg_list);
		
			if(!$create_only) {
				# do simulation
				system("make; make v | tee vsim.log;");
				system("make p; make f;");
	
				&summary(($cname, @arg_list));
			}

			# finalize work dir
			chdir "../../";
			$scenario_cnt = $scenario_cnt + 1;
		}
	}
	close(FSH);
	close(FRH);
}

sub summary {
	my ($cname, $tmr, $svoter_mode, $pvoter_mode, $local_ram, $pipeline) = @_;
	print FRH "#----- foler_name=$cname, TMR=$tmr, SYNC_VOTER_MODE=$svoter_mode, PART_VOTER_MODE=$pvoter_mode -----\n";
	print FRH "#-----     LOCAL_RAM=$local_ram, pipeline=$pipeline -----\n";

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
		if((m/^Total logic elements : [\d,]+ \/ [\d,]+ /) ||
		   (m/\s+Total combinational functions : [\d,]+ \/ [\d,]+ /) ||
		   (m/\s+Dedicated logic registers : [\d,]+ \/ [\d,]+ /) ||
		   (m/^Total memory bits : [\d,]+ \/ [\d,]+ /) ||
		   (m/^Embedded Multiplier 9-bit elements : [\d,]+ \/ [\d,]+ /)) {
			print FRH "\t$_\n";
		}

		last if(m/^Embedded Multiplier 9-bit elements : [\d,]+ \/ [\d,]+ /);
	}
	die "cannot find string in 'top.fit.summary'" if(eof FH);
	close(FH);

	open(FH, '<', "top.sta.rpt") or die "cannot open 'top.sta.rpt' $!";
	while(<FH>) {
		chomp;
		if(m/^; Slow Model Fmax Summary/) {
			my $line;
			$line = readline(FH) for(1..4);
			print FRH "\tFMax = $line\n";
			last;
		}
	}
	die "cannot find string in 'top.sta.rpt'" if(eof FH);
	close(FH);
}

sub change_config {
	my ($tmr, $smode, $pmode, $lram, $pipe) = @_;
	open(FH, '+<', "config.tcl") or die "cannot open 'config.tcl' $!";
	my $out = '';
	while(<FH>) {
		chomp;
		if(m/^set_parameter TMR \d$/) {
			$out .= "set_parameter TMR $tmr\n";
		} elsif(m/^set_parameter SYNC_VOTER_MODE \d$/) {
			$out .= "set_parameter SYNC_VOTER_MODE $smode\n";
		} elsif(m/^set_parameter PART_VOTER_MODE \d$/) {
			$out .= "set_parameter PART_VOTER_MODE $pmode\n";
		} elsif(m/set_parameter LOCAL_RAMS \d$/) {
			$out .= "set_parameter LOCAL_RAMS $lram\n";
		} elsif(m/loop_pipeline \"loop\"$/) {
			if($pipe==0) {
				$out .= "#loop_pipeline \"loop\"\n";
			} else {
				$out .= "loop_pipeline \"loop\"\n";
			}
		} else {
			$out .= "$_\n";
		}
	}
	seek(FH, 0, 0);
	print FH $out;
	truncate(FH, tell(FH));
	close(FH);
}
