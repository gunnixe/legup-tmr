set HOME [pwd]

set family [lindex $argv 0]
set board  [lindex $argv 1]
set part "xc7a200tsbg484-1"
set DUT  [lindex $argv 2]

set legupdir  [file normalize [file dirname [info script]]/../]
set board_xdc  $legupdir/boards/$family/$board/$board.xdc

create_project -force $DUT . -part $part
add_files -norecurse $DUT.v
add_files $legupdir/ip/libs/generic/div_unsigned.v
add_files $legupdir/ip/libs/generic/div_signed.v

file copy -force $board_xdc "const.xdc"
add_files -fileset constrs_1 const.xdc

set_property top ARTIX7 [current_fileset]
set_property strategy Flow_PerfOptimized_high [get_runs synth_1]
set_property STEPS.SYNTH_DESIGN.ARGS.FLATTEN_HIERARCHY none [get_runs synth_1]
#set_property STEPS.SYNTH_DESIGN.ARGS.KEEP_EQUIVALENT_REGISTERS true [get_runs synth_1]
#set_property STEPS.SYNTH_DESIGN.ARGS.RESOURCE_SHARING off [get_runs synth_1]

#launch_runs impl_1
#wait_on_run impl_1
#open_run impl_1
#report_utilization
#report_timing_summary -delay_type min_max -report_unconstrained -check_timing_verbose -max_paths 10
