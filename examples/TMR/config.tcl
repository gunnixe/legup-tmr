source ../../../legup.tcl

#set_parameter PRINTF_CYCLES 1
set_parameter DEBUG_TMR 2
set_parameter MB_MINIMIZE_HW 1
set_parameter VOTER_BEFORE_FSM 1
set_parameter PART_AREA_MARGIN_PERCENTAGE 30
#set_parameter PARTITION_AREA_LIMIT 3000
set_parameter NUMBER_OF_PARTITIONS 2

set_parameter GROUP_RAMS 1
#set_operation_latency multiply 1
set_parameter VSIM_NO_ASSERT 1
#set_operation_latency local_mem_dual_port 2

set_parameter EXTRACT_PART_VOTERS 0
set_parameter VIVADO_FLOW 1
set_parameter SEQUENTIAL_PART_VOTER 1
set_parameter EBIT_FOR_PART_VOTER 1
set_parameter EBIT_FOR_SYNC_VOTER 1
set_parameter EBIT_MODE 2
set_parameter EBIT_PIPELINED 1
set_parameter MERGE_PVOTER_WITH_SVOTER 1
set_parameter SEPERATE_BB_CTRL 0
set_parameter PARTITION_SHARED_RESOURCES 1
set_parameter NO_VOTER_AREA_ESTIMATE 1
set_parameter PARTITION_WITH_DATA_WIDTH 1

set_parameter USE_REG_VOTER_FOR_LOCAL_RAMS 1
set_parameter SEQUENTIAL_PART_VOTER 1
set_parameter EBIT_FOR_PART_VOTER 1
set_parameter EBIT_FOR_SYNC_VOTER 1
set_parameter MERGE_PVOTER_WITH_SVOTER 1

set_operation_sharing -off signed_multiply 
set_operation_sharing -off signed_divide 
set_operation_sharing -off signed_modulus 
set_operation_sharing -off unsigned_multiply 
set_operation_sharing -off unsigned_divide 
set_operation_sharing -off unsigned_modulus 

#set_project StratixV DE5-Net Tiger_DDR3
#set_project CycloneII DE2 Tiger_SDRAM
#set_project CycloneIV DE2-115 Tiger_SDRAM
#set_project Virtex6 ML605 hw_only
#set_parameter INFERRED_RAM_FORMAT "xilinx"
#set_parameter DIVIDER_MODULE generic

