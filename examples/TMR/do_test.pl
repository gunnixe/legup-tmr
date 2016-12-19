#!/usr/bin/perl -s

my @example_list;

#simplelist
@example_list = (@example_list,qw(add fir_opt matrixmultiply qsort fft));

#dhrystone benchmark example
@example_list = (@example_list,qw(aes gsm blowfish motion));
@example_list = (@example_list,qw(dfadd dfmul dfdiv));
@example_list = (@example_list,qw(mips sha));
# adpcm have an error for local memory alias analysis
# jpeg & dfsin too large
@example_list = (@example_list,qw(adpcm jpeg dfsin));

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
my @sensitive_bits_total;
my @sensitive_interconnection_bits;
my @sensitive_interface_bits;
my @sensitive_block_configuration_bits;
my @sensitive_interconnection_frames;
my @sensitive_interface_frames;
my @sensitive_block_configuration_frames;
my @sensitive_frames;
my @device_interconnection_frames;
my @device_interface_frames;
my @device_block_configuration_frames;
my @device_frames;
my @sim_max_freq_mhz;
my @sim_number_of_clock_cycles;
my @sim_wall_clock_us;
my @fault_injection_recoverable;
my @fault_injection_unrecoverable;

sub summarize_in_csv {
	my ($cname) = @_;
	push @name_list, $cname;

	open(FXH, '<', "ML605.par") or die "cannot open 'ML605.par' $!";
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
		if(/Maximum frequency: (\d+\.\d+)MHz/) {
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

sub summary_for_altera {
	my ($cname) = @_;
	push @name_list, $cname;

	my $altera_ver = 13;
	open(FH, '<', "top.fit.summary") or die "cannot open 'top.fit.summary' $!";
	while(<FH>) {
		chomp;
		if(m/^Quartus II 64-Bit Version : 16/) {
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
	while(<FH>) {
		chomp;
		if(m/^; Fmax\s+;/) {
			my $line;
			$line = readline(FH) for(1..2);
			if(/^;\s+(\d+\.\d+) MHz ;/) {
				push @sim_max_freq_mhz, $num;
				$current_design_delay = $num;
			}
			last;
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

	close (FCSV);
}

if($fname eq "all") {
	&do_work($_, $c, $f, $x) for(@example_list);
} else {
	$dest_folder = "template/".$fname;
	die "Folder '$dest_folder' is not exist\n" if(!-d $dest_folder);
	&do_work($fname, $c, $f, $x);
}
&write_csv_file();

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
			  ) {
				$arg_list[4] = 0;
			}

			# - need to skip localmemory use
			if(($fname eq "adpcm")
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
			system("make | tee make.log;") if $flag_exec;
			system("make v | tee vsim.log;") if $flag_exec;
			system("make p; make f | tee synth.log;") if($flag_exec);
	
			# summarize
			if($flag_summary) {
				&make_report($cname, @arg_list);
				&summary_for_altera($cname) if $xilinx==0;
				&summarize_in_csv($cname) if $xilinx==1;
			}

			# finalize work dir
			chdir "../../";
			$scenario_cnt = $scenario_cnt + 1;
		}
	}
	close(FSH);
	close(FRH);
}

sub make_report {
	my ($cname, $tmr, $smode, $pmode, $lram, $pipe, $rvlram) = @_;
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
