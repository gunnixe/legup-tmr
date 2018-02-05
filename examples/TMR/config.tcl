source ../../../legup.tcl

#set_parameter PRINTF_CYCLES 1
set_parameter DEBUG_TMR 2
set_parameter MB_MINIMIZE_HW 1
set_parameter PART_AREA_MARGIN_PERCENTAGE 10
set_parameter USER_REPORT 1

set_parameter LOCAL_RAMS 1

set_parameter GROUP_RAMS 1
#set_operation_latency multiply 1
set_parameter VSIM_NO_ASSERT 1
#set_operation_latency local_mem_dual_port 2

set_parameter VIVADO_FLOW 1

set_parameter FLATTEN_HIERARCHY 1
set_parameter NO_VOTER_AREA_ESTIMATE 1


#set_operation_sharing -off signed_multiply 
#set_operation_sharing -off signed_divide 
#set_operation_sharing -off signed_modulus 
#set_operation_sharing -off unsigned_multiply 
#set_operation_sharing -off unsigned_divide 
#set_operation_sharing -off unsigned_modulus 

#set_project StratixV DE5-Net Tiger_DDR3
#set_project CycloneII DE2 Tiger_SDRAM
#set_project CycloneIV DE2-115 Tiger_SDRAM
#set_project Virtex6 ML605 hw_only
#set_parameter INFERRED_RAM_FORMAT "xilinx"
#set_parameter DIVIDER_MODULE generic



set_project Artix7 NexysVideo hw_only
set_parameter INFERRED_RAM_FORMAT "xilinx"
set_parameter DIVIDER_MODULE generic

set_parameter TMR 1
set_parameter SYNC_VOTER_MODE 1
set_parameter PART_VOTER_MODE 0
set_parameter NUMBER_OF_PARTITIONS 0
set_parameter EBIT_MODE 0

set_project Artix7 NexysVideo hw_only
set_parameter INFERRED_RAM_FORMAT "xilinx"
set_parameter DIVIDER_MODULE generic
