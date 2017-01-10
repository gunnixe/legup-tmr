#!/usr/bin/perl -s

my @example_list;

#simplelist
#@example_list = (@example_list,qw(add fir mmult qsort fft));
@example_list = (@example_list,qw(add));
@example_list = (@example_list,qw(mmult));
#
##dhrystone benchmark example
@example_list = (@example_list,qw(aes aesdec gsm blowfish motion));
@example_list = (@example_list,qw(mips sha));
@example_list = (@example_list,qw(satd bellmanford));
@example_list = (@example_list,qw(sobel));
@example_list = (@example_list,qw(dfadd dfmul dfdiv));
@example_list = (@example_list,qw(dfsin));
@example_list = (@example_list,qw(adpcm));
@example_list = (@example_list,qw(jpeg));

my ($fname) = @ARGV;
die "Need folder name\n" if(not defined $fname);

my @name_list;
my @number_of_slice_registers;
my @number_of_slice_luts;
my @number_of_occupied_slices;
my @number_of_bounded_iobs;
my @number_of_ramb36e1s;
my @number_of_ramb18e1s;
my @number_of_dsp48e1s;
my @sim_max_freq_mhz;
my @sim_number_of_clock_cycles;
my @sim_wall_clock_us;
my @sensitive_01;
my @sensitive_02;
my @sensitive_03;
my @sensitive_04;
my @sensitive_05;
my @sensitive_06;
my @sensitive_07;
my @sensitive_08;
my @sensitive_09;
my @sensitive_0A;
my @sensitive_0B;
my @sensitive_0C;
my @sensitive_0D;
my @sensitive_0E;
my @sensitive_0F;
my @sensitive_10;
my @sensitive_11;
my @sensitive_12;
my @sensitive_13;
my @sensitive_14;
my @sensitive_15;
my @sensitive_16;
my @sensitive_17;
my @sensitive_18;
my @sensitive_19;
my @sensitive_1A;
my @sensitive_1B;

sub summarize_essential_bits {
	my ($cname) = @_;
	my $sname = "/home/legup/Dropbox/FCCM/results/SensitivityAnalysis/".$cname."_log.txt";
	open(FEH, '<', "$sname") or die "cannot open '$sname' $!";
	while(<FEH>) {
		chomp;
		if(/#01=(\d+)#/) { push @sensitive_01, $1; }
		elsif(/#02=(\d+)#/) { push @sensitive_02, $1; }
		elsif(/#03=(\d+)#/) { push @sensitive_03, $1; }
		elsif(/#04=(\d+)#/) { push @sensitive_04, $1; }
		elsif(/#05=(\d+)#/) { push @sensitive_05, $1; }
		elsif(/#06=(\d+)#/) { push @sensitive_06, $1; }
		elsif(/#07=(\d+)#/) { push @sensitive_07, $1; }
		elsif(/#08=(\d+)#/) { push @sensitive_08, $1; }
		elsif(/#09=(\d+)#/) { push @sensitive_09, $1; }
		elsif(/#0A=(\d+)#/) { push @sensitive_0A, $1; }
		elsif(/#0B=(\d+)#/) { push @sensitive_0B, $1; }
		elsif(/#0C=(\d+)#/) { push @sensitive_0C, $1; }
		elsif(/#0D=(\d+)#/) { push @sensitive_0D, $1; }
		elsif(/#0E=(\d+)#/) { push @sensitive_0E, $1; }
		elsif(/#0F=(\d+)#/) { push @sensitive_0F, $1; }
		elsif(/#10=(\d+)#/) { push @sensitive_10, $1; }
		elsif(/#11=(\d+)#/) { push @sensitive_11, $1; }
		elsif(/#12=(\d+)#/) { push @sensitive_12, $1; }
		elsif(/#13=(\d+)#/) { push @sensitive_13, $1; }
		elsif(/#14=(\d+)#/) { push @sensitive_14, $1; }
		elsif(/#15=(\d+)#/) { push @sensitive_15, $1; }
		elsif(/#16=(\d+)#/) { push @sensitive_16, $1; }
		elsif(/#17=(\d+)#/) { push @sensitive_17, $1; }
		elsif(/#18=(\d+)#/) { push @sensitive_18, $1; }
		elsif(/#19=(\d+)#/) { push @sensitive_19, $1; }
		elsif(/#1A=(\d+)#/) { push @sensitive_1A, $1; }
		elsif(/#1B=(\d+)#/) { push @sensitive_1B, $1; }
	}
	close(FEH);
}

sub summary_for_xilinx_syn {
	my ($cname) = @_;
	push @name_list, $cname;

	open(FXH, '<', "ML605.syr") or die "cannot open '$cname/ML605.syr' $!";
	while(<FXH>) {
		chomp;
		if(/\s+Number of Slice Registers:\s+(\d+)/) {
			$num = $1; $num =~ s/,//g;
			push @number_of_slice_registers, $num;
		} elsif(/\s+Number of Slice LUTs:\s+(\d+)/) {
			$num = $1; $num =~ s/,//g;
			push @number_of_slice_luts, $num;
		} elsif(/\s+Number of occupied Slices:\s+([\d,]+)/) {
			$num = $1; $num =~ s/,//g;
			push @number_of_occupied_slices, $num;
		} elsif(/\s+Number of bonded IOBs:\s+([\d,]+)/) {
			$num = $1; $num =~ s/,//g;
			push @number_of_bounded_iobs, $num;
		} elsif(/\s+Number of Block RAM\/FIFO:\s+([\d,]+)/) {
			$num = $1; $num =~ s/,//g;
			push @number_of_ramb36e1s, $num;
		} elsif(/\s+Number of DSP48E1s:\s+([\d,]+)/) {
			$num = $1; $num =~ s/,//g;
			push @number_of_dsp48e1s, $num;
		} elsif(/Maximum Frequency:\s+(\d+\.\d+)MHz/) {
			push @sim_max_freq_mhz, $1;
			$current_design_delay = $1;
			last;
		}
	}
	if(@number_of_dsp48e1s < @number_of_slice_luts) {
		push @number_of_dsp48e1s, "-";
	}
	if(@number_of_ramb36e1s < @number_of_slice_luts) {
		push @number_of_ramb36e1s, "-";
	}
	die "cannot find string in 'ML605.syr'" if(eof FXH);
	close(FXH);

	open(FSIMH, '<', "vsim.log") or die "cannot open 'vsim.log' $!";
	while(<FSIMH>) {
		chomp;
		if(/^# Cycle: +\d+ Time: +\d+ +RESULT: (\w+)$/) {
			die "simulation fail" if($1 eq "FAIL");
		} elsif(/^# Cycles:\s+(\d+)$/) {
			push @sim_number_of_clock_cycles, $1;
			my $current_design_wall_clock = $1/$current_design_delay;
			push @sim_wall_clock_us, $current_design_wall_clock;
			last;
		}
	}
	die "cannot find string in 'vsim.log'" if(eof FSIMH);
	close(FSIMH);
}

sub summary_for_xilinx_pnr {
	my ($cname) = @_;
	push @name_list, $cname;

	open(FXH, '<', "ML605.par") or die "cannot open '$cname/ML605.par' $!";
	while(<FXH>) {
		chomp;
		if(/\s+Number of Slice Registers:\s+([\d,]+) out of\s+[\d,]+\s+\d+%/) {
			$num = $1; $num =~ s/,//g;
			push @number_of_slice_registers, $num;
		} elsif(/\s+Number of Slice LUTs:\s+([\d,]+) out of\s+[\d,]+\s+\d+%/) {
			$num = $1; $num =~ s/,//g;
			push @number_of_slice_luts, $num;
		} elsif(/\s+Number of occupied Slices:\s+([\d,]+) out of\s+[\d,]+\s+\d+%/) {
			$num = $1; $num =~ s/,//g;
			push @number_of_occupied_slices, $num;
		} elsif(/\s+Number of bonded IOBs:\s+([\d,]+) out of\s+[\d,]+\s+\d+%/) {
			$num = $1; $num =~ s/,//g;
			push @number_of_bounded_iobs, $num;
		} elsif(/\s+Number of RAMB36E1\/FIFO36E1s:\s+([\d,]+) out of\s+[\d,]+\s+\d+%/) {
			$num = $1; $num =~ s/,//g;
			push @number_of_ramb36e1s, $num;
		} elsif(/\s+Number of RAMB18E1\/FIFO18E1s:\s+([\d,]+) out of\s+[\d,]+\s+\d+%/) {
			$num = $1; $num =~ s/,//g;
			push @number_of_ramb18e1s, $num;
		} elsif(/\s+Number of DSP48E1s:\s+([\d,]+) out of\s+[\d,]+\s+\d+%/) {
			$num = $1; $num =~ s/,//g;
			push @number_of_dsp48e1s, $num;
			last;
		}
	}
	die "cannot find string in 'ML605.par'" if(eof FXH);
	close(FXH);

	open(FTIMH, '<', "ML605.twr") or die "cannot open 'ML605.twr' $!";
	my $current_design_delay = 0;
	while(<FTIMH>) {
		chomp;
		if(/Maximum frequency:\s+(\d+\.\d+)MHz/) {
			push @sim_max_freq_mhz, $1;
			$current_design_delay = $1;
			last;
		}
	}
	die "cannot find string in 'ML605.twr'" if(eof FTIMH);
	close(FTIMH);

	open(FSIMH, '<', "vsim.log") or die "cannot open 'vsim.log' $!";
	while(<FSIMH>) {
		chomp;
		if(/^# Cycle: +\d+ Time: +\d+ +RESULT: (\w+)$/) {
			die "simulation fail" if($1 eq "FAIL");
		} elsif(/^# Cycles:\s+(\d+)$/) {
			push @sim_number_of_clock_cycles, $1;
			my $current_design_wall_clock = $1/$current_design_delay;
			push @sim_wall_clock_us, $current_design_wall_clock;
			last;
		}
	}
	die "cannot find string in 'vsim.log'" if(eof FSIMH);
	close(FSIMH);
}

sub summary_for_altera_syn {
	my ($cname) = @_;
	push @name_list, $cname;

	die "not implemented yet";
}

sub summary_for_altera_pnr {
	my ($cname) = @_;
	push @name_list, $cname;

	my $altera_ver = 13;
	open(FH, '<', "top.fit.summary") or die "cannot open '$cname/top.fit.summary' $!";
	while(<FH>) {
		chomp;
		if(m/^Quartus Prime Version : 16/) {
			$altera_ver = 16;
		}

		if($altera_ver == 13) { # for alter 13.0-sp1
			if(/^Total logic elements : ([\d,]+) \/ [\d,]+ /) {
				$num = $1; $num =~ s/,//g;
				push @number_of_slice_luts, $num;
			} elsif(/\s+Dedicated logic registers : ([\d,]+) \/ [\d,]+ /) {
				$num = $1; $num =~ s/,//g;
				push @number_of_slice_registers, $num;
			} elsif(/^Total memory bits : ([\d,]+) \/ [\d,]+ /) {
				$num = $1; $num =~ s/,//g;
				push @number_of_ramb18e1s, $num;
			} elsif(/^Embedded Multiplier 9-bit elements : ([\d,]+) \/ [\d,]+ /) {
				$num = $1; $num =~ s/,//g;
				push @number_of_dsp48e1s, $num;
				last;
			}
		} else { ## modifed for alter 16.1
			if(/^Logic utilization \(in ALMs\) : ([\d,]+) \/ [\d,]+ /) {
				$num = $1; $num =~ s/,//g;
				push @number_of_slice_luts, $num;
			} elsif(/^Total registers : ([\d,]+)/) {
				$num = $1; $num =~ s/,//g;
				push @number_of_slice_registers, $num;
			} elsif(/^Total RAM Blocks : ([\d,]+) \/ [\d,]+ /) {
				$num = $1; $num =~ s/,//g;
				push @number_of_ramb18e1s, $num;
			} elsif(/^Total DSP Blocks : ([\d,]+) \/ [\d,]+ /) {
				$num = $1; $num =~ s/,//g;
				push @number_of_dsp48e1s, $num;
				last;
			}
		}
	}
	die "cannot find string in 'top.fit.summary'" if(eof FH);
	close(FH);

	open(FH, '<', "top.sta.rpt") or die "cannot open 'top.sta.rpt' $!";
	my $current_design_delay = 0;
	my $found_string = 0;
	while(<FH>) {
		chomp;
		if(m/^; Fmax\s+;/) {
			$found_string = 1;
		} elsif($found_string==1) {
			if(/^;\s+(\d+\.\d+) MHz ;/) {
				push @sim_max_freq_mhz, $1;
				$current_design_delay = $1;
				last;
			}
		}
	}
	die "cannot find string in 'top.sta.rpt'" if(eof FH);
	close(FH);

	open(FSIMH, '<', "vsim.log") or die "cannot open 'vsim.log' $!";
	while(<FSIMH>) {
		chomp;
		if(/^# Cycle: +\d+ Time: +\d+ +RESULT: (\w+)$/) {
			die "simulation fail" if($1 eq "FAIL");
		} elsif(/^# Cycles:\s+(\d+)$/) {
			push @sim_number_of_clock_cycles, $1;
			my $current_design_wall_clock = $1/$current_design_delay;
			push @sim_wall_clock_us, $current_design_wall_clock;
			last;
		}
	}
	die "cannot find string in 'vsim.log'" if(eof FSIMH);
	close(FSIMH);
}

sub write_csv_file {
	my $xilinx = $_[0];
	my $csv_name = "output/report.csv";
	open(FCSV, '>', $csv_name) or die "cannot open '$csv_name' $!";

	print FCSV ",\n";
	print FCSV ",$_" for(@name_list);
	print FCSV ",\nNumber of Slice Registers";
	print FCSV ",$_" for(@number_of_slice_registers);
	print FCSV ",\nNumber of Slice LUTs";
	print FCSV ",$_" for(@number_of_slice_luts);
	if($xilinx) {
	print FCSV ",\nNumber of occupied Slices";
	print FCSV ",$_" for(@number_of_occupied_slices);
	print FCSV ",\nNumber of bounded IOBS";
	print FCSV ",$_" for(@number_of_bounded_iobs);
	print FCSV ",\nNumber of RAMB36E1/FIFO36E1s";
	print FCSV ",$_" for(@number_of_ramb36e1s);
	}
	print FCSV ",\nNumber of RAMB18E1/FIFO18E1s";
	print FCSV ",$_" for(@number_of_ramb18e1s);
	print FCSV ",\nNumber of DSP48E1s";
	print FCSV ",$_" for(@number_of_dsp48e1s);
	print FCSV ",\n";

	print FCSV ",\nMaximum Clock Frequency (MHz)";
	print FCSV ",$_" for(@sim_max_freq_mhz);
	print FCSV ",\nNumber of clock cycles";
	print FCSV ",$_" for(@sim_number_of_clock_cycles);
	print FCSV ",\nWall clock (ms)";
	print FCSV ",$_" for(@sim_wall_clock_us);
	print FCSV ",\n";

	print FCSV ",\nSensitive bits (total)";                     print FCSV ",$_" for(@sensitive_01);
	print FCSV ",\nSensitive interconnection bits";             print FCSV ",$_" for(@sensitive_02);
	print FCSV ",\nSensitive interface bits";                   print FCSV ",$_" for(@sensitive_03);
	print FCSV ",\nSensitive block configuration bits";         print FCSV ",$_" for(@sensitive_04);
	print FCSV ",\nIOBs Sensitive interconnection bits";        print FCSV ",$_" for(@sensitive_05);
	print FCSV ",\nIOBs Sensitive interface bits";              print FCSV ",$_" for(@sensitive_06);
	print FCSV ",\nIOBs Sensitive block configuration bits";    print FCSV ",$_" for(@sensitive_07);
	print FCSV ",\nCLBs Sensitive interconnection bits";        print FCSV ",$_" for(@sensitive_08);
	print FCSV ",\nCLBs Sensitive interface bits";              print FCSV ",$_" for(@sensitive_09);
	print FCSV ",\nCLBs Sensitive block configuration bits";    print FCSV ",$_" for(@sensitive_0A);
	print FCSV ",\nBRAMs Sensitive interconnection bits";       print FCSV ",$_" for(@sensitive_0B);
	print FCSV ",\nBRAMs Sensitive interface bits";             print FCSV ",$_" for(@sensitive_0C);
	print FCSV ",\nBRAMs Sensitive block configuration bits";   print FCSV ",$_" for(@sensitive_0D);
	print FCSV ",\nDSPs Sensitive interconnection bits";        print FCSV ",$_" for(@sensitive_0E);
	print FCSV ",\nDSPs Sensitive interface bits";              print FCSV ",$_" for(@sensitive_0F);
	print FCSV ",\nDSPs Sensitive block configuration bits";    print FCSV ",$_" for(@sensitive_10);
	print FCSV ",\nCLKs Sensitive interconnection bits";        print FCSV ",$_" for(@sensitive_11);
	print FCSV ",\nCLKs Sensitive interface bits";              print FCSV ",$_" for(@sensitive_12);
	print FCSV ",\nCLKs Sensitive block configuration bits";    print FCSV ",$_" for(@sensitive_13);
	print FCSV ",\n";                                           
	print FCSV ",\nSensitive interconnection frames";           print FCSV ",$_" for(@sensitive_14);
	print FCSV ",\nSensitive interface frames";                 print FCSV ",$_" for(@sensitive_15);
	print FCSV ",\nSensitive block configuration frames";       print FCSV ",$_" for(@sensitive_16);
	print FCSV ",\nSensitive frames";                           print FCSV ",$_" for(@sensitive_17);
	print FCSV ",\n";                                           
	print FCSV ",\nDevice interconnection frames";              print FCSV ",$_" for(@sensitive_18);
	print FCSV ",\nDevice interface frames";                    print FCSV ",$_" for(@sensitive_19);
	print FCSV ",\nDevice block configuration frames";          print FCSV ",$_" for(@sensitive_1A);
	print FCSV ",\nDevice frames";                              print FCSV ",$_" for(@sensitive_1B);

	close (FCSV);
}

if($fname eq "all") {
	&do_work($_) for(@example_list);
} else {
	$dest_folder = "template/".$fname;
	die "Folder '$dest_folder' is not exist\n" if(!-d $dest_folder);
	&do_work($fname);
}
&write_csv_file($x);

sub do_work {
	my $fname = $_[0];
	# options
	#-x : xilinx
	#-c : create only
	#-f : (finalize) summary only
	#-s : simulation only
	#-ns : no simulation
	#-p : full (pnr) synthesis only
	#-q : quick synthesis
	my $xilinx = $x;
	my $flag_create = 1;
	my $flag_synth = 1;
	my $flag_summary = 1;
	my $flag_sim = 1;
	if($c) {
		($flag_create, $flag_sim, $flag_synth, $flag_summary) = (1,0,0,0);
	} elsif($s) {
		($flag_create, $flag_sim, $flag_synth, $flag_summary) = (1,1,0,0);
	} elsif($f) {
		($flag_create, $flag_sim, $flag_synth, $flag_summary) = (0,0,0,1);
	} elsif($p) {
		($flag_create, $flag_sim, $flag_synth, $flag_summary) = (0,0,1,1);
	}

	# special options
	if($nosim) { $flag_sim = 0; }
	if($nosummary) { $flag_summary = 0; }

	system("mkdir -p output") if(!-d "output");
	#my $report_name = "output/".$fname.".rpt";
	#open(FRH, '>', $report_name) or die "cannot open '$report_name' $!";
	
	my $scenario_cnt = 0;
	open(FSH, '<', "scenario.txt") or die "cannot open 'scenario.txt' $!";
	while(<FSH>) {
		chomp;
		next if /^#/; #discard comments

		if(m/^\d \d \d \d \d \d$/) {
			@arg_list = split(/ /, $_, 6);
			#my ($tmr, $smode, $pmode, $lram, $pipe, $rvlram) = @arg_list; 

			# LegUp4.0 does not support complex data dependency analysis.
			# - need to skip pipelining
			if(($fname eq "qsort")
					|| ($fname eq "fft")
					|| ($fname eq "dfdiv")
					|| ($fname eq "gsm")
					|| ($fname eq "motion")
					|| ($fname eq "blowfish")
					|| ($fname eq "sha")
					|| ($fname eq "adpcm")
					|| ($fname eq "jpeg")
					|| ($fname eq "dfsin")
					|| ($fname eq "bellmanford")
			  ) {
				$arg_list[4] = 0;
			}

			# - need to skip localmemory use
			if(($fname eq "adpcm")
					|| ($fname eq "bellmanford")
			  ) {
				$arg_list[3] = 0;
			}

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
			system("make 2>&1 | tee make.log;") if $flag_create;
			system("make v | tee vsim.log;") if $flag_sim;
			system("make p; make f | tee synth.log;") if($flag_synth && $q==0);
			system("make p; make q | tee synth.log;") if($flag_synth && $q==1);
	
			# summarize
			#&make_report($cname, @arg_list) if ($flag_sim || $flag_summary);
			if($flag_summary) {
				&summary_for_altera_syn($cname) if ($x==0 && $q==1);
				&summary_for_altera_pnr($cname) if ($x==0 && $q==0);
				&summary_for_xilinx_syn($cname) if ($x==1 && $q==1);
				&summary_for_xilinx_pnr($cname) if ($x==1 && $q==0);
			}

			if($flag_summary && $e) { #essential bit analysis
				&summarize_essential_bits($cname) if $x==1;
			}

			# finalize work dir
			chdir "../../";
			$scenario_cnt = $scenario_cnt + 1;
		}
	}
	close(FSH);
	#close(FRH);
}

#sub make_report {
#	my ($cname, $tmr, $smode, $pmode, $lram, $pipe, $rvlram) = @_;
#	print FRH "\n#----- dir_name=$cname, TMR=$tmr, SYNC_VOTER_MODE=$smode, PART_VOTER_MODE=$pmode -----\n";
#	print FRH "#-----     LOCAL_RAM=$lram, USE_REG_VOTER_FOR_LOCAL_RAMS=$rvlram, pipeline=$pipe -----\n";
#
#	open(FH, '<', "vsim.log") or die "cannot open '$cname/vsim.log' $!";
#	while(<FH>) {
#		chomp;
#		if(m/^# Cycle: +\d+ Time: +\d+ +RESULT: \w+$/) {
#			print FRH "\t$_\n";
#			my $line2 = readline(FH);
#			print FRH "\t$line2";
#			my $line3 = readline(FH);
#			print FRH "\t$line3";
#			last;
#		}
#	}
#	die "cannot find string in '$cname/vsim.log'" if(eof FH);
#	close(FH);
#}

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
		next if(m/set_parameter PIPELINE_ALL \d$/);
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
	#$out .= "loop_pipeline \"loop\"\n" if($pipe);
	$out .= "set_parameter PIPELINE_ALL $pipe\n";

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
