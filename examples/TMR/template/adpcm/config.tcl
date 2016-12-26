source ../../config.tcl

set_resource_constraint add 2
set_operation_sharing -on signed_add

# temporarily disable local rams
# points-to analysis has trouble with:
#       %1 = call i8* bitcast (void (i8*, i8, i32)* @legup_memset_4 to i8* (i8*, i8, i64)*)(i8* bitcast ([24 x i32]* @tqmf to i8*), i8 0, i64 96) #2
#set_parameter LOCAL_RAMS 0

#set_parameter PARTITION_AREA_LIMIT 2347

set_parameter MB_MINIMIZE_HW 0
set_parameter LOCAL_RAMS 0
#set_parameter GROUP_RAMS 1
#set_parameter GROUP_RAMS_SIMPLE_OFFSET 1
set_parameter CASE_FSM 1

set_parameter TMR 1
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
