set HOME [pwd]
set DUT unsigned_multiply_8
set TEMPLATE template

create_project -force $DUT $HOME/$DUT -part xc7a200tsbg484-1
add_files -norecurse $HOME/$TEMPLATE/$DUT.v
add_files -fileset constrs_1 $HOME/const.xdc
launch_runs impl_1
wait_on_run impl_1
open_run impl_1
report_utilization
report_timing_summary -delay_type min_max -report_unconstrained -check_timing_verbose -max_paths 10
