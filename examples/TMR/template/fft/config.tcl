source ../../config.tcl

#set_parameter CLOCK_PERIOD 5

#set_parameter LOCAL_RAMS 1
#loop_pipeline "loop1"
loop_pipeline "loop2"

#set_parameter PARTITION_AREA_LIMIT 300
set_parameter MB_MINIMIZE_HW 1

set_parameter TMR 1
set_parameter SYNC_VOTER_MODE 2
# VOTER_MODE 0=output only,
#            1=all reigisters,
#            2=dest signal(fanin, phi),
#            3=source signal(fanout),
#            4=dest signal with reg(registered phi)

set_parameter PART_VOTER_MODE 0
