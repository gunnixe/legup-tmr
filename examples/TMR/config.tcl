source ../../../legup.tcl

#set_parameter PRINTF_CYCLES 1
set_parameter DEBUG_TMR 2
set_parameter MB_MINIMIZE_HW 1
set_parameter VOTER_BEFORE_FSM 1
set_parameter PART_AREA_MARGIN_PERCENTAGE 5
#set_parameter PARTITION_AREA_LIMIT 3000
set_parameter NUMBER_OF_PARTITIONS 4

set_parameter GROUP_RAMS 1
#set_operation_latency multiply 1
set_parameter VSIM_NO_ASSERT 1
#set_operation_latency local_mem_dual_port 2

set_parameter EXTRACT_PART_VOTERS 1
set_parameter VIVADO_FLOW 1

#set_project StratixV DE5-Net Tiger_DDR3
#set_project CycloneII DE2 Tiger_SDRAM
#set_project CycloneIV DE2-115 Tiger_SDRAM
#set_project Virtex6 ML605 hw_only
#set_parameter INFERRED_RAM_FORMAT "xilinx"
#set_parameter DIVIDER_MODULE generic

