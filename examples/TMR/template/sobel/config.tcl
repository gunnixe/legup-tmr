source ../../config.tcl

#set_parameter CLOCK_PERIOD 5


#set_parameter SDC_NO_CHAINING 1

# SVOTER_MODE 0=output only,
#             1=all reigisters,
#             2=dest signal(fanin, phi),
#             3=source signal(fanout),
#             4=dest signal long delay
#             4=source signal long delay

# PVOTER_MODE 0=none
#             1=topological list

set_parameter TMR 1
set_parameter SYNC_VOTER_MODE 2
set_parameter PART_VOTER_MODE 0
set_parameter LOCAL_RAMS 1
set_parameter USE_REG_VOTER_FOR_LOCAL_RAMS 1
set_parameter PIPELINE_ALL 1
