source ../../config.tcl

set_parameter LOCAL_RAMS 1
set_parameter GROUP_RAMS 0
set_parameter GROUP_RAMS_SIMPLE_OFFSET 0
set_parameter CASE_FSM 1

set_parameter VSIM_NO_ASSERT 1

set_parameter TMR 0
set_parameter SYNC_VOTER_MODE 0
# SVOTER_MODE 0=output only,
#             1=all reigisters,
#             2=dest signal(fanin, phi),
#             3=source signal(fanout),
#             4=dest signal long delay
#             4=source signal long delay

set_parameter PART_VOTER_MODE 0
# PVOTER_MODE 0=none
#             1=topological list
