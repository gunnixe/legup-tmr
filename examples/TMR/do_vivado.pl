#!/usr/bin/perl -s

use POSIX;

my $DEST = output_simplex_lat3;
my $START_CNT = 0;
my $SYN_TOP = top;
my $PNR_TOP = top;
my $TEMPLATE = template;
my $CLEAN_START = 1;

my @example_list;

#simplelist
#@example_list = (@example_list,qw(add fir mmult qsort fft));
#@example_list = qw(adpcm aes aesdec gsm sha blowfish dfadd dfdiv dfmul dfsin jpeg mips motion satd sobel bellmanford mmult);
@example_list = qw(adpcm aes aesdec gsm sha blowfish dfadd dfdiv dfmul dfsin mips motion satd sobel bellmanford mmult);
#@example_list = qw(adpcm aes aesdec gsm sha blowfish dfadd dfdiv dfmul mips motion satd bellmanford mmult);
#@example_list = qw(aes aesdec gsm sha dfadd dfmul mips motion satd bellmanford mmult);
#@example_list = qw(aes gsm mmult);

my ($fname) = @ARGV;
die "Need folder name\n" if(not defined $fname);

my $max_number_of_partitions = 1;
my @name_list;
my @verilog_number_of_sync_voters;
my @verilog_number_of_part_voters;
my @part_info;
my @number_of_slice_registers;
my @number_of_slice_luts;
my @number_of_occupied_slices;
my @number_of_bounded_iobs;
my @number_of_rams;
my @number_of_dsp48e1s;
my @sim_max_freq_mhz;
my @sim_number_of_clock_cycles;
my @sim_expected_result;
my @sim_wall_clock_us;
my @hier_lut;
my @hier_reg;
my @hier_dsp;
my @hier_bram;

open(FPH, '>', "part_report.rpt") or die "cannot open 'part_report.rpt' $!";

if($fname eq "all") {
	&do_work($_) for(@example_list);
} else {
	$dest_folder = "$TEMPLATE/".$fname;
	die "Folder '$dest_folder' is not exist\n" if(!-d $dest_folder);
	&do_work($fname);
}
&write_csv_file_vertical();

sub do_work {
	my $fname = $_[0];
	# options
	#-c : create only
	#-f : (finalize) summary only
	#-s : simulation only
	#-p : full (pnr) synthesis only

	#-nosim : no simulation
	#-nosummary
	#-realclock

	my $xilinx = 1;
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

	system("mkdir -p $DEST") if(!-d "$DEST");
	#my $report_name = "$DEST/".$fname.".rpt";
	#open(FRH, '>', $report_name) or die "cannot open '$report_name' $!";
	
	my $scenario_cnt = $START_CNT;
	open(FSH, '<', "scenario.txt") or die "cannot open 'scenario.txt' $!";
	while(<FSH>) {
		chomp;
		next if /^#/; #discard comments

		#if($scenario_cnt==5) { $scenario_cnt++; next; }

		if(m/^\d \d \d \d \d \d \d \d$/) {
			@arg_list = split(/ /, $_, 8);
			#my ($tmr, $smode, $pmode, $lram, $pipe, $numpart) = @arg_list; 

			# LegUp4.0 does not support complex data dependency analysis.
			# - need to skip pipelining
			if(($fname eq "qsort")
					|| ($fname eq "fft")
					|| ($fname eq "dfdiv")
					|| ($fname eq "gsm")
					|| ($fname eq "motion")
					|| ($fname eq "blowfish")
					|| ($fname eq "sha")
					#|| ($fname eq "adpcm")
					|| ($fname eq "jpeg")
					|| ($fname eq "dfsin")
					|| ($fname eq "bellmanford")
			  ) {
				$arg_list[4] = 0;
			}

			# - need to skip localmemory use
			#if(($fname eq "adpcm")
			#		#|| ($fname eq "bellmanford")
			#  ) {
			#	$arg_list[3] = 0;
			#}

			# minimum partition size = 2
			#if($arg_list[2] != 0 && $arg_list[7]<2) {
			#	$arg_list[7] = 2;
			#}

			# prepare work dir
			my $cname = $fname."_".$scenario_cnt;
			my $skip = (-e "$DEST/$cname" && !$force)? 1 : 0;
			my $fc = ($flag_create && !$skip)? 1 : 0;
			my $fs = ($flag_sim && !$skip)? 1 : 0;
			my $fi = ($flag_synth && !$skip)? 1 : 0;

			if($fc) {
				system("rm -rf $DEST/$cname") if($CLEAN_START);
				system("mkdir -p $DEST") if(!-d "$DEST");
				system("cp -r $TEMPLATE/$fname $DEST/$cname");
				system("cp ../../ip/libs/generic/div_unsigned.v $DEST/$cname");
				system("cp ../../ip/libs/generic/div_signed.v $DEST/$cname");
			}
			chdir "$DEST/$cname";
			&change_config(@arg_list, $xilinx) if($fc);
			my $pname = &change_makefile($xilinx);# if($fc);
		
			# do simulation
			system("make 2>&1 | tee make.log;") if $fc;
			system("make v | tee vsim.log;") if $fs;
			system("make p") if $fi;
			system("make f | tee synth.log;") if($fi);
	
			# summarize
			&parse_vsim_log($cname, $fname);
			&parse_generate_log($cname, $fname);
			&summary_for_xilinx_pnr($cname, $fname, $pname) if($flag_summary);
			&summary_info_txt($fname, $cname, $pname);

			# finalize work dir
			chdir "../../";
			$scenario_cnt = $scenario_cnt + 1;
		}
	}
	close(FSH);
}

sub summary_info_txt {
	my ($fname, $cname, $pname) = @_;

	#&parse_verilog($cname);

	open(FWH, '>', "info.txt") or die "cannot open '$cname/info.txt' $!";
	my $now = `date`;
	my $intFmax = int($sim_max_freq_mhz[-1]);
	print FWH "projectName = $cname\n";
	print FWH "maxFreq = $intFmax\n";
	print FWH "cycles = $sim_number_of_clock_cycles[-1]\n";
	print FWH "expectedResult = $sim_expected_result[-1]\n";
	print FWH "date = $now\n";
	close(FWH);
}

sub change_makefile {
	open(FH, '+<', "Makefile") or die "cannot open 'Makefile' $!";
	my $out = '';
	while(<FH>) {
		chomp;
		if(/^XILINX/ || /^VIVADO/ || /^ALWAYS_INLINE/) {
			next;
		} elsif(m/^include/ && m/Makefile.common/) {
			$out .= "XILINX = 1\n";
			$out .= "VIVADO = 1\n";
			$out .= "ALWAYS_INLINE=1\n";
			$out .= "$_\n";
		} else {
			if(m/^NAME\s*=\s*(\w+)/) {
				$name = $1;
			}
			$out .= "$_\n";
		}
	}
	seek(FH, 0, 0);
	print FH $out;
	truncate(FH, tell(FH));
	close(FH);

	die "cannot find string 'NAME' in Makefile" if(!defined $name);
	return $name;
}

sub change_config {
	my ($tmr, $smode, $pmode, $lram, $pipe,
		$numpart, $emode, $nvarea, $xilinx) = @_;
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
		next if(m/set_parameter NUMBER_OF_PARTITIONS \d$/);
		next if(m/set_parameter SEQUENTIAL_PART_VOTER \d$/);
		next if(m/set_parameter EBIT_FOR_PART_VOTER \d$/);
		next if(m/set_parameter EBIT_FOR_SYNC_VOTER \d$/);
		next if(m/set_parameter EBIT_MODE \d$/);
		next if(m/set_parameter MERGE_PVOTER_WITH_SVOTER \d$/);
		next if(m/set_parameter SEPERATE_BB_CTRL \d$/);
		next if(m/set_parameter EXTRACT_PART_VOTERS \d$/);
		next if(m/set_parameter NO_VOTER_AREA_ESTIMATE \d$/);
		next if(m/loop_pipeline \"loop\"$/);

		$out .= "$_\n";
	}

	$out .= "\n";
	$out .= "set_parameter TMR $tmr\n";
	#$out .= "set_parameter CLOCK_PERIOD 10\n" if($tmr && $clkmargin);
	$out .= "set_parameter SYNC_VOTER_MODE $smode\n";
	$out .= "set_parameter PART_VOTER_MODE $pmode\n";
	$out .= "set_parameter LOCAL_RAMS $lram\n";
	#$out .= "set_parameter USE_REG_VOTER_FOR_LOCAL_RAMS $rvlram\n";
	$out .= "set_parameter NUMBER_OF_PARTITIONS $numpart\n";
	#$out .= "set_parameter SEQUENTIAL_PART_VOTER $seqvote\n";
	#$out .= "set_parameter EBIT_FOR_PART_VOTER $epvoter\n";
	#$out .= "set_parameter EBIT_FOR_SYNC_VOTER $esvoter\n";
	$out .= "set_parameter EBIT_MODE $emode\n";
	#$out .= "set_parameter MERGE_PVOTER_WITH_SVOTER $mergev\n";
	$out .= "set_parameter NO_VOTER_AREA_ESTIMATE $nvarea\n";
	#add locam mem constrant for FMax
	#if($lram && (($tmr==0) || ($tmr==1 && $smode==2))) {
	#	$out .= "set_operation_latency local_mem_dual_port 2\n";
	#}
	#$out .= "loop_pipeline \"loop\"\n" if($pipe);
	$out .= "set_parameter PIPELINE_ALL $pipe\n";

	#Vivado flow
	$out .= "\nset_project Artix7 NexysVideo hw_only\n";
	$out .= "set_parameter INFERRED_RAM_FORMAT \"xilinx\"\n";
	$out .= "set_parameter DIVIDER_MODULE generic\n";

	seek(FH, 0, 0);
	print FH $out;
	truncate(FH, tell(FH));
	close(FH);
}

sub summary_for_xilinx_pnr {
	my $cname = $_[0];
	push @name_list, $cname;

	&parse_hierarchy_log(@_);
	&parse_timing_log(@_);
}

sub parse_hierarchy_log {
	my ($cname, $fname, $pname) = @_;

	my $hier_rpt = "./hierarchical_utilization.rpt";
	#open(FHL, '<', $hier_rpt) or die "cannot open '$hier_rpt' $!";
	#print FPH "#$cname#\n";
	#while(<FHL>) {
	#	chomp;
	#	if(/^\|\s+Instance\s+\|\s+Module\s+\|/) {
	#		print FPH "$_\n";
	#	} elsif(/^\|\s+BB_main_1\s+\|/) {
	#		print FPH "$_\n";
	#	} elsif(/^\|\s+BB_main_2\s+\|/) {
	#		print FPH "$_\n";
	#	} elsif(/^\|\s+BB_main_CTRL\s+\|/) {
	#		print FPH "$_\n\n";
	#		last;
	#	} elsif(/^\|\s+BB_main_0\s+\|/) {
	#		print FPH "$_\n\n";
	#		last;
	#	}
	#}
	#die "cannot find string in '$cname/$hier_rpt'" if(eof FHL);
	#close(FHL);

	#if(!-e $hier_rpt) {
	#	for (my $i=0; $i<$max_number_of_partitions; $i++) {
	#		push @hier_lut, "";
	#		push @hier_reg, "";
	#		push @hier_dsp, "";
	#		push @hier_bram, "";
	#	}
	#	push @number_of_slice_luts, "";
	#	push @number_of_slice_registers, "";
	#	push @number_of_rams, "";
	#	push @number_of_dsp48e1s, "";
	#	return;
	#}

	open(FHL, '<', $hier_rpt) or die "cannot open '$hier_rpt' $!";
	print FPH "#$cname#\n";
	my $num_part_find = 0;
	while(<FHL>) {
		chomp;
		if(/^\|\s+top\_inst\s+\|\s+top \|\s+(\d+) \|\s+(\d+) \|\s+(\d+) \|\s+(\d+) \|\s+(\d+) \|\s+(\d+) \|\s+(\d+) \|\s+(\d+) \|/) {
			push @number_of_slice_luts, $1;
			push @number_of_slice_registers, $5;
			#push @number_of_occupied_slices, "";
			my $ram = $6*2 + $7;
			push @number_of_rams, $ram;
			push @number_of_dsp48e1s, $8;
		} elsif(/^\|\s+BB\_main\_\d+\s+\|\s+BB\_main\_[\w\d\_]+ \|\s+(\d+) \|\s+(\d+) \|\s+(\d+) \|\s+(\d+) \|\s+(\d+) \|\s+(\d+) \|\s+(\d+) \|\s+(\d+) \|/) {
			push @hier_lut, $1;
			push @hier_reg, $5;
			my $ram = $6*2 + $7;
			push @hier_bram, $ram;
			push @hier_dsp, $8;
			$num_part_find++;
		} elsif(/^\|\s+BB\_main\_CTRL\s+\|\s+BB\_main\_CTRL* \|\s+(\d+) \|\s+(\d+) \|\s+(\d+) \|\s+(\d+) \|\s+(\d+) \|\s+(\d+) \|\s+(\d+) \|\s+(\d+) \|/) {
			push @hier_lut, $1;
			push @hier_reg, $5;
			my $ram = $6*2 + $7;
			push @hier_bram, $ram;
			push @hier_dsp, $8;
			$num_part_find++;
		} elsif(/^\|\s+main\_inst\_r1\s+\|\s+main\_*/) {
			last;
		} elsif($num_part_find == $max_number_of_partitions) {
			last;
		}
	}

	for (my $i=$num_part_find; $i<$max_number_of_partitions; $i++) {
		push @hier_lut, "";
		push @hier_reg, "";
		push @hier_bram, "";
		push @hier_dsp, "";
	}

	die "cannot find string in '$hier_rpt'" if(eof FHL);
	close(FHL);
}

sub parse_utilization_log {
	my ($cname, $fname, $pname) = @_;
	#push @name_list, $cname;

	my $util_rpt = "./$pname.runs/impl_1/ARTIX7_utilization_placed.rpt";
	open(FUH, '<', "$util_rpt") or die "cannot open '$util_rpt' $!";
	my $rpt_idx = 0;
	while(<FUH>) {
		chomp;
		if ($rpt_idx == 0) {
			$rpt_idx = 1 if(/^10. Instantiated Netlists$/); #to skip table of contents
		} elsif ($rpt_idx == 1) {
			$rpt_idx = 2 if(/^2. Slice Logic Distribution$/);
		} elsif ($rpt_idx == 2) {
			$rpt_idx = 3 if(/^3. Memory$/);
		} elsif ($rpt_idx == 3) {
			$rpt_idx = 4 if(/^4. DSP$/);
		}

		if ($rpt_idx == 1) {
			if(/^\| Slice LUTs\s+\|\s+(\d+) \|/) {
				push @number_of_slice_luts, $1;
			} elsif(/^\| Slice Registers\s+\|\s+(\d+) \|/) {
				push @number_of_slice_registers, $1;
			}
		} elsif ($rpt_idx==2) {
			if(/^\| Slice\s+\|\s+(\d+) \|/) {
				push @number_of_occupied_slices, $1;
			}
		} elsif ($rpt_idx==3) {
			if(/^\| Block RAM Tile\s+\|\s+(\d+) \|/) {
				push @number_of_occupied_rams, $1;
			}
		} elsif ($rpt_idx==4) {
			if(/^\| DSPs\s+\|\s+(\d+) \|/) {
				push @number_of_dsp48e1s, $1;
				last;
			}
		}
	}
	die "cannot find string in '$util_rpt'" if(eof FUH);
	close(FUH);
}

sub parse_timing_log {
	my ($cname, $fname, $pname) = @_;

	my $time_rpt = "./$pname.runs/impl_1/ARTIX7_timing_summary_routed.rpt";
	open(FTL, '<', $time_rpt) or die "cannot open '$time_rpt' $!";
	while(<FTL>) {
		chomp;
		#if(/Slack \(MET\) :\s+(\d+\.\d+)ns/) {
		#	$slack = $1;
		#} elsif (/\s+Requirement:\s+(\d+\.\d+)ns/) {
		#	$current_design_delay = $1 - $slack;
		#	push @sim_max_freq_mhz, 1000/$current_design_delay;
		#	my $current_design_wall_clock = $sim_number_of_clock_cycles[-1]*$1/1000;
		#	push @sim_wall_clock_us, $current_design_wall_clock;
		#	last;
		#}
		if(/\s+Data Path Delay:\s+(\d+\.\d+)ns/) {
			$current_design_delay = $1;
			push @sim_max_freq_mhz, 1000/$current_design_delay;
			my $current_design_wall_clock = $sim_number_of_clock_cycles[-1]*$1/1000;
			push @sim_wall_clock_us, $current_design_wall_clock;
			last;
		}
	}
	die "cannot find string in '$time_rpt'" if(eof FTL);
	close(FTL);

	return $current_design_delay;
}

sub parse_generate_log {
	my ($cname, $fname) = @_;
	#my $fv_count = 0;
	my $pv_count = 0;

	#open(FITC, '>', "connections.txt") or die "cannot open '$cname/make.log' $!";
	#print FITC "fip_inst top_inst/main_inst_r0/BB_main_1 2\n";
	#print FITC "fip_inst top_inst/main_inst_r0/BB_main_2 2\n";
	#print FITC "fip_inst top_inst/main_inst_r0/BB_main_CTRL 2\n";
	#print FITC "fip_inst top_inst/main_inst_r1/BB_main_1 2\n";
	#print FITC "fip_inst top_inst/main_inst_r1/BB_main_2 2\n";
	#print FITC "fip_inst top_inst/main_inst_r1/BB_main_CTRL 2\n";
	#print FITC "fip_inst top_inst/main_inst_r2/BB_main_1 2\n";
	#print FITC "fip_inst top_inst/main_inst_r2/BB_main_2 2\n";
	#print FITC "fip_inst top_inst/main_inst_r2/BB_main_CTRL 2\n";
	#print FITC "top_inst/main_inst_r0/BB_main_CTRL fip_inst 60\n";
	#print FITC "top_inst/main_inst_r1/BB_main_CTRL fip_inst 60\n";
	#print FITC "top_inst/main_inst_r2/BB_main_CTRL fip_inst 60\n";

	open(FGENH, '<', "make.log") or die "cannot open '$cname/make.log' $!";
	#while(<FGENH>) {
	#	chomp;
	#	if(/^    Total number of sync voters of \[\d+\] = (\d+)$/) {
	#		my $num = $1; $num = "-" if ($num eq "0");
	#		push @verilog_number_of_sync_voters, $num;
	#		$fv_count++;
	#	} elsif(/^    Total number of part voters of \[\d+\] = (\d+)$/) {
	#		my $num = $1; $num = "-" if ($num eq "0");
	#		push @verilog_number_of_part_voters, $num;
	#		$pv_count++;
	#	#} elsif(/^INTERCONNECT ([\_\w]+) ([\_\w]+) (\d+)$/) {
	#	#	if ($3 != 0) {
	#	#		print FITC "top_inst/main_inst_r0/$1 top_inst/main_inst_r0/$2 $3\n";
	#	#		print FITC "top_inst/main_inst_r1/$1 top_inst/main_inst_r0/$1 $3\n";
	#	#		print FITC "top_inst/main_inst_r2/$1 top_inst/main_inst_r0/$1 $3\n";
	#	#		print FITC "top_inst/main_inst_r1/$1 top_inst/main_inst_r1/$2 $3\n";
	#	#		print FITC "top_inst/main_inst_r0/$1 top_inst/main_inst_r1/$1 $3\n";
	#	#		print FITC "top_inst/main_inst_r2/$1 top_inst/main_inst_r1/$1 $3\n";
	#	#		print FITC "top_inst/main_inst_r2/$1 top_inst/main_inst_r2/$2 $3\n";
	#	#		print FITC "top_inst/main_inst_r0/$1 top_inst/main_inst_r2/$1 $3\n";
	#	#		print FITC "top_inst/main_inst_r1/$1 top_inst/main_inst_r2/$1 $3\n";
	#	#	}
	#	#} elsif(/^# DEBUG_TMR=2 - Partition List \(n=(\d+)\)$/) {
	#	#	$max_number_of_partitions = $1;
	#	}
	#}
	while(<FGENH>) {
		chomp;
		if(/^Total Ebits of \'[\_\w]+\' = (\d+)$/) {
			push @verilog_number_of_part_voters, $1;
			$pv_count++;
		}
	}

	#while($fv_count++<$max_number_of_partitions) {
	#	push @verilog_number_of_sync_voters, "";
	#}
	while($pv_count++<$max_number_of_partitions) {
		push @verilog_number_of_part_voters, "";
	}
	close(FGENH);
}

sub parse_vsim_log {
	my ($cname, $fname) = @_;
	open(FSIMH, '<', "vsim.log") or die "cannot open '$cname/vsim.log' $!";
	while(<FSIMH>) {
		chomp;
		if(/^# Cycle: +\d+ Time: +\d+ +RESULT: (\w+)$/) {
			die "simulation fail" if($1 eq "FAIL");
		} elsif(/Result:\s+(\d+)$/) {
			push @sim_expected_result, $1;
		} elsif(/^# Cycles:\s+(\d+)$/) {
			push @sim_number_of_clock_cycles, $1;
			last;
		}
	}
	die "cannot find string in '$cname/vsim.log'" if(eof FSIMH);
	close(FSIMH);
}

sub write_csv_file_vertical {
	my $csv_name = "$DEST/report.csv";
	open(FCSV, '>', $csv_name) or die "cannot open '$csv_name' $!";
	print FCSV ",Mode";

	#for (my $i=0; $i<$max_number_of_partitions; $i++) {
	#	#print FCSV ",Number of Sync Voters of partition $i";
	#	print FCSV ",SV$i";
	#}
	for (my $i=0; $i<$max_number_of_partitions; $i++) {
		#print FCSV ",Number of Part Voters of partition $i";
		print FCSV ",PV$i";
	}
	print FCSV ",LUT,REG";
	print FCSV ",DSP,MEM";
	for (my $i=1; $i<$max_number_of_partitions+1; $i++) {
		print FCSV ",LutP$i";
	}
	for (my $i=1; $i<$max_number_of_partitions+1; $i++) {
		print FCSV ",RegP$i";
	}
	for (my $i=1; $i<$max_number_of_partitions+1; $i++) {
		print FCSV ",DspP$i";
	}
	for (my $i=1; $i<$max_number_of_partitions+1; $i++) {
		print FCSV ",BramP$i";
	}
	print FCSV ",Fmax,Lat,ExTime";
	print FCSV "\n";

	my $val;
	while($val = shift(@name_list)) {
		my @name = split(/_/, $val);
		print FCSV "$name[0],$name[1]";
		#for (my $i=1; $i<$max_number_of_partitions+1; $i++) {
		#	$val = shift(@verilog_number_of_sync_voters);  print FCSV ",$val";
		#}
		for (my $i=1; $i<$max_number_of_partitions+1; $i++) {
			$val = shift(@verilog_number_of_part_voters);  print FCSV ",$val";
		}

		$val = shift(@number_of_slice_luts);       print FCSV ",$val";
		$val = shift(@number_of_slice_registers);  print FCSV ",$val";
		$val = shift(@number_of_dsp48e1s);         print FCSV ",$val";
		$val = shift(@number_of_rams);             print FCSV ",$val";
		for (my $i=1; $i<$max_number_of_partitions+1; $i++) {
			$val = shift(@hier_lut);                  print FCSV ",$val";
		}
		for (my $i=1; $i<$max_number_of_partitions+1; $i++) {
			$val = shift(@hier_reg);                  print FCSV ",$val";
		}
		for (my $i=1; $i<$max_number_of_partitions+1; $i++) {
			$val = shift(@hier_dsp);                  print FCSV ",$val";
		}
		for (my $i=1; $i<$max_number_of_partitions+1; $i++) {
			$val = shift(@hier_bram);                 print FCSV ",$val";
		}
		$val = shift(@sim_max_freq_mhz);           print FCSV ",$val";
		$val = shift(@sim_number_of_clock_cycles); print FCSV ",$val";
		$val = shift(@sim_wall_clock_us);          print FCSV ",$val";

		print FCSV "\n";
	}

	close (FCSV);
}

