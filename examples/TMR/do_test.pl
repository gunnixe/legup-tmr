#!/usr/bin/perl

my @example_list = qw(add fir matrixmultiply qsort fft);

my ($fname) = @ARGV;
die "Need folder name\n" if(not defined $fname);

if($fname eq "all") {
	&do_work($_) for(@example_list);
} else {
	$dest_folder = "template/".$fname;
	die "Folder '$dest_folder' is not exist\n" if(!-d $dest_folder);
	&do_work($fname);
}

sub do_work {
	my $fname = $_[0];
	&clean_work($fname);

	system("mkdir -p output") if(!-d "outout");
	my $report_name = "output/".$fname.".rpt";
	open(FRH, '>', $report_name) or die "cannot open '$report_name' $!";
	
	for(my $i=0; $i<=5; $i++) {
	
		# prepare work dir
		my $cname = $fname."_".$i;
		system("mkdir -p output") if(!-d "output");
		system("cp -r template/$fname output/$cname");
		chdir "output/$cname";
	
		if($i==0) { #no TMR
			&change_config(0,0);
		} else { #TMR with Mode=$i-1
			&change_config(1,$i-1);
		}
	
		# do simulation
		system("make; make v | tee vsim.log;");
		system("make p; make f;");
	
		# finalize work dir
		&summary($fname, $i);
		chdir "../../";
	}
	
	close(FRH);
}

sub clean_work {
	for(my $i=0; $i<=5; $i++) {
		my $cname = $_[0]."_".$i;
		system("rm -rf output/$cname");
	}
}

sub summary {
	my ($fname, $i) = @_;
	my $tmr = ($i==0)? 0 : 1;
	my $voter_mode = ($i==0)? 0 : $i-1;
	print FRH "----- TMR=$tmr, VOTER_MODE=$voter_mode -----\n";

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
	($tmr, $voter_mode) = @_;
	open(FH, '+<', "config.tcl") or die "cannot open 'config.tcl' $!";
	my $out = '';
	while(<FH>) {
		chomp;
		if(m/^set_parameter TMR \d$/) {
			$out .= "set_parameter TMR $tmr\n";
		} elsif(m/^set_parameter VOTER_MODE \d$/) {
			$out .= "set_parameter VOTER_MODE $voter_mode\n";
		} else {
			$out .= "$_\n";
		}
	}
	seek(FH, 0, 0);
	print FH $out;
	truncate(FH, tell(FH));
	close(FH);
}
