//===-- VerilogWriter.cpp -----------------------------------------*- C++
//-*-===//
//
// This file is distributed under the LegUp license. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// Implements VerilogWriter class
//
//===----------------------------------------------------------------------===//

#include "VerilogWriter.h"
#include "Allocation.h"
#include "LegupPass.h"
#include "GenerateRTL.h"
#include "utils.h"
#include "Scheduler.h"
#include "Ram.h"
#include "Allocation.h"
#include "Debug.h"
#include "LegupConfig.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/ErrorHandling.h"
//#include "llvm/Support/system_error.h"
#include <vector>
#include <iostream>
#include <fstream>
#include "llvm/Support/FileSystem.h"

using namespace llvm;
using namespace legup;

// extern std::set<Function*> AcceleratedFcts;
namespace legup {

bool VerilogWriter::isTmrSig(const RTLSignal *sig) {
	if (LEGUP_CONFIG->getParameterInt("TMR") &&
	    (sig->getType() == "reg" || sig->getType() == "wire")) {
		assert(currReplica=="0" || currReplica=="1" || currReplica=="2");
		return true;
	}

	return false;
}

void VerilogWriter::print() {

    // check if it uses synchronization
    alloc->getSynchronizationUsage(syncMap);
    // check if it uses synchronization
    if (!syncMap.empty()) {
        usesSynchronization = true;
    }

    Out << getFileHeader();

    // include all of the files containing custom verilog
    std::set<std::string> customVerilogFileNames =
        LEGUP_CONFIG->customVerilogFileNames();
    for (std::set<std::string>::iterator it = customVerilogFileNames.begin();
         it != customVerilogFileNames.end(); ++it) {
        Out << "`include \"" << static_cast<std::string>(*it) << "\"\n";
    }

    if (LEGUP_CONFIG->isHybridFlow() || LEGUP_CONFIG->isPCIeFlow()) {
        // print Top Module for each accelerated function
        for (std::set<const Function *>::const_iterator it =
                 AcceleratedFcts.begin();
             it != AcceleratedFcts.end(); ++it) {
            printTop(*it);
        }
    } else {
        printTop(NULL);
    }

    printMemoryController();

    // if locks/barriers are used
    // print synchronization module which contains
    // steering logic and instantiations for locks/barriers
    if (usesSynchronization) {
        printSynchronizationController();
    }

    for (Allocation::const_rtl_iterator i = alloc->rtl_begin(),
                                        e = alloc->rtl_end();
         i != e; ++i) {
        printRTL(*i);
    }

    if (alloc->getDbgInfo()->isDebugRtlEnabled()) {
        // Add the trace scheduler logic
        if (alloc->getDbgInfo()->getOptionTraceRegs()) {
            RTLModule *dbgTraceScheduler =
                alloc->getDbgInfo()->getTraceSchedulerRtl();
            printRTL(dbgTraceScheduler);
        }

        // Mux all of the state machines into a single "current_state"
        if (alloc->getDbgInfo()->getOptionPreserveOneHot()) {
            RTLModule *dbgStateMuxer = alloc->getDbgInfo()->getStateMuxer();
            printRTL(dbgStateMuxer);
        }
    }

    // TODO: don't print if we don't have rams
    // if (rams.empty()) return;
    printRAMModule();

    if (LEGUP_CONFIG->getParameterInt("MULTIPUMPING")) {
        printPLLModule();
        printClkFollowerModule();
        printMultipumpModule();
    }

    if (LEGUP_CONFIG->getArbiterUsage()) {
        printArbiterModule();
    }

    if (syncMap.find("lock") != syncMap.end()) {
        printLockModule();
    }

    if (syncMap.find("barrier") != syncMap.end()) {
        printBarrierModule();
    }

    if (LEGUP_CONFIG->isDebuggerUsed()) {
        printDebugModule();
    }

    if (LEGUP_CONFIG->getTopLevelModule() == "") {
        printBoardTops();
        printHex();
    }

    // don't print test bench for hybrid flow
    // or if there is a custom test bench, or
    // if it is the pcie flow
    if (!LEGUP_CONFIG->isHybridFlow() && !LEGUP_CONFIG->usesCustomTestBench() &&
        !LEGUP_CONFIG->isPCIeFlow()) {
        printVerilogTestbench();
    }

    StreamOut << Out.str();
    StreamOut.flush();
}

void VerilogWriter::printBlankDefaultCase(std::string indent) {
    Out << indent << "default:"
        << "\n";
    Out << indent << "begin"
        << "\n";
    Out << indent << "\t// quartus issues a warning if we have no default case"
        << "\n";
    Out << indent << "end"
        << "\n";
}

void VerilogWriter::printPLLModule() {

    Out << "// megafunction wizard: %ALTPLL%\n"
        << "// GENERATION: STANDARD\n"
        << "// VERSION: WM1.0\n"
        << "// MODULE: altpll \n"
        << "\n"
        << "// ============================================================\n"
        << "// File Name: pll.v\n"
        << "// Megafunction Name(s):\n"
        << "// 			altpll\n"
        << "//\n"
        << "// Simulation Library Files(s):\n"
        << "// 			altera_mf\n"
        << "// ============================================================\n"
        << "// ************************************************************\n"
        << "// THIS IS A WIZARD-GENERATED FILE. DO NOT EDIT THIS FILE!\n"
        << "//\n"
        << "// 10.1 Build 197 01/19/2011 SP 1 SJ Full Version\n"
        << "// ************************************************************\n"
        << "\n"
        << "\n"
        << "//Copyright (C) 1991-2011 Altera Corporation\n"
        << "//Your use of Altera Corporation's design tools, logic functions \n"
        << "//and other software and tools, and its AMPP partner logic \n"
        << "//functions, and any output files from any of the foregoing \n"
        << "//(including device programming or simulation files), and any \n"
        << "//associated documentation or information are expressly subject \n"
        << "//to the terms and conditions of the Altera Program License \n"
        << "//Subscription Agreement, Altera MegaCore Function License \n"
        << "//Agreement, or other applicable license agreement, including, \n"
        << "//without limitation, that your use is for the sole purpose of \n"
        << "//programming logic devices manufactured by Altera and sold by \n"
        << "//Altera or its authorized distributors.  Please refer to the \n"
        << "//applicable agreement for further details.\n"
        << "\n"
        << "\n"
        << "// synopsys translate_off\n"
        << "`timescale 1 ns / 1 ns\n"
        << "// synopsys translate_on\n"
        << "module pll (\n"
        << "inclk0,\n"
        << "c0,\n"
        << "c1);\n"
        << "\n"
        << "input	  inclk0;\n"
        << "output	  c0;\n"
        << "output	  c1;\n"
        << "\n"
        << "wire [5:0] sub_wire0;\n"
        << "wire [0:0] sub_wire5 = 1'h0;\n"
        << "wire [1:1] sub_wire2 = sub_wire0[1:1];\n"
        << "wire [0:0] sub_wire1 = sub_wire0[0:0];\n"
        << "wire  c0 = sub_wire1;\n"
        << "wire  c1 = sub_wire2;\n"
        << "wire  sub_wire3 = inclk0;\n"
        << "wire [1:0] sub_wire4 = {sub_wire5, sub_wire3};\n"
        << "\n"
        << "altpll	altpll_component (\n"
        << ".inclk (sub_wire4),\n"
        << ".clk (sub_wire0),\n"
        << ".activeclock (),\n"
        << ".areset (1'b0),\n"
        << ".clkbad (),\n"
        << ".clkena ({6{1'b1}}),\n"
        << ".clkloss (),\n"
        << ".clkswitch (1'b0),\n"
        << ".configupdate (1'b0),\n"
        << ".enable0 (),\n"
        << ".enable1 (),\n"
        << ".extclk (),\n"
        << ".extclkena ({4{1'b1}}),\n"
        << ".fbin (1'b1),\n"
        << ".fbmimicbidir (),\n"
        << ".fbout (),\n"
        << ".fref (),\n"
        << ".icdrclk (),\n"
        << ".locked (),\n"
        << ".pfdena (1'b1),\n"
        << ".phasecounterselect ({4{1'b1}}),\n"
        << ".phasedone (),\n"
        << ".phasestep (1'b1),\n"
        << ".phaseupdown (1'b1),\n"
        << ".pllena (1'b1),\n"
        << ".scanaclr (1'b0),\n"
        << ".scanclk (1'b0),\n"
        << ".scanclkena (1'b1),\n"
        << ".scandata (1'b0),\n"
        << ".scandataout (),\n"
        << ".scandone (),\n"
        << ".scanread (1'b0),\n"
        << ".scanwrite (1'b0),\n"
        << ".sclkout0 (),\n"
        << ".sclkout1 (),\n"
        << ".vcooverrange (),\n"
        << ".vcounderrange ());\n"
        << "defparam\n"
        << "altpll_component.clk0_divide_by = 1,\n"
        << "altpll_component.clk0_duty_cycle = 50,\n"
        << "altpll_component.clk0_multiply_by = 1,\n"
        << "altpll_component.clk0_phase_shift = \"0\",\n"
        << "altpll_component.clk1_divide_by = 1,\n"
        << "altpll_component.clk1_duty_cycle = 50,\n"
        << "altpll_component.clk1_multiply_by = 2,\n"
        << "altpll_component.clk1_phase_shift = \"0\",\n"
        << "altpll_component.compensate_clock = \"CLK0\",\n"
        << "altpll_component.inclk0_input_frequency = 20000,\n"
        << "altpll_component.intended_device_family = \"Cyclone II\",\n"
        << "altpll_component.lpm_hint = \"CBX_MODULE_PREFIX=pll\",\n"
        << "altpll_component.lpm_type = \"altpll\",\n"
        << "altpll_component.operation_mode = \"NORMAL\",\n"
        << "altpll_component.port_activeclock = \"PORT_UNUSED\",\n"
        << "altpll_component.port_areset = \"PORT_UNUSED\",\n"
        << "altpll_component.port_clkbad0 = \"PORT_UNUSED\",\n"
        << "altpll_component.port_clkbad1 = \"PORT_UNUSED\",\n"
        << "altpll_component.port_clkloss = \"PORT_UNUSED\",\n"
        << "altpll_component.port_clkswitch = \"PORT_UNUSED\",\n"
        << "altpll_component.port_configupdate = \"PORT_UNUSED\",\n"
        << "altpll_component.port_fbin = \"PORT_UNUSED\",\n"
        << "altpll_component.port_inclk0 = \"PORT_USED\",\n"
        << "altpll_component.port_inclk1 = \"PORT_UNUSED\",\n"
        << "altpll_component.port_locked = \"PORT_UNUSED\",\n"
        << "altpll_component.port_pfdena = \"PORT_UNUSED\",\n"
        << "altpll_component.port_phasecounterselect = \"PORT_UNUSED\",\n"
        << "altpll_component.port_phasedone = \"PORT_UNUSED\",\n"
        << "altpll_component.port_phasestep = \"PORT_UNUSED\",\n"
        << "altpll_component.port_phaseupdown = \"PORT_UNUSED\",\n"
        << "altpll_component.port_pllena = \"PORT_UNUSED\",\n"
        << "altpll_component.port_scanaclr = \"PORT_UNUSED\",\n"
        << "altpll_component.port_scanclk = \"PORT_UNUSED\",\n"
        << "altpll_component.port_scanclkena = \"PORT_UNUSED\",\n"
        << "altpll_component.port_scandata = \"PORT_UNUSED\",\n"
        << "altpll_component.port_scandataout = \"PORT_UNUSED\",\n"
        << "altpll_component.port_scandone = \"PORT_UNUSED\",\n"
        << "altpll_component.port_scanread = \"PORT_UNUSED\",\n"
        << "altpll_component.port_scanwrite = \"PORT_UNUSED\",\n"
        << "altpll_component.port_clk0 = \"PORT_USED\",\n"
        << "altpll_component.port_clk1 = \"PORT_USED\",\n"
        << "altpll_component.port_clk2 = \"PORT_UNUSED\",\n"
        << "altpll_component.port_clk3 = \"PORT_UNUSED\",\n"
        << "altpll_component.port_clk4 = \"PORT_UNUSED\",\n"
        << "altpll_component.port_clk5 = \"PORT_UNUSED\",\n"
        << "altpll_component.port_clkena0 = \"PORT_UNUSED\",\n"
        << "altpll_component.port_clkena1 = \"PORT_UNUSED\",\n"
        << "altpll_component.port_clkena2 = \"PORT_UNUSED\",\n"
        << "altpll_component.port_clkena3 = \"PORT_UNUSED\",\n"
        << "altpll_component.port_clkena4 = \"PORT_UNUSED\",\n"
        << "altpll_component.port_clkena5 = \"PORT_UNUSED\",\n"
        << "altpll_component.port_extclk0 = \"PORT_UNUSED\",\n"
        << "altpll_component.port_extclk1 = \"PORT_UNUSED\",\n"
        << "altpll_component.port_extclk2 = \"PORT_UNUSED\",\n"
        << "altpll_component.port_extclk3 = \"PORT_UNUSED\";\n"
        << "\n"
        << "\n"
        << "endmodule\n"
        << "\n"
        << "// ============================================================\n"
        << "// CNX file retrieval info\n"
        << "// ============================================================\n"
        << "// Retrieval info: PRIVATE: ACTIVECLK_CHECK STRING \"0\"\n"
        << "// Retrieval info: PRIVATE: BANDWIDTH STRING \"1.000\"\n"
        << "// Retrieval info: PRIVATE: BANDWIDTH_FEATURE_ENABLED STRING "
           "\"0\"\n"
        << "// Retrieval info: PRIVATE: BANDWIDTH_FREQ_UNIT STRING \"MHz\"\n"
        << "// Retrieval info: PRIVATE: BANDWIDTH_PRESET STRING \"Low\"\n"
        << "// Retrieval info: PRIVATE: BANDWIDTH_USE_AUTO STRING \"1\"\n"
        << "// Retrieval info: PRIVATE: BANDWIDTH_USE_CUSTOM STRING \"0\"\n"
        << "// Retrieval info: PRIVATE: BANDWIDTH_USE_PRESET STRING \"0\"\n"
        << "// Retrieval info: PRIVATE: CLKBAD_SWITCHOVER_CHECK STRING \"0\"\n"
        << "// Retrieval info: PRIVATE: CLKLOSS_CHECK STRING \"0\"\n"
        << "// Retrieval info: PRIVATE: CLKSWITCH_CHECK STRING \"1\"\n"
        << "// Retrieval info: PRIVATE: CNX_NO_COMPENSATE_RADIO STRING \"0\"\n"
        << "// Retrieval info: PRIVATE: CREATE_CLKBAD_CHECK STRING \"0\"\n"
        << "// Retrieval info: PRIVATE: CREATE_INCLK1_CHECK STRING \"0\"\n"
        << "// Retrieval info: PRIVATE: CUR_DEDICATED_CLK STRING \"c0\"\n"
        << "// Retrieval info: PRIVATE: CUR_FBIN_CLK STRING \"c0\"\n"
        << "// Retrieval info: PRIVATE: DEVICE_SPEED_GRADE STRING \"6\"\n"
        << "// Retrieval info: PRIVATE: DIV_FACTOR0 NUMERIC \"1\"\n"
        << "// Retrieval info: PRIVATE: DIV_FACTOR1 NUMERIC \"1\"\n"
        << "// Retrieval info: PRIVATE: DUTY_CYCLE0 STRING \"50.00000000\"\n"
        << "// Retrieval info: PRIVATE: DUTY_CYCLE1 STRING \"50.00000000\"\n"
        << "// Retrieval info: PRIVATE: EFF_OUTPUT_FREQ_VALUE0 STRING "
           "\"100.000000\"\n"
        << "// Retrieval info: PRIVATE: EFF_OUTPUT_FREQ_VALUE1 STRING "
           "\"200.000000\"\n"
        << "// Retrieval info: PRIVATE: EXPLICIT_SWITCHOVER_COUNTER STRING "
           "\"0\"\n"
        << "// Retrieval info: PRIVATE: EXT_FEEDBACK_RADIO STRING \"0\"\n"
        << "// Retrieval info: PRIVATE: GLOCKED_COUNTER_EDIT_CHANGED STRING "
           "\"1\"\n"
        << "// Retrieval info: PRIVATE: GLOCKED_FEATURE_ENABLED STRING \"1\"\n"
        << "// Retrieval info: PRIVATE: GLOCKED_MODE_CHECK STRING \"0\"\n"
        << "// Retrieval info: PRIVATE: GLOCK_COUNTER_EDIT NUMERIC "
           "\"1048575\"\n"
        << "// Retrieval info: PRIVATE: HAS_MANUAL_SWITCHOVER STRING \"1\"\n"
        << "// Retrieval info: PRIVATE: INCLK0_FREQ_EDIT STRING \"50.000\"\n"
        << "// Retrieval info: PRIVATE: INCLK0_FREQ_UNIT_COMBO STRING \"MHz\"\n"
        << "// Retrieval info: PRIVATE: INCLK1_FREQ_EDIT STRING \"100.000\"\n"
        << "// Retrieval info: PRIVATE: INCLK1_FREQ_EDIT_CHANGED STRING \"1\"\n"
        << "// Retrieval info: PRIVATE: INCLK1_FREQ_UNIT_CHANGED STRING \"1\"\n"
        << "// Retrieval info: PRIVATE: INCLK1_FREQ_UNIT_COMBO STRING \"MHz\"\n"
        << "// Retrieval info: PRIVATE: INTENDED_DEVICE_FAMILY STRING "
           "\"Cyclone II\"\n"
        << "// Retrieval info: PRIVATE: INT_FEEDBACK__MODE_RADIO STRING \"1\"\n"
        << "// Retrieval info: PRIVATE: LOCKED_OUTPUT_CHECK STRING \"0\"\n"
        << "// Retrieval info: PRIVATE: LONG_SCAN_RADIO STRING \"1\"\n"
        << "// Retrieval info: PRIVATE: LVDS_MODE_DATA_RATE STRING \"Not "
           "Available\"\n"
        << "// Retrieval info: PRIVATE: LVDS_MODE_DATA_RATE_DIRTY NUMERIC "
           "\"0\"\n"
        << "// Retrieval info: PRIVATE: LVDS_PHASE_SHIFT_UNIT0 STRING \"deg\"\n"
        << "// Retrieval info: PRIVATE: LVDS_PHASE_SHIFT_UNIT1 STRING \"deg\"\n"
        << "// Retrieval info: PRIVATE: MIG_DEVICE_SPEED_GRADE STRING \"Any\"\n"
        << "// Retrieval info: PRIVATE: MIRROR_CLK0 STRING \"0\"\n"
        << "// Retrieval info: PRIVATE: MIRROR_CLK1 STRING \"0\"\n"
        << "// Retrieval info: PRIVATE: MULT_FACTOR0 NUMERIC \"4\"\n"
        << "// Retrieval info: PRIVATE: MULT_FACTOR1 NUMERIC \"1\"\n"
        << "// Retrieval info: PRIVATE: NORMAL_MODE_RADIO STRING \"1\"\n"
        << "// Retrieval info: PRIVATE: OUTPUT_FREQ0 STRING \"100.00000000\"\n"
        << "// Retrieval info: PRIVATE: OUTPUT_FREQ1 STRING \"200.00000000\"\n"
        << "// Retrieval info: PRIVATE: OUTPUT_FREQ_MODE0 STRING \"1\"\n"
        << "// Retrieval info: PRIVATE: OUTPUT_FREQ_MODE1 STRING \"1\"\n"
        << "// Retrieval info: PRIVATE: OUTPUT_FREQ_UNIT0 STRING \"MHz\"\n"
        << "// Retrieval info: PRIVATE: OUTPUT_FREQ_UNIT1 STRING \"MHz\"\n"
        << "// Retrieval info: PRIVATE: PHASE_RECONFIG_FEATURE_ENABLED STRING "
           "\"0\"\n"
        << "// Retrieval info: PRIVATE: PHASE_RECONFIG_INPUTS_CHECK STRING "
           "\"0\"\n"
        << "// Retrieval info: PRIVATE: PHASE_SHIFT0 STRING \"0.00000000\"\n"
        << "// Retrieval info: PRIVATE: PHASE_SHIFT1 STRING \"0.00000000\"\n"
        << "// Retrieval info: PRIVATE: PHASE_SHIFT_STEP_ENABLED_CHECK STRING "
           "\"0\"\n"
        << "// Retrieval info: PRIVATE: PHASE_SHIFT_UNIT0 STRING \"deg\"\n"
        << "// Retrieval info: PRIVATE: PHASE_SHIFT_UNIT1 STRING \"deg\"\n"
        << "// Retrieval info: PRIVATE: PLL_ADVANCED_PARAM_CHECK STRING \"0\"\n"
        << "// Retrieval info: PRIVATE: PLL_ARESET_CHECK STRING \"0\"\n"
        << "// Retrieval info: PRIVATE: PLL_AUTOPLL_CHECK NUMERIC \"1\"\n"
        << "// Retrieval info: PRIVATE: PLL_ENA_CHECK STRING \"0\"\n"
        << "// Retrieval info: PRIVATE: PLL_ENHPLL_CHECK NUMERIC \"0\"\n"
        << "// Retrieval info: PRIVATE: PLL_FASTPLL_CHECK NUMERIC \"0\"\n"
        << "// Retrieval info: PRIVATE: PLL_FBMIMIC_CHECK STRING \"0\"\n"
        << "// Retrieval info: PRIVATE: PLL_LVDS_PLL_CHECK NUMERIC \"0\"\n"
        << "// Retrieval info: PRIVATE: PLL_PFDENA_CHECK STRING \"0\"\n"
        << "// Retrieval info: PRIVATE: PLL_TARGET_HARCOPY_CHECK NUMERIC "
           "\"0\"\n"
        << "// Retrieval info: PRIVATE: PRIMARY_CLK_COMBO STRING \"inclk0\"\n"
        << "// Retrieval info: PRIVATE: RECONFIG_FILE STRING \"pll.mif\"\n"
        << "// Retrieval info: PRIVATE: SACN_INPUTS_CHECK STRING \"0\"\n"
        << "// Retrieval info: PRIVATE: SCAN_FEATURE_ENABLED STRING \"0\"\n"
        << "// Retrieval info: PRIVATE: SELF_RESET_LOCK_LOSS STRING \"0\"\n"
        << "// Retrieval info: PRIVATE: SHORT_SCAN_RADIO STRING \"0\"\n"
        << "// Retrieval info: PRIVATE: SPREAD_FEATURE_ENABLED STRING \"0\"\n"
        << "// Retrieval info: PRIVATE: SPREAD_FREQ STRING \"50.000\"\n"
        << "// Retrieval info: PRIVATE: SPREAD_FREQ_UNIT STRING \"KHz\"\n"
        << "// Retrieval info: PRIVATE: SPREAD_PERCENT STRING \"0.500\"\n"
        << "// Retrieval info: PRIVATE: SPREAD_USE STRING \"0\"\n"
        << "// Retrieval info: PRIVATE: SRC_SYNCH_COMP_RADIO STRING \"0\"\n"
        << "// Retrieval info: PRIVATE: STICKY_CLK0 STRING \"1\"\n"
        << "// Retrieval info: PRIVATE: STICKY_CLK1 STRING \"1\"\n"
        << "// Retrieval info: PRIVATE: SWITCHOVER_COUNT_EDIT NUMERIC \"1\"\n"
        << "// Retrieval info: PRIVATE: SWITCHOVER_FEATURE_ENABLED STRING "
           "\"1\"\n"
        << "// Retrieval info: PRIVATE: SYNTH_WRAPPER_GEN_POSTFIX STRING "
           "\"0\"\n"
        << "// Retrieval info: PRIVATE: USE_CLK0 STRING \"1\"\n"
        << "// Retrieval info: PRIVATE: USE_CLK1 STRING \"1\"\n"
        << "// Retrieval info: PRIVATE: USE_CLKENA0 STRING \"0\"\n"
        << "// Retrieval info: PRIVATE: USE_CLKENA1 STRING \"0\"\n"
        << "// Retrieval info: PRIVATE: USE_MIL_SPEED_GRADE NUMERIC \"0\"\n"
        << "// Retrieval info: PRIVATE: ZERO_DELAY_RADIO STRING \"0\"\n"
        << "// Retrieval info: LIBRARY: altera_mf "
           "altera_mf.altera_mf_components.all\n"
        << "// Retrieval info: CONSTANT: CLK0_DIVIDE_BY NUMERIC \"1\"\n"
        << "// Retrieval info: CONSTANT: CLK0_DUTY_CYCLE NUMERIC \"50\"\n"
        << "// Retrieval info: CONSTANT: CLK0_MULTIPLY_BY NUMERIC \"2\"\n"
        << "// Retrieval info: CONSTANT: CLK0_PHASE_SHIFT STRING \"0\"\n"
        << "// Retrieval info: CONSTANT: CLK1_DIVIDE_BY NUMERIC \"1\"\n"
        << "// Retrieval info: CONSTANT: CLK1_DUTY_CYCLE NUMERIC \"50\"\n"
        << "// Retrieval info: CONSTANT: CLK1_MULTIPLY_BY NUMERIC \"4\"\n"
        << "// Retrieval info: CONSTANT: CLK1_PHASE_SHIFT STRING \"0\"\n"
        << "// Retrieval info: CONSTANT: COMPENSATE_CLOCK STRING \"CLK0\"\n"
        << "// Retrieval info: CONSTANT: INCLK0_INPUT_FREQUENCY NUMERIC "
           "\"20000\"\n"
        << "// Retrieval info: CONSTANT: INTENDED_DEVICE_FAMILY STRING "
           "\"Cyclone II\"\n"
        << "// Retrieval info: CONSTANT: LPM_TYPE STRING \"altpll\"\n"
        << "// Retrieval info: CONSTANT: OPERATION_MODE STRING \"NORMAL\"\n"
        << "// Retrieval info: CONSTANT: PORT_ACTIVECLOCK STRING "
           "\"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_ARESET STRING \"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_CLKBAD0 STRING \"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_CLKBAD1 STRING \"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_CLKLOSS STRING \"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_CLKSWITCH STRING "
           "\"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_CONFIGUPDATE STRING "
           "\"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_FBIN STRING \"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_INCLK0 STRING \"PORT_USED\"\n"
        << "// Retrieval info: CONSTANT: PORT_INCLK1 STRING \"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_LOCKED STRING \"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_PFDENA STRING \"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_PHASECOUNTERSELECT STRING "
           "\"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_PHASEDONE STRING "
           "\"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_PHASESTEP STRING "
           "\"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_PHASEUPDOWN STRING "
           "\"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_PLLENA STRING \"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_SCANACLR STRING \"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_SCANCLK STRING \"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_SCANCLKENA STRING "
           "\"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_SCANDATA STRING \"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_SCANDATAOUT STRING "
           "\"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_SCANDONE STRING \"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_SCANREAD STRING \"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_SCANWRITE STRING "
           "\"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_clk0 STRING \"PORT_USED\"\n"
        << "// Retrieval info: CONSTANT: PORT_clk1 STRING \"PORT_USED\"\n"
        << "// Retrieval info: CONSTANT: PORT_clk2 STRING \"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_clk3 STRING \"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_clk4 STRING \"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_clk5 STRING \"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_clkena0 STRING \"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_clkena1 STRING \"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_clkena2 STRING \"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_clkena3 STRING \"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_clkena4 STRING \"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_clkena5 STRING \"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_extclk0 STRING \"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_extclk1 STRING \"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_extclk2 STRING \"PORT_UNUSED\"\n"
        << "// Retrieval info: CONSTANT: PORT_extclk3 STRING \"PORT_UNUSED\"\n"
        << "// Retrieval info: USED_PORT: @clk 0 0 6 0 OUTPUT_CLK_EXT VCC "
           "\"@clk[5..0]\"\n"
        << "// Retrieval info: USED_PORT: @extclk 0 0 4 0 OUTPUT_CLK_EXT VCC "
           "\"@extclk[3..0]\"\n"
        << "// Retrieval info: USED_PORT: c0 0 0 0 0 OUTPUT_CLK_EXT VCC "
           "\"c0\"\n"
        << "// Retrieval info: USED_PORT: c1 0 0 0 0 OUTPUT_CLK_EXT VCC "
           "\"c1\"\n"
        << "// Retrieval info: USED_PORT: inclk0 0 0 0 0 INPUT_CLK_EXT GND "
           "\"inclk0\"\n"
        << "// Retrieval info: CONNECT: @inclk 0 0 1 1 GND 0 0 0 0\n"
        << "// Retrieval info: CONNECT: @inclk 0 0 1 0 inclk0 0 0 0 0\n"
        << "// Retrieval info: CONNECT: c0 0 0 0 0 @clk 0 0 1 0\n"
        << "// Retrieval info: CONNECT: c1 0 0 0 0 @clk 0 0 1 1\n"
        << "// Retrieval info: GEN_FILE: TYPE_NORMAL pll.v TRUE\n"
        << "// Retrieval info: GEN_FILE: TYPE_NORMAL pll.ppf TRUE\n"
        << "// Retrieval info: GEN_FILE: TYPE_NORMAL pll.inc FALSE\n"
        << "// Retrieval info: GEN_FILE: TYPE_NORMAL pll.cmp FALSE\n"
        << "// Retrieval info: GEN_FILE: TYPE_NORMAL pll.bsf FALSE\n"
        << "// Retrieval info: GEN_FILE: TYPE_NORMAL pll_inst.v TRUE\n"
        << "// Retrieval info: GEN_FILE: TYPE_NORMAL pll_bb.v TRUE\n"
        << "// Retrieval info: LIB_FILE: altera_mf\n"
        << "// Retrieval info: CBX_MODULE_PREFIX: ON\n";
}

void VerilogWriter::printClkFollowerModule() {
    Out << "// see XAPP706\n"
        << "// the clock follower avoids hold-time violations by generating a "
           "signal\n"
        << "// from a register clocked by clk2x that follows clk1x\n"
        << "module clock_follower (reset, clk1x, clk2x, clk1x_follower);\n"
        << "input clk1x, clk2x, reset;\n"
        << "output reg clk1x_follower;\n"
        << "reg toggle, tog_1;\n"
        << "\n"
        << "always @(posedge clk1x or posedge reset)\n"
        << "if (reset)\n"
        << "toggle <= 0;\n"
        << "else\n"
        << "toggle <= ~toggle;\n"
        << "\n"
        << "always @(posedge clk2x)\n"
        << "tog_1 <= toggle;\n"
        << "\n"
        << "always @(posedge clk2x)\n"
        << "clk1x_follower <= ~(toggle ^ tog_1);\n"
        << "\n"
        << "endmodule\n";
}

void VerilogWriter::printMultipumpModule() {
    // multiplier is clocked at 2x the system clock
    int stages = 0;
    LEGUP_CONFIG->getOperationLatency("signed_multiply", &stages);
    assert(stages && "Must set multiplier latency for multipumping.");

    bool inputStage = (stages >= 3);
    if (inputStage) {
        // use one stage at the inputs
        stages--;
    }
    unsigned lpm_pipeline = 2 * stages;

    Out << "module multipump (\n"
        << "clk,\n"
        << "clk2x,\n"
        << "clk1x_follower,\n"
        << "inA,\n"
        << "inB,\n"
        << "outAxB,\n"
        << "inC,\n"
        << "inD,\n"
        << "clken,\n"
        << "outCxD);\n"
        << "\n"
        << "parameter size = 32;\n"
        << "parameter sign = \"UNSIGNED\";\n"
        << "\n"
        << "input	   clk, clk2x, clk1x_follower, clken;\n"
        << "input	   [size-1:0] inA, inB, inC, inD;\n"
        << "output reg [size*2-1:0] outAxB, outCxD;\n";
    if (inputStage) {
        Out << "reg	   [size-1:0] inAreg, inBreg, inCreg, inDreg;\n"
            << "always @(posedge clk) begin\n"
            << "inAreg <= inA;\n"
            << "inBreg <= inB;\n"
            << "inCreg <= inC;\n"
            << "inDreg <= inD;\n"
            << "end\n"
            << "\n"
            << "\n";
    }
    Out << "\n"
        << "wire [size*2-1:0] dsp_out;\n"
        << "reg [size*2-1:0]  resultAB_reg, resultCD_reg;\n"
        << "\n"
        << "\n"
        << "always @(*) begin\n"
        << "outCxD = resultCD_reg;\n"
        << "outAxB = resultAB_reg;\n"
        << "end\n"
        << "\n"
        << "\n"
        << "reg [size*2-1:0] dsp_out_fast;\n"
        << "\n"
        << "\n"
        << "\n"
        << "always @(posedge clk2x) begin\n"
        << "dsp_out_fast <= dsp_out;\n"
        << "end\n"
        << "\n"
        << "\n"
        << "// the C x D result is ready\n"
        << "always @(*) begin\n"
        << "resultCD_reg <= dsp_out;\n"
        << "end\n"
        << "\n"
        << "// the A x B result is ready\n"
        << "always @(*) begin\n"
        << "resultAB_reg <= dsp_out_fast;\n"
        << "end\n"
        << "\n"
        << "\n"
        << "\n"
        << "wire      mux_sel;\n"
        << "\n"
        << "\n"
        << "assign mux_sel = ~clk1x_follower;\n"
        << "\n"
        << "reg       [size-1:0]  dataa;\n"
        << "reg       [size-1:0]  datab;\n"
        << "wire      [size-1:0]  dataa_wire;\n"
        << "wire      [size-1:0]  datab_wire;\n"
        << "assign dataa_wire = dataa;\n"
        << "assign datab_wire = datab;\n"
        << "\n"
        << "always @(*)\n"
        << "begin\n"
        << "if (mux_sel == 0) \n"
        << "begin\n";
    if (inputStage) {
        Out << "dataa = inAreg;\n"
            << "datab = inBreg;\n";
    } else {
        Out << "dataa = inA;\n"
            << "datab = inB;\n";
    }
    Out << "end\n"
        << "else \n"
        << "begin\n";
    if (inputStage) {
        Out << "dataa = inCreg;\n"
            << "datab = inDreg;\n";
    } else {
        Out << "dataa = inC;\n"
            << "datab = inD;\n";
    }
    Out << "end\n"
        << "end\n"
        << "\n"
        << "\n"
        << "\n"
        << "// DSP multiplier - has two pipeline stages, so both inputs and "
           "outputs\n"
        << "// are registered\n"
        << "lpm_mult	lpm_mult_component (\n";
    if (lpm_pipeline == 0) {
        Out << ".clock (),\n";
    } else {
        Out << ".clock (clk2x),\n";
    }
    Out << ".dataa (dataa_wire),\n"
        << ".datab (datab_wire),\n"
        << ".result (dsp_out),\n"
        << ".aclr (1'b0),\n"
        << ".clken (1'b1),\n"
        << ".sum (1'b0));\n"
        << "\n"
        << "defparam\n"
        << "lpm_mult_component.lpm_hint = "
           "\"DEDICATED_MULTIPLIER_CIRCUITRY=YES,MAXIMIZE_SPEED=5\",\n"
        << "lpm_mult_component.lpm_representation = sign,\n"
        << "lpm_mult_component.lpm_type = \"LPM_MULT\",\n"
        << "lpm_mult_component.lpm_pipeline = " << lpm_pipeline << ",\n"
        << "lpm_mult_component.lpm_widtha = size,\n"
        << "lpm_mult_component.lpm_widthb = size,\n"
        << "lpm_mult_component.lpm_widthp = size*2;\n"
        << "\n"
        << "\n"
        << "endmodule\n";
}

// print lock instance
void VerilogWriter::printLockInstance(int lockIndex) {

    std::string instanceName = "lock_inst" + utostr(lockIndex);
    std::string mutex_address = instanceName + "_address";
    std::string mutex_read = instanceName + "_enable";
    std::string mutex_write = instanceName + "_write_enable";
    std::string mutex_readdata = instanceName + "_out";
    std::string mutex_writedata = instanceName + "_in";

    Out << "legup_HW_lock " << instanceName << " (\n"
        << "   .clk( clk ),\n"
        << "   .reset( reset ),\n"
        << "   .address( " << mutex_address << " ),\n"
        << "   .read( " << mutex_read << " ),\n"
        << "   .write( " << mutex_write << " ),\n"
        << "   .readdata( " << mutex_readdata << " ),\n"
        << "   .writedata( " << mutex_writedata << " )\n"
        << ");\n\n";
}

// print barrier instance
void VerilogWriter::printBarrierInstance(int barrierIndex) {

    std::string instanceName = "barrier_inst" + utostr(barrierIndex);
    std::string barrier_address = instanceName + "_address";
    std::string barrier_read = instanceName + "_enable";
    std::string barrier_write = instanceName + "_write_enable";
    std::string barrier_readdata = instanceName + "_out";
    std::string barrier_writedata = instanceName + "_in";

    Out << "legup_HW_barrier " << instanceName << " (\n"
        << "   .clk( clk ),\n"
        << "   .reset( reset ),\n"
        << "   .address( " << barrier_address << " ),\n"
        << "   .read( " << barrier_read << " ),\n"
        << "   .write( " << barrier_write << " ),\n"
        << "   .readdata( " << barrier_readdata << " ),\n"
        << "   .writedata( " << barrier_writedata << " )\n"
        << ");\n\n";
}

// print barrier module
void VerilogWriter::printBarrierModule() {
    Out << "`timescale 1ns / 1ns\n"
        << "module legup_HW_barrier (\n"
        << "                  clk,\n"
        << "                  reset,\n"
        << "                  address,\n"
        << "                  read,\n"
        << "                  write,\n"
        << "                  readdata,\n"
        << "                  writedata\n"
        << ");\n\n"
        <<

        "input                                      clk;\n"
        << "input                                      reset;\n"
        << "input                                      address;\n"
        << "input                                      read;\n"
        << "input                                      write;\n"
        << "output [`MEMORY_CONTROLLER_DATA_SIZE-1:0]  readdata;\n"
        << "input  [`MEMORY_CONTROLLER_DATA_SIZE-1:0]  writedata;\n\n"
        <<

        "reg    [`MEMORY_CONTROLLER_DATA_SIZE-1:0]  barrier_count;\n"
        << "reg    [`MEMORY_CONTROLLER_DATA_SIZE-1:0]  numthreads;\n"
        << "wire   [`MEMORY_CONTROLLER_DATA_SIZE-1:0]  writedata;\n\n"
        <<

        "always @(posedge clk)\n"
        << "begin\n"
        << "    if (reset)\n"
        << "    begin\n"
        << "        numthreads <= 0;\n"
        << "    end\n"
        << "    //barrier initialization to store the number of threads\n"
        << "    else if (write && !address)\n"
        << "    begin\n"
        << "        numthreads <= writedata;\n"
        << "    end\n"
        << "end\n\n"
        <<

        "always @(posedge clk)\n"
        << "begin\n"
        << "    if (reset)\n"
        << "    begin\n"
        << "        barrier_count <= 0;\n"
        << "    end\n"
        << "    //whenever each thread reaches the barrier, it writes to the "
           "barrier, which increments the counter\n"
        << "    else if (write && address)\n"
        << "    begin\n"
        << "        barrier_count <= barrier_count + 1;\n"
        << "    end\n"
        << "    //if the counter reaches the total number of threads, it "
           "resets to zero\n"
        << "    //this is needed for when the same barrier is used multiplie "
           "times without initializing the barrier again\n"
        << "    else if (barrier_count == numthreads)\n"
        << "    begin\n"
        << "        barrier_count <= 0;\n"
        << "    end\n"
        << "end\n\n"
        <<

        "//if the barrier count is zero, that means all threads have reached "
        "the barrier\n"
        << "assign readdata = barrier_count;\n\n"
        <<

        "endmodule\n\n";
}

// print lock module
void VerilogWriter::printLockModule() {

    Out << "`timescale 1 ns / 1 ns\n"
        << "module legup_HW_lock (\n"
        << "              clk,\n"
        << "  	 		   reset,\n"
        << "              address,\n"
        << "              read,\n"
        << "              write,\n"
        << "              readdata,\n"
        << "              writedata\n"
        << ");\n"
        << "\n"
        << "input  clk;\n"
        << "input  reset;\n"
        << "input  address;\n"
        << "input  read;\n"
        << "input  write;\n"
        << "input  writedata;\n"
        << "output readdata; \n"
        << "\n"
        << "wire    mutex_free;\n"
        << "reg     mutex_owner;\n"
        << "reg     mutex_status;\n"
        << "wire    owner_valid;\n"
        << "wire    accel_id;\n"
        << "\n"
        << "assign accel_id = writedata;\n"
        << "\n"
        << "always @(posedge clk)\n"
        << "begin\n"
        << "  if (reset)\n"
        << "  begin\n"
        << "     mutex_status <= 0;\n"
        << "	  mutex_owner <= 0;\n"
        << "  end\n"
        << "  //if mutex is free and someone tries to get the mutex\n"
        << "  else if (mutex_free && read && !address)\n"
        << "  begin\n"
        << "     mutex_status <= 1;\n"
        << "	  mutex_owner <= 1;\n"
        << "  end\n"
        << "  //if mutex is taken and the owner wants to release the mutex\n"
        << "  else if (owner_valid && write && address)\n"
        << "  begin\n"
        << "	  mutex_status <= 0;\n"
        << "	  mutex_owner <= 0;\n"
        << "  end\n"
        << "end\n"
        << "\n"
        << "assign readdata = mutex_free;\n"
        << "assign mutex_free = mutex_status == 0;\n"
        << "assign owner_valid = mutex_owner == accel_id;\n"
        << "\n"
        << "endmodule\n\n";
}

void VerilogWriter::printPriorityArbiterModule() {
    /*Out <<
    "module priority_arbiter(\n" <<
    "	req,\n" <<
    "	gnt\n" <<
    ");\n" <<
    "\nparameter n=2;\n" <<
    "input [n-1:0] req;\n" <<
    "output [n-1:0] gnt;\n" <<
    "wire [n-1:0] c = {1'b1,(~req[n-1:1] & c[n-1:1])};\n" <<
    "wire [n-1:0] gnt = req & c;\n" <<
    "endmodule\n\n";*/
    /*
        Out <<
        "module priority_arbiter(\n" <<
        "	clk,\n" <<
        "	req,\n" <<
        "	gnt,\n" <<
        "	waitrequest\n" <<
        ");\n" <<
        "\n" <<
        "parameter n=2;\n" <<
        "input clk;\n" <<
        "input [n-1:0] req;\n" <<
        "input waitrequest;\n" <<
        "output [n-1:0] gnt;\n" <<
        "reg [n-1:0] c; \n" <<
        "reg [n-1:0] gnt;\n" <<
        "reg [n-1:0] gnt_reg;\n" <<
        "reg waitrequest_reg;\n" <<
        "\n" <<
        "always @(posedge clk)\n" <<
        "begin\n" <<
        "\twaitrequest_reg <= waitrequest;\n" <<
        "\tif (waitrequest)\n" <<
        "\tbegin\n" <<
        "\t\tgnt_reg <= gnt;\n" <<
        "\tend\n" <<
        "end\n" <<
        "\n" <<
        "always @(*)\n" <<
        "begin\n" <<
        "\tif (waitrequest_reg) begin\n" <<
        "\t\tgnt = gnt_reg;\n" <<
        "\tend else begin\n" <<
        "\t\tc = {1'b1,(~req[n-1:1] & c[n-1:1])};\n" <<
        "\t\tgnt = req & c;\n" <<
        "\tend\n" <<
        "end\n" <<
        "\n" <<
        "endmodule\n";*/
    Out << "module priority_arbiter(\n"
        << "	clk,\n"
        << "	req,\n"
        << "	gnt,\n"
        << "	waitrequest\n"
        << ");\n"
        << "\n"
        << "parameter n=2;\n"
        << "input clk;\n"
        << "input [n-1:0] req;\n"
        << "input waitrequest;\n"
        << "output [n-1:0] gnt;\n"
        << "reg [n-1:0] gnt;\n"
        << "reg [n-1:0] gnt_reg;\n"
        << "reg waitrequest_reg;\n"
        << "integer i;\n"
        << "\n"
        << "always @(posedge clk)\n"
        << "begin\n"
        << "\twaitrequest_reg <= waitrequest;\n"
        << "\tif (waitrequest)\n"
        << "\tbegin\n"
        << "\t\tgnt_reg <= gnt;\n"
        << "\tend\n"
        << "end\n"
        << "\n"
        << "always @(*)\n"
        << "begin : priorityArbiter\n"
        << "\tif (waitrequest_reg) begin\n"
        << "\t\tgnt = gnt_reg;\n"
        << "\tend else begin\n"
        << "\t\tgnt = {n{1'b0}};\n"
        << "\t\tfor (i=n-1; i>=0; i=i-1) begin\n"
        << "\t\t\tif (req[i]) begin\n"
        << "\t\t\t\tgnt[i] = 1'b1;\n"
        << "\t\t\t\tdisable priorityArbiter;\n"
        << "\t\t\tend\n"
        << "\t\tend\n"
        << "\tend\n"
        << "end\n"
        << "\n"
        << "endmodule\n\n";
}

void VerilogWriter::printArbiterModule() {

    // gnt has a single cycle latency
    // gnt is given one cycle after req
    Out << "module round_robin_arbiter (\n"
        << "\trst_an,\n"
        << "\tclk,\n"
        << "\treq_in,\n"
        << "\tgrant_final,\n" <<
        //"\tgrant,\n" <<
        "\twaitrequest\n"
        << ");\n\n"
        <<

        "parameter N = 4;\n"
        <<

        "input		rst_an;\n"
        << "input		clk;\n"
        << "input	[N-1:0]	req_in;\n"
        << "input waitrequest;\n"
        << "output	[N-1:0]	grant_final;\n\n"
        <<

        "wire [N-1:0] req_final;\n"
        << "wire [N-1:0] req;\n"
        << "reg	[N-1:0]	req_reg;\n"
        << "reg	[N-1:0]	rotate_ptr;\n"
        << "wire	[N-1:0]	mask_req;\n"
        << "wire	[N-1:0]	mask_grant;\n"
        << "wire	[N-1:0]	grant_comb;\n"
        << "reg	[N-1:0]	grant;\n"
        << "wire		no_mask_req;\n"
        << "wire	[N-1:0] nomask_grant;\n"
        << "wire		update_ptr;\n"
        << "genvar i;\n\n";

    // TO DO: Fix this to work for both cases
    // For the hybrid case, the grant signal should only be asserted for once
    // cycle
    // once the grant signal goes low
    // or else it will make multiple requests over Avalon?
    // for pure HW case, it makes a combinational loop for Pthreads+OpenMP case
    // when the waitrequest is in the if condition
    if (LEGUP_CONFIG->isHybridFlow()) {
        Out << "assign grant_final = (waitrequest)? {N{1'b0}}: grant_comb;\n";
    } else {
        Out << "assign grant_final = grant_comb;\n";
    }
    /*
    if (LEGUP_CONFIG->isHybridFlow()) {
        Out << "assign grant_final = (waitrequest)? {N{1'b0}}: grant;\n";
    } else {
        Out << "assign grant_final = grant;\n";
    }*/
    Out << "always @(posedge clk)\n"
        << "begin\n"
        << "\tif (!waitrequest)\n"
        << "\treq_reg[N-1:0] <= req_final[N-1:0];\n"
        << "end\n\n"
        <<

        "assign req_final[N-1:0] = req_in[N-1:0] & ~req_reg[N-1:0];\n" <<
        //"assign req[N-1:0] = req_final[N-1:0];\n" <<
        "assign req[N-1:0] = req_in[N-1:0];\n"
        << "// rotate pointer update logic\n"
        << "assign update_ptr = |grant[N-1:0];\n"
        << "always @ (posedge clk or negedge rst_an)\n"
        << "begin\n"
        << "\tif (!rst_an)\n"
        << "\tbegin\n"
        << "\t\trotate_ptr[0] <= 1'b1;\n"
        << "\t\trotate_ptr[1] <= 1'b1;\n"
        << "\tend\n"
        << "\telse if (update_ptr)\n"
        << "\tbegin\n"
        << "\t\t// note: N must be at least 2\n"
        << "\t\trotate_ptr[0] <= grant[N-1];\n"
        << "\t\trotate_ptr[1] <= grant[N-1] | grant[0];\n"
        << "\tend\n"
        << "end\n\n"
        <<

        "generate\n"
        << "for (i=2;i<N;i=i+1) begin : abc\n"
        << "always @ (posedge clk or negedge rst_an)\n"
        << "begin\n"
        << "\tif (!rst_an)\n"
        << "\t\trotate_ptr[i] <= 1'b1;\n"
        << "\telse if (update_ptr)\n"
        << "\t\trotate_ptr[i] <= grant[N-1] | (|grant[i-1:0]);\n"
        << "end\n"
        << "end\n"
        << "endgenerate\n\n"
        <<

        "// mask grant generation logic\n"
        << "assign mask_req[N-1:0] = req[N-1:0] & rotate_ptr[N-1:0];\n\n"
        <<

        "assign mask_grant[0] = mask_req[0];\n"
        << "generate\n"
        << "for (i=1;i<N;i=i+1) begin : abcd\n"
        << "\tassign mask_grant[i] = (~|mask_req[i-1:0]) & mask_req[i];\n"
        << "end\n"
        << "endgenerate\n\n"
        <<

        "// non-mask grant generation logic\n"
        << "assign nomask_grant[0] = req[0];\n"
        << "generate\n"
        << "for (i=1;i<N;i=i+1) begin : abcde\n"
        << "\tassign nomask_grant[i] = (~|req[i-1:0]) & req[i];\n"
        << "end\n"
        << "endgenerate\n\n"
        <<

        "// grant generation logic\n"
        << "assign no_mask_req = ~|mask_req[N-1:0];\n"
        << "assign grant_comb[N-1:0] = mask_grant[N-1:0] | "
           "(nomask_grant[N-1:0] & {N{no_mask_req}});\n\n"
        <<

        "always @ (posedge clk or negedge rst_an)\n"
        << "begin\n"
        << "\tif (!rst_an)	grant[N-1:0] <= {N{1'b0}};\n" <<
        //    "\telse if (waitrequest)    grant[N-1:0] <= grant[N-1:0];\n" <<
        "\telse if (!waitrequest) grant[N-1:0] <= grant_comb[N-1:0] & "
        "~grant[N-1:0];\n\n" <<
        //    "\telse		grant[N-1:0] <= grant_comb[N-1:0] &
        //    ~grant[N-1:0];\n\n"
        //    <<
        "end\n\n"
        <<

        "endmodule\n\n";
    /*
        //Not working
        //gnt has a single cycle latency
        //gnt is given one cycle after req
        Out <<
        "module round_robin_arbiter (\n" <<
        "	rst_an,\n" <<
        "	clk,\n" <<
        "	req_in,\n" <<
        "	grant,\n" <<
        "	waitrequest\n" <<
        ");\n\n" <<

        "parameter N = 2;\n" <<

        "input		rst_an;\n" <<
        "input		clk;\n" <<
        "input	[N-1:0]	req_in;\n" <<
        "input waitrequest;\n" <<
        "output	[N-1:0]	grant;\n\n" <<

        "wire [N-1:0] req;\n" <<
        "wire [N-1:0] req_final;\n" <<
        "reg	[N-1:0]	req_reg;\n" <<
        "reg	[N-1:0]	rotate_ptr;\n" <<
        "wire	[N-1:0]	mask_req;\n" <<
        "wire	[N-1:0]	mask_grant;\n" <<
        "wire	[N-1:0]	grant_comb;\n" <<
        "reg	[N-1:0]	grant_reg;\n" <<
        "reg	[N-1:0]	grant;\n" <<
        "wire		no_mask_req;\n" <<
        "wire	[N-1:0] nomask_grant;\n" <<
        "wire		update_ptr;\n" <<
        "reg        waitrequest_reg;\n" <<
        "genvar i;\n\n" <<

        "always @(posedge clk)\n" <<
        "begin\n" <<
        "\tif (!waitrequest)\n" <<
        "\treq_reg[N-1:0] <= req_final[N-1:0];\n" <<
        "end\n\n" <<

        "assign req_final[N-1:0] = req_in[N-1:0] & ~req_reg[N-1:0];\n" <<
        "assign req[N-1:0] = req_final[N-1:0];\n" <<
        "// rotate pointer update logic\n" <<
        "assign update_ptr = |grant_comb[N-1:0];\n" <<
        "always @ (posedge clk)\n" <<
        "begin\n" <<
        "\tif (rst_an) begin\n" <<
        "\t\trotate_ptr[0] <= 1'b1;\n" <<
        "\t\trotate_ptr[1] <= 1'b1;\n" <<
        "\tend\n" <<
        "\t//else if (update_ptr && !waitrequest)\n" <<
        "\telse if (update_ptr)\n" <<
        "\tbegin\n" <<
        "\t\t// note: N must be at least 2\n" <<
        "\t\trotate_ptr[0] <= grant_comb[N-1];\n" <<
        "\t\trotate_ptr[1] <= grant_comb[N-1] | grant_comb[0];\n" <<
        "\tend\n" <<
        "end\n\n" <<

        "generate\n" <<
        "\tfor (i=2;i<N;i=i+1) begin : rotateptr\n" <<
        "\t\talways @ (posedge clk)\n" <<
        "\t\tbegin\n" <<
        "\t\tif (rst_an)\n" <<
        "\t\t\trotate_ptr[i] <= 1'b1;\n" <<
        "\t\t//else if (update_ptr && !waitrequest)\n" <<
        "\t\telse if (update_ptr )\n" <<
        "\t\t\trotate_ptr[i] <= grant[N-1] | (|grant[i-1:0]);\n" <<
        "\t\tend\n" <<
        "\tend\n" <<
        "endgenerate\n\n" <<

        "// mask grant generation logic\n" <<
        "assign mask_req[N-1:0] = req[N-1:0] & rotate_ptr[N-1:0];\n\n" <<

        "assign mask_grant[0] = mask_req[0];\n" <<
        "generate \n" <<
        "\tfor (i=1;i<N;i=i+1) begin : maskgrant\n" <<
        "\t\tassign mask_grant[i] = (~|mask_req[i-1:0]) & mask_req[i];\n" <<
        "\tend\n" <<
        "endgenerate\n\n" <<

        "// non-mask grant generation logic\n" <<
        "assign nomask_grant[0] = req[0];\n" <<
        "generate\n" <<
        "\tfor (i=1;i<N;i=i+1) begin : nomaskgrant\n" <<
        "\t\tassign nomask_grant[i] = (~|req[i-1:0]) & req[i];\n" <<
        "\tend\n" <<
        "endgenerate\n\n" <<

        "// grant generation logic\n" <<
        "assign no_mask_req = ~|mask_req[N-1:0];\n" <<
        "assign grant_comb[N-1:0] = (mask_grant[N-1:0] | (nomask_grant[N-1:0] &
       {N{no_mask_req}}));\n\n" <<

        "always @ (posedge clk)\n" <<
        "begin\n" <<
        "\tif (rst_an) begin\n" <<
        "\t\tgrant[N-1:0] <= {N{1'b0}};\n" <<
        "\tend\n" <<
        "\telse if (waitrequest) begin\n" <<
        "\t\tgrant[N-1:0] <= grant[N-1:0];\n" <<
        "\tend\n" <<
        "\telse begin\n" <<
        "\t\tgrant[N-1:0] <= grant_comb[N-1:0];\n" <<
        "\tend\n" <<
        "end\n" <<
        "endmodule\n";
    */

    // combination logic
    // gnt is asserted in the same cycle as req
    /*
    Out <<
    "module round_robin_arbiter (\n" <<
    "	rst_an,\n" <<
    "	clk,\n" <<
    "	req_in,\n" <<
    "	grant_comb,\n" <<
    "	waitrequest\n" <<
    ");\n" <<
    "\n" <<
    "parameter N = 2;\n" <<
    "\n" <<
    "input		rst_an;\n" <<
    "input		clk;\n" <<
    "input	[N-1:0]	req_in;\n" <<
    "input waitrequest;\n" <<
    "output	[N-1:0]	grant_comb;\n" <<
    "\n" <<
    "wire [N-1:0] req;\n" <<
    "reg	[N-1:0]	rotate_ptr;\n" <<
    "wire	[N-1:0]	mask_req;\n" <<
    "wire	[N-1:0]	mask_grant;\n" <<
    "wire	[N-1:0]	grant_comb;\n" <<
    "reg	[N-1:0]	grant_reg;\n" <<
    "reg	[N-1:0]	grant;\n" <<
    "wire		no_mask_req;\n" <<
    "wire	[N-1:0] nomask_grant;\n" <<
    "wire		update_ptr;\n" <<
    "reg        waitrequest_reg;\n" <<
    "genvar i;\n" <<
    "\n" <<

    "always @(posedge clk)\n" <<
    "begin\n" <<
    "\twaitrequest_reg <= waitrequest;\n" <<
    "\tif (waitrequest)\n" <<
    "\tbegin\n" <<
    "\t\tgrant_reg <= grant_comb;\n" <<
    "\tend\n" <<
    "end\n\n" <<

    "assign req[N-1:0] = req_in[N-1:0];\n" <<
    "// rotate pointer update logic\n" <<
    "assign update_ptr = |grant_comb[N-1:0];\n" <<
    "always @ (posedge clk)\n" <<
    "begin\n" <<
    "	if (rst_an) begin\n" <<
    "		rotate_ptr[0] <= 1'b1;\n" <<
    "		rotate_ptr[1] <= 1'b1;\n" <<
    "	end\n" <<
    "	else if (update_ptr && !waitrequest)\n" <<
    "	begin\n" <<
    "		// note: N must be at least 2\n" <<
    "		rotate_ptr[0] <= grant_comb[N-1];\n" <<
    "		rotate_ptr[1] <= grant_comb[N-1] | grant_comb[0];\n" <<
    "	end\n" <<
    "end\n" <<
    "\n" <<
    "generate\n" <<
    "	for (i=2;i<N;i=i+1) begin : rotateptr\n" <<
    "		always @ (posedge clk or negedge rst_an)\n" <<
    "		begin\n" <<
    "			if (!rst_an)\n" <<
    "				rotate_ptr[i] <= 1'b1;\n" <<
    "			else if (update_ptr && !waitrequest)\n" <<
    "				rotate_ptr[i] <= grant[N-1] | (|grant[i-1:0]);\n" <<
    "		end\n" <<
    "	end\n" <<
    "endgenerate\n" <<
    "\n" <<
    "// mask grant generation logic\n" <<
    "assign mask_req[N-1:0] = req[N-1:0] & rotate_ptr[N-1:0];\n" <<
    "\n" <<
    "assign mask_grant[0] = mask_req[0];\n" <<
    "generate \n" <<
    "	for (i=1;i<N;i=i+1) begin : maskgrant\n" <<
    "		assign mask_grant[i] = (~|mask_req[i-1:0]) & mask_req[i];\n" <<
    "	end\n" <<
    "endgenerate\n" <<
    "\n" <<
    "// non-mask grant generation logic\n" <<
    "assign nomask_grant[0] = req[0];\n" <<
    "generate\n" <<
    "	for (i=1;i<N;i=i+1) begin : nomaskgrant\n" <<
    "		assign nomask_grant[i] = (~|req[i-1:0]) & req[i];\n" <<
    "	end\n" <<
    "endgenerate\n" <<
    "\n" <<
    "// grant generation logic\n" <<
    "assign no_mask_req = ~|mask_req[N-1:0];\n" <<
    "assign grant_comb[N-1:0] = waitrequest_reg? grant_reg[N-1:0] :
    (mask_grant[N-1:0] | (nomask_grant[N-1:0] & {N{no_mask_req}}));\n" <<
    //"assign grant_comb[N-1:0] = mask_grant[N-1:0] | (nomask_grant[N-1:0] &
    {N{no_mask_req}});\n" <<
    "\n" <<
    "always @ (posedge clk or negedge rst_an)\n" <<
    "begin\n" <<
    "	if (!rst_an)	grant[N-1:0] <= {N{1'b0}};\n" <<
    "	else		grant[N-1:0] <= grant_comb[N-1:0];\n" <<
    "end\n" <<
    "endmodule\n\n";*/

    /*
    Out <<
    "module round_robin_arbiter (\n" <<
    "	rst_an,\n" <<
    "	clk,\n" <<
    "	req_in,\n" <<
    "	grant_comb,\n" <<
    "	waitrequest\n" <<
    ");\n" <<
    "\n" <<
    "parameter N = 2;\n" <<
    "\n" <<
    "input		rst_an;\n" <<
    "input		clk;\n" <<
    "input	[N-1:0]	req_in;\n" <<
    "input waitrequest;\n" <<
    "output	[N-1:0]	grant_comb;\n" <<
    "\n" <<
    "wire [N-1:0] req;\n" <<
    "reg	[N-1:0]	rotate_ptr;\n" <<
    "wire	[N-1:0]	mask_req;\n" <<
    "wire	[N-1:0]	mask_grant;\n" <<
    "wire	[N-1:0]	grant_comb;\n" <<
    "reg	[N-1:0]	grant;\n" <<
    "wire		no_mask_req;\n" <<
    "wire	[N-1:0] nomask_grant;\n" <<
    "wire		update_ptr;\n" <<
    "genvar i;\n" <<
    "\n" <<
    "assign req[N-1:0] = req_in[N-1:0];\n" <<
    "// rotate pointer update logic\n" <<
    "assign update_ptr = |grant_comb[N-1:0];\n" <<
    "always @ (posedge clk)\n" <<
    "begin\n" <<
    "	if (rst_an) begin\n" <<
    "		rotate_ptr[0] <= 1'b1;\n" <<
    "		rotate_ptr[1] <= 1'b1;\n" <<
    "	end\n" <<
    "	else if (update_ptr && !waitrequest)\n" <<
    "	begin\n" <<
    "		// note: N must be at least 2\n" <<
    "		rotate_ptr[0] <= grant_comb[N-1];\n" <<
    "		rotate_ptr[1] <= grant_comb[N-1] | grant_comb[0];\n" <<
    "	end\n" <<
    "end\n" <<
    "\n" <<
    "generate\n" <<
    "	for (i=2;i<N;i=i+1) begin : rotateptr\n" <<
    "		always @ (posedge clk or negedge rst_an)\n" <<
    "		begin\n" <<
    "			if (!rst_an)\n" <<
    "				rotate_ptr[i] <= 1'b1;\n" <<
    "			else if (update_ptr && !waitrequest)\n" <<
    "				rotate_ptr[i] <= grant[N-1] | (|grant[i-1:0]);\n" <<
    "		end\n" <<
    "	end\n" <<
    "endgenerate\n" <<
    "\n" <<
    "// mask grant generation logic\n" <<
    "assign mask_req[N-1:0] = req[N-1:0] & rotate_ptr[N-1:0];\n" <<
    "\n" <<
    "assign mask_grant[0] = mask_req[0];\n" <<
    "generate \n" <<
    "	for (i=1;i<N;i=i+1) begin : maskgrant\n" <<
    "		assign mask_grant[i] = (~|mask_req[i-1:0]) & mask_req[i];\n" <<
    "	end\n" <<
    "endgenerate\n" <<
    "\n" <<
    "// non-mask grant generation logic\n" <<
    "assign nomask_grant[0] = req[0];\n" <<
    "generate\n" <<
    "	for (i=1;i<N;i=i+1) begin : nomaskgrant\n" <<
    "		assign nomask_grant[i] = (~|req[i-1:0]) & req[i];\n" <<
    "	end\n" <<
    "endgenerate\n" <<
    "\n" <<
    "// grant generation logic\n" <<
    "assign no_mask_req = ~|mask_req[N-1:0];\n" <<
    "assign grant_comb[N-1:0] = mask_grant[N-1:0] | (nomask_grant[N-1:0] &
    {N{no_mask_req}});\n" <<
    "\n" <<
    "always @ (posedge clk or negedge rst_an)\n" <<
    "begin\n" <<
    "	if (!rst_an)	grant[N-1:0] <= {N{1'b0}};\n" <<
    "	else		grant[N-1:0] <= grant_comb[N-1:0];\n" <<
    "end\n" <<
    "endmodule\n\n";
    */
}

void VerilogWriter::printDebugModule() {

    Out << "// synthesis translate_off\n";
    Out << "module legup_memory_transaction_monitor #(\n"
        << "\tparameter DW = `MEMORY_CONTROLLER_DATA_SIZE-1,\n"
        << "\tparameter AW = `MEMORY_CONTROLLER_ADDR_SIZE-1,\n\n"
        <<

        "\tparameter NUM_READS					= 1000000,\n"
        << "\tparameter NUM_WRITES					= 1000000,\n\n"
        <<

        "\tparameter CAPTURE_TRANSACTIONS	= 1,\n\n"
        <<

        "\tparameter READ_ADDRESS_FILENAME	= \"read_addresses.dat\",\n"
        << "\tparameter READ_DATA_FILENAME		= \"read_data.dat\",\n"
        << "\tparameter WRITE_ADDRESS_FILENAME	= \"write_addresses.dat\",\n"
        << "\tparameter WRITE_DATA_FILENAME		= \"write_data.dat\"\n"
        << ") (\n"
        << "\tinput					clk,\n"
        << "\tinput			[AW: 0]	address,\n"
        << "\tinput					read,\n"
        << "\tinput					write,\n"
        << "\tinput		    [DW: 0]	writedata,\n"
        << "\tinput		    [DW: 0]	readdata,\n"
        << "\tinput				    readdatavalid,\n"
        << "\tinput				    waitrequest\n"
        << ");\n\n"
        <<

        "reg		[31: 0]	read_address_file_handle;\n"
        << "reg		[31: 0]	read_data_file_handle;\n"
        << "reg		[31: 0]	write_address_file_handle;\n"
        << "reg		[31: 0]	write_data_file_handle;\n\n"
        <<

        "reg		[31: 0]	address_reads;\n"
        << "reg		[31: 0]	data_reads;\n"
        << "reg		[31: 0]	reads_addresses	 [NUM_READS:0];\n"
        << "reg		[DW: 0]	reads_data		 [NUM_READS:0];\n"
        << "reg		[31: 0]	writes;\n"
        << "reg		[31: 0]	writes_addresses [NUM_WRITES:0];\n"
        << "reg		[DW: 0]	writes_data 	 [NUM_WRITES:0];\n\n"
        <<

        "initial\n"
        << "begin\n"
        << "\tif (CAPTURE_TRANSACTIONS == 1)\n"
        << "\tbegin\n"
        << "\t\tread_address_file_handle	= $fopen (READ_ADDRESS_FILENAME);\n"
        << "\t\tread_data_file_handle		= $fopen (READ_DATA_FILENAME);\n"
        << "\t\twrite_address_file_handle	= $fopen "
           "(WRITE_ADDRESS_FILENAME);\n"
        << "\t\twrite_data_file_handle		= $fopen (WRITE_DATA_FILENAME);\n"
        << "\tend\n"
        << "\telse\n"
        << "\tbegin\n"
        << "\t\t$readmemh(READ_ADDRESS_FILENAME,	reads_addresses);\n"
        << "\t\t$readmemh(READ_DATA_FILENAME,		reads_data);\n"
        << "\t\t$readmemh(WRITE_ADDRESS_FILENAME,	writes_addresses);\n"
        << "\t\t$readmemh(WRITE_DATA_FILENAME,		writes_data);\n"
        << "\tend\n\n"
        <<

        "\taddress_reads	<= 32'd0;\n"
        << "\tdata_reads		<= 32'd0;\n"
        << "\twrites			<= 32'd0;\n"
        << "end\n"
        << "always @(posedge clk)\n"
        << "begin\n"
        << "\tif (CAPTURE_TRANSACTIONS == 1)\n"
        << "\tbegin\n"
        << "\t\tif (read & ~waitrequest)\n"
        << "\t\tbegin\n"
        << "\t\t\t$fwrite (read_address_file_handle, \"%x\\n\", address);\n"
        << "\t\t\t$fflush (read_address_file_handle);\n"
        << "\t\t\taddress_reads <= address_reads + 1;\n"
        << "\t\tend\n"
        << "\t\tif (readdatavalid & ~waitrequest)\n"
        << "\t\tbegin\n"
        << "\t\t\t$fwrite (read_data_file_handle,    \"%x\\n\", readdata);\n"
        << "\t\t\t$fflush (read_data_file_handle);\n"
        << "\t\t\tdata_reads <= data_reads + 1;\n"
        << "\t\tend\n"
        << "\t\tif (write & ~waitrequest)\n"
        << "\t\tbegin\n"
        << "\t\t\t$fwrite (write_address_file_handle, \"%x\\n\", address);\n"
        << "\t\t\t$fwrite (write_data_file_handle,    \"%x\\n\", writedata);\n"
        << "\t\t\t$fflush (write_address_file_handle);\n"
        << "\t\t\t$fflush (write_data_file_handle);\n"
        << "\t\t\twrites <= writes + 1;\n"
        << "\t\tend\n"
        << "\tend\n"
        << "\telse\n"
        << "\tbegin\n"
        << "\t\tif (read & ~waitrequest)\n"
        << "\t\tbegin\n"
        << "\t\t\tif (reads_addresses[address_reads] != address)\n"
        << "\t\t\tbegin\n"
        << "\t\t\t\t$display (\"Read %d has an address error !\\n\", "
           "address_reads + 1);\n"
        << "\t\t\t\t$display (\"Expected transaction address: %08x\", "
           "reads_addresses[address_reads]);\n"
        << "\t\t\t\t$display (\"Actual transaction address:   %08x\\n\", "
           "address);\n"
        << "\t\t\t\t$stop;\n"
        << "\t\t\tend\n"
        << "\t\t\t	address_reads <= address_reads + 1;\n"
        << "\t\tend\n"
        << "\t\tif (readdatavalid & ~waitrequest)\n"
        << "\t\tbegin\n"
        << "\t\t\tif (reads_data[data_reads] != readdata)\n"
        << "\t\t\tbegin\n"
        << "\t\t\t\t$display (\"Read %d has an data error !\\n\", data_reads + "
           "1);\n"
        << "\t\t\t\t$display (\"Expected transaction data:    %08x\", "
           "reads_data[data_reads]);\n"
        << "\t\t\t\t$display (\"Actual transaction data:      %08x\", "
           "readdata);\n"
        << "\t\t\t\t$stop;\n"
        << "\t\t\tend\n"
        << "\t\t\tdata_reads <= data_reads + 1;\n"
        << "\t\tend\n"
        << "\t\tif (write & ~waitrequest)\n"
        << "\t\tbegin\n"
        << "\t\t\tif ((writes_addresses[writes] != address) || "
           "(writes_data[writes] != writedata))\n"
        << "\t\t\tbegin\n"
        << "\t\t\t\t$display (\"Write %d has an error!\\n\", writes + 1);\n"
        << "\t\t\t\t$display (\"Expected transaction address: %08x\", "
           "writes_addresses[writes]);\n"
        << "\t\t\t\t$display (\"Actual transaction address:   %08x\", "
           "address);\n"
        << "\t\t\t\t$display (\"Expected transaction data:    %08x\", "
           "writes_data[writes]);\n"
        << "\t\t\t\t$display (\"Actual transaction data:      %08x\\n\", "
           "writedata);\n"
        << "\t\t\t\t$stop;\n"
        << "\t\t\tend\n"
        << "\t\t\twrites <= writes + 1;\n"
        << "\t\tend\n"
        << "\tend\n"
        << "end\n"
        <<

        "endmodule\n";
    Out << "// synthesis translate_on\n\n";
}

void VerilogWriter::printDE4() {
#if 0
    Out << "module de4 (\n" <<
    "	OSC_50_BANK2,\n" <<
    "    BUTTON,\n" <<
    "    LED,\n" <<
    "	SEG0_D,\n" <<
    "	SEG1_D\n" <<
    ");\n" <<
    "    input OSC_50_BANK2;\n" <<
    "    input [1:0] BUTTON;\n" <<
    "    output [6:0] SEG0_D;\n" <<
    "    output [6:0] SEG1_D;\n" <<
    "    output [7:0] LED;\n" <<
    "\n" <<
    "    de2 de2_inst (\n" <<
    "        .CLOCK_50 (OSC_50_BANK2),\n" <<
    "        .LEDG (LED),\n" <<
    "        .KEY (BUTTON),\n" <<
    "        .SW (),\n" <<
    "        .HEX0 (SEG0_D),\n" <<
    "        .HEX1 (SEG1_D),\n" <<
    "        .HEX2 (),\n" <<
    "        .HEX3 (),\n" <<
    "        .HEX4 (),\n" <<
    "        .HEX5 (),\n" <<
    "        .HEX6 (),\n" <<
    "        .HEX7 ()\n" <<
    "    );\n" <<
    "\n" <<
    "endmodule\n";
#endif
}

bool stringInDE2PropertyList(const std::string &s) {
    return (s == "CLOCK_50" || s == "KEY" || s == "SW" || s == "HEX0" ||
            s == "HEX1" || s == "HEX2" || s == "HEX3" || s == "HEX4" ||
            s == "HEX5" || s == "HEX6" || s == "HEX7" || s == "LEDG");
}

bool VerilogWriter::currentBoardHasPin(std::string pinName) {

    char *qsfPaths = getenv("BOARD_QSF_FILE_PATHS");
    std::vector<std::string> paths = split(qsfPaths, ' ');
    ifstream verilogFile;
    std::string line;

    std::string boardQSFFileName = LEGUP_CONFIG->getFPGABoard() + ".qsf";

    for (std::vector<std::string>::iterator it = paths.begin();
         it != paths.end(); ++it) {
        if (it->find(boardQSFFileName) != std::string::npos) {
            verilogFile.open(it->c_str());
            while (getline(verilogFile, line)) {
                if (line.find(pinName) != std::string::npos) {
                    return true;
                }
            }
            break;
        }
    }
    return false;
}

void VerilogWriter::printBoardPortList() {

    PropagatingSignals *ps = alloc->getPropagatingSignals();
    std::vector<PropagatingSignal *> signals =
        ps->getPropagatingSignalsForFunctionNamed("main");
    for (std::vector<PropagatingSignal *>::iterator si = signals.begin();
         si != signals.end(); ++si) {

        PropagatingSignal *propSignal = *si;
        std::string name = propSignal->getName();
        if (!stringInDE2PropertyList(name) &&
            !(propSignal->stopsAtTopLevelModule()))
            Out << ", "
                << "\n\t\t" << name;
    }

    // Add serial connection for debugging control
    Out << ",\n\t\tUART_RXD,\n\t\tUART_TXD";
}

void VerilogWriter::printBoardSignalDeclarations() {
    PropagatingSignals *ps = alloc->getPropagatingSignals();
    std::vector<PropagatingSignal *> signals =
        ps->getPropagatingSignalsForFunctionNamed("main");
    for (std::vector<PropagatingSignal *>::iterator si = signals.begin();
         si != signals.end(); ++si) {

        PropagatingSignal *propSignal = *si;
        if (!stringInDE2PropertyList(propSignal->getName()) &&
            !(propSignal->stopsAtTopLevelModule())) {
            if (propSignal->getType() == "input") {
                Out << "\n    input ";
            } else { // implies output here
                Out << "\n    output ";
            }
            Out << propSignal->getSignal()->getWidth().str() << " "
                << propSignal->getName() << ";";
        }
    }

    // Add Serial connections for debugger control
    Out << "\n    input UART_RXD;";
    Out << "\n    output UART_TXD;";

    if (LEGUP_CONFIG->getParameterInt("MULTIPUMPING")) {
        Out << "\n    wire  clk;\n"
            << "    wire  clk2x;\n"
            << "    wire  clk1x_follower;\n"
            << "    pll pll_inst (\n"
            << "        .inclk0 (CLOCK_50),\n"
            << "        .c0 (clk),\n"
            << "        .c1 (clk2x)\n"
            << "    );\n";
    } else {
        Out << "    \n\twire clk = CLOCK_50;";
    }

    if (LEGUP_CONFIG->getParameterInt("INSPECT_DEBUG") ||
        LEGUP_CONFIG->getParameterInt("INSPECT_ONCHIP_BUG_DETECT_DEBUG")) {
        Out << "\n   reg go /* synthesis preserve */;\n"
            << "   reg [28:0] cnt = 29'b0;\n"
            << "   always @(posedge clk)\n"
            << "   begin\n"
            << "       cnt = cnt + 1;\n"
            << "       if (cnt == 29'd500000000)\n"
            << "       go = 1'b1;\n"
            << "       else\n"
            << "       go = 1'b0;\n"
            << "   end";
    } else if (alloc->getDbgInfo()->isDebugRtlEnabled()) {
        // No need to add 'go' signal - debugger will start/stop circuit
    } else {
        Out << "\n\twire go = ~KEY[1];";
    }
}

void VerilogWriter::printBoardLogic() {
    if (alloc->getDbgInfo()->isDebugRtlEnabled()) {
        // Use switches to view debugging information on the 7-segs
        Out << "\n    \n"
            << "    wire [15:0] " << DEBUG_SIGNAL_NAME_ACTIVE_INST << ";\n"
            << "    wire [15:0] " << DEBUG_SIGNAL_NAME_CURRENT_STATE << ";\n"
            << "    wire [7:0] hlsd_state;";

        Out << "\n\talways @ (*) begin\n"
            << "\t\thex7 <= 4'b0;\n"
            << "\t\thex6 <= 4'b0;\n"
            << "\t\thex5 <= 4'b0;\n"
            << "\t\thex4 <= 4'b0;\n"
            << "\t\thex3 <= 4'b0;\n"
            << "\t\thex2 <= 4'b0;\n"
            << "\t\thex1 <= 4'b0;\n"
            << "\t\thex0 <= 4'b0;\n"
            << "\t\tif (SW[0] == 1'b0) begin\n"
            << "\t\t\thex7 <= return_val_reg[31:28];\n"
            << "\t\t\thex6 <= return_val_reg[27:24];\n"
            << "\t\t\thex5 <= return_val_reg[23:20];\n"
            << "\t\t\thex4 <= return_val_reg[19:16];\n"
            << "\t\t\thex3 <= return_val_reg[15:12];\n"
            << "\t\t\thex2 <= return_val_reg[11:8];\n"
            << "\t\t\thex1 <= return_val_reg[7:4];\n"
            << "\t\t\thex0 <= return_val_reg[3:0];\n"
            << "\t\tend\n"
            << "\t\telse if (SW[0] == 1'b1) begin\n"
            << "\t\t\thex7 <= " << DEBUG_SIGNAL_NAME_ACTIVE_INST << "[11:8];\n"
            << "\t\t\thex6 <= " << DEBUG_SIGNAL_NAME_ACTIVE_INST << "[7:4];\n"
            << "\t\t\thex5 <= " << DEBUG_SIGNAL_NAME_ACTIVE_INST << "[3:0];\n"
            << "\t\t\thex4 <= " << DEBUG_SIGNAL_NAME_CURRENT_STATE
            << "[11:8];\n"
            << "\t\t\thex3 <= " << DEBUG_SIGNAL_NAME_CURRENT_STATE << "[7:4];\n"
            << "\t\t\thex2 <= " << DEBUG_SIGNAL_NAME_CURRENT_STATE << "[3:0];\n"
            << "\t\t\thex1 <= hlsd_state[7:4];\n"
            << "\t\t\thex0 <= hlsd_state[3:0];\n"
            << "\t\tend\n"
            << "\tend\n";
    } else {
        Out << "\n\talways @ (*) begin\n"
            << "\t\thex7 <= return_val_reg[31:28];\n"
            << "\t\thex6 <= return_val_reg[27:24];\n"
            << "\t\thex5 <= return_val_reg[23:20];\n"
            << "\t\thex4 <= return_val_reg[19:16];\n"
            << "\t\thex3 <= return_val_reg[15:12];\n"
            << "\t\thex2 <= return_val_reg[11:8];\n"
            << "\t\thex1 <= return_val_reg[7:4];\n"
            << "\t\thex0 <= return_val_reg[3:0];\n"
            << "\tend\n"
            << "assign UART_TXD = 1'b0;\n";
    }

    if (LEGUP_CONFIG->getParameterInt("MULTIPUMPING")) {
        Out << "\n    clock_follower clock_follower_inst (\n"
            << "        .reset ( reset ),\n"
            << "        .clk1x ( clk ),\n"
            << "        .clk2x ( clk2x ),\n"
            << "        .clk1x_follower ( clk1x_follower )\n"
            << "    );";
    }

    if (alloc->getDbgInfo()->isDebugRtlEnabled()) {
        // Debugger doesn't need a state machine start/stop circuit.
        Out << "\n\talways @(posedge clk) begin\n"
            << "\t\tif (reset)\n"
            << "\t\t\treturn_val_reg <= 0;\n"
            << "\t\telse if (finish) begin\n"
            << "\t\t\treturn_val_reg <= return_val;\n"
            << "\t\tend\n"
            << "\tend\n";
    } else {
        Out << "\n    parameter s_WAIT = 3'b001, s_START = 3'b010, s_EXE = "
               "3'b011,\n"
            << "                s_DONE = 3'b100;\n"
            << "\n"
            << "    // state registers\n"
            << "    reg [3:0] y_Q, Y_D;\n"
            << "\n"
            << "    assign LEDG[3:0] = y_Q;\n"
            << "\n"
            << "    // next state\n"
            << "    always @(*)\n"
            << "    begin\n"
            << "        case (y_Q)\n"
            << "            s_WAIT: if (go) Y_D = s_START; else Y_D = y_Q;\n"
            << "\n"
            << "            s_START: Y_D = s_EXE;\n"
            << "\n"
            << "            s_EXE: if (!finish) Y_D = s_EXE; else Y_D = "
               "s_DONE;\n"
            << "\n"
            << "            s_DONE: Y_D = s_DONE;\n"
            << "\n"
            << "            default: Y_D = 3'bxxx;\n"
            << "        endcase\n"
            << "    end\n"
            << "\n"
            << "    // current state\n"
            << "    always @(posedge clk)\n"
            << "    begin\n"
            << "        if (reset) // synchronous clear\n"
            << "            y_Q <= s_WAIT;\n"
            << "        else\n"
            << "            y_Q <= Y_D;\n"
            << "    end\n"
            << "\n"
            << "    always @(posedge clk)\n"
            << "        if (y_Q == s_EXE && finish)\n"
            << "            return_val_reg <= return_val;\n"
            << "        else if (y_Q == s_DONE)\n"
            << "            return_val_reg <= return_val_reg;\n"
            << "        else\n"
            << "            return_val_reg <= 0;\n"
            << "\n"
            << "\n"
            << "    assign start = (y_Q == s_START);";
    }
}

void VerilogWriter::printBoardTopSignals() {
    if (LEGUP_CONFIG->getParameterInt("MULTIPUMPING")) {
        Out << ",\n        .clk2x (clk2x),"
            << "\n        .clk1x_follower (clk1x_follower)";
    }
    PropagatingSignals *ps = alloc->getPropagatingSignals();
    std::vector<PropagatingSignal *> signals =
        ps->getPropagatingSignalsForFunctionNamed("main");
    for (std::vector<PropagatingSignal *>::iterator si = signals.begin();
         si != signals.end(); ++si) {
        PropagatingSignal *propSignal = *si;

        if (!(propSignal->stopsAtTopLevelModule()))
            Out << ",\n        ." << propSignal->getName() << " ( "
                << propSignal->getName() << " )";
    }

    if (alloc->getDbgInfo()->isDebugRtlEnabled()) {
        // Debugger will start circuit instead of the state machine
        Out << ",\n        .start (1'b0)";
    } else {
        Out << ",\n        .start (start)";
    }

    if (alloc->getDbgInfo()->isDebugRtlEnabled()) {
        // Add Serial signals for debugger control - and some debug signals to
        // debug the debugger :)
        Out << ",\n"
            << "        .hlsd_state(hlsd_state),\n"
            << "        ." << DEBUG_SIGNAL_NAME_ACTIVE_INST << "("
            << DEBUG_SIGNAL_NAME_ACTIVE_INST << "),\n"
            << "        ." << DEBUG_SIGNAL_NAME_CURRENT_STATE << "("
            << DEBUG_SIGNAL_NAME_CURRENT_STATE << "),\n"
            << "        .uart_tx(UART_TXD),\n"
            << "        .uart_rx(UART_RXD)";
    }
}

void VerilogWriter::printVerilogAtSpaceSeparatedPaths(char *ssPaths) {
    std::vector<std::string> paths = split(ssPaths, ' ');

    for (std::vector<std::string>::iterator it = paths.begin();
         it != paths.end(); ++it) {

        ifstream verilogFile;
        string line;
        string verilogFileName{it->c_str()};

        verilogFile.open(it->c_str());

        assert(verilogFile.is_open() &&
               "One of the paths to a static verilog file did not load "
               "properly.  Static Verilog Paths cannot have spaces in them.  "
               "Check the paths in the BOARD_TOPS and BOARD_UTILITIES "
               "environment variables to make sure none of them have spaces.");

        Out << "// Adding code from verilog file: ";
        Out << it->c_str() << '\n';

        Out << '\n';
        while (getline(verilogFile, line)) {
            bool foundToken = false;

            if (line.find("<") != std::string::npos) {
                foundToken = true;
                if (line.find("<LEGUP_PORT_LIST>") != std::string::npos) {
                    printBoardPortList();
                } else if (line.find("<LEGUP_SIGNAL_DEC>") !=
                           std::string::npos) {
                    printBoardSignalDeclarations();
                } else if (line.find("<LEGUP_LOGIC>") != std::string::npos) {
                    printBoardLogic();
                } else if (line.find("<LEGUP_TOP_SIGNALS>") !=
                           std::string::npos) {
                    printBoardTopSignals();
                } else {
                    foundToken = false;
                }
                if (foundToken) {
                    Out << '\n';
                }
            }
            if (!foundToken) {
                Out << '\n' << line;
            }
        }
        verilogFile.close();
        Out << '\n';
    }
}

void VerilogWriter::printBoardUtils() {
    char *pathToRoot = getenv("BOARD_UTILITIES");
    printVerilogAtSpaceSeparatedPaths(pathToRoot);
}

void VerilogWriter::printBoardTops() {
    char *pathToRoot = getenv("BOARD_TOPS");
    printVerilogAtSpaceSeparatedPaths(pathToRoot);

    Out << "module circuit_start_control (\n"
        << "    go,\n"
        << "    control_key\n"
        << ");\n"
        << "    input control_key;\n";
    // NC changes...
    if (LEGUP_CONFIG->getParameterInt("INSPECT_ONCHIP_BUG_DETECT_DEBUG")) {
        Out << "   output reg go /* synthesis preserve */;\n"
            << "   reg [28:0] cnt = 29'b0;\n"
            << "   always @(posedge clk)\n"
            << "   begin\n"
            << "       cnt = cnt + 1;\n"
            << "       if (cnt == 29'd500000000)\n"
            << "       go = 1'b1;\n"
            << "       else\n"
            << "       go = 1'b0;\n"
            << "   end\n";
    } else {
        Out << "    output go;\n    assign go = control_key;\n";
    }
    Out << "endmodule\n";
}

void VerilogWriter::printDE2() {
#if 0
        Out <<
      "module de2 (CLOCK_50, KEY, SW, HEX0, HEX1, HEX2, HEX3, HEX4, HEX5, HEX6, HEX7, LEDG";

    PropagatingSignals *ps = alloc->getPropagatingSignals();
    std::vector<PropagatingSignal *> signals = 
      ps->getPropagatingSignalsForFunctionNamed("main");
    for (std::vector<PropagatingSignal *>::iterator si = signals.begin(); si != signals.end(); ++si) {
      
      PropagatingSignal *propSignal = *si;
      std::string name = propSignal->getName();
      if (!stringInDE2PropertyList(name) && !(propSignal->stopsAtTopLevelModule()))
	Out << ", " << name;

    }

    if (LEGUP_CONFIG->getParameterInt("INSERT_DEBUG_CORE")) {
    	Out << ", UART_RXD, UART_TXD";
    }

    Out << ");\n";

    for (std::vector<PropagatingSignal *>::iterator si = signals.begin(); si != signals.end(); ++si) {
      
      PropagatingSignal *propSignal = *si;
      if (!stringInDE2PropertyList(propSignal->getName()) && !(propSignal->stopsAtTopLevelModule())) {
	if (propSignal->getType() == "input") {
	  Out << "    input ";
	}
	else { // implies output here
	  Out << "    output ";
	}
	Out << propSignal->getSignal()->getWidth().str() << " " << propSignal->getName() << ";\n";
      }
    }

    Out << 
    "    input CLOCK_50;\n" <<
    "    output [7:0] LEDG;\n" <<
    "    input [3:0] KEY;\n" <<
    "    input [17:0] SW;\n" <<
    "    output [6:0] HEX0, HEX1, HEX2, HEX3, HEX4, HEX5, HEX6, HEX7;\n" <<
    "\n";

    if (LEGUP_CONFIG->getParameterInt("INSERT_DEBUG_CORE")) {
    	Out << "    input UART_RXD;\n";
    	Out << "    output UART_TXD;\n";
    }

    if (LEGUP_CONFIG->getParameterInt("MULTIPUMPING")) {
        Out <<
        "    wire  clk;\n" <<
        "    wire  clk2x;\n" <<
        "    wire  clk1x_follower;\n" <<
        "    pll pll_inst (\n" <<
        "        .inclk0 (CLOCK_50),\n" <<
        "        .c0 (clk),\n" <<
        "        .c1 (clk2x)\n" <<
        "    );\n" <<
        "\n";
    } else {
        Out <<
        "    wire clk = CLOCK_50;\n";
    }
    Out <<
    "    wire reset = ~KEY[0];\n";
    //NC changes...    
    if (LEGUP_CONFIG->getParameterInt("INSPECT_DEBUG") || LEGUP_CONFIG->getParameterInt("INSPECT_ONCHIP_BUG_DETECT_DEBUG")) {
        Out << "   reg go /* synthesis preserve */;\n"<<
        "   reg [28:0] cnt = 29'b0;\n" <<
        "   always @(posedge clk)\n"<<
        "   begin\n" <<
        "       cnt = cnt + 1;\n" <<
        "       if (cnt == 29'd500000000)\n"<<
        "       go = 1'b1;\n" <<
        "       else\n" <<
        "       go = 1'b0;\n" <<                        
        "   end\n";
    } else if (LEGUP_CONFIG->getParameterInt("INSERT_DEBUG_CORE")){

    } else {
        Out << "    wire go = ~KEY[1];\n";
    }            
            
    Out << "\n" <<
    "    wire  start;\n" <<
    "    wire [31:0] return_val;\n" <<
    "    wire  finish;\n" <<
    "\n" <<
    "    reg [31:0] return_val_reg;\n" <<
    "    \n";

    if (LEGUP_CONFIG->getParameterInt("INSERT_DEBUG_CORE")) {
    	Out << "    \n"
			<< "    wire [15:0] pc_module;\n"
			<< "    wire [15:0] pc_state;\n"
			<< "    wire [7:0] hlsd_state;\n";
    }

	Out << "\treg [3:0] hex0, hex1, hex2, hex3, hex4, hex5, hex6, hex7;\n";

	// Use switches to view debugging information on the 7-segs
	if (LEGUP_CONFIG->getParameterInt("INSERT_DEBUG_CORE")) {
	Out << "\talways @ (*) begin\n"
			<< "\t\thex7 <= 4'b0;\n"
			<< "\t\thex6 <= 4'b0;\n"
			<< "\t\thex5 <= 4'b0;\n"
			<< "\t\thex4 <= 4'b0;\n"
			<< "\t\thex3 <= 4'b0;\n"
			<< "\t\thex2 <= 4'b0;\n"
			<< "\t\thex1 <= 4'b0;\n"
			<< "\t\thex0 <= 4'b0;\n"
			<< "\t\tif (SW[0] == 1'b0) begin\n"
			<< "\t\t\thex7 <= return_val_reg[31:28];\n"
			<< "\t\t\thex6 <= return_val_reg[27:24];\n"
			<< "\t\t\thex5 <= return_val_reg[23:20];\n"
			<< "\t\t\thex4 <= return_val_reg[19:16];\n"
			<< "\t\t\thex3 <= return_val_reg[15:12];\n"
			<< "\t\t\thex2 <= return_val_reg[11:8];\n"
			<< "\t\t\thex1 <= return_val_reg[7:4];\n"
			<< "\t\t\thex0 <= return_val_reg[3:0];\n"
			<< "\t\tend\n"
			<< "\t\telse if (SW[0] == 1'b1) begin\n"
			<< "\t\t\thex7 <= pc_module[7:4];\n"
			<< "\t\t\thex6 <= pc_module[3:0];\n"
			<< "\t\t\thex5 <= pc_state[7:4];\n"
			<< "\t\t\thex4 <= pc_state[3:0];\n"
			<< "\t\t\thex3 <= 0;\n"
			<< "\t\t\thex2 <= 0;\n"
			<< "\t\t\thex1 <= hlsd_state[7:4];\n"
			<< "\t\t\thex0 <= hlsd_state[3:0];\n"
			<< "\t\tend\n"
			<< "\tend\n\n";
	} else {
		Out << "\talways @ (*) begin\n"
			<< "\t\thex7 <= return_val_reg[31:28];\n"
			<< "\t\thex6 <= return_val_reg[27:24];\n"
			<< "\t\thex5 <= return_val_reg[23:20];\n"
			<< "\t\thex4 <= return_val_reg[19:16];\n"
			<< "\t\thex3 <= return_val_reg[15:12];\n"
			<< "\t\thex2 <= return_val_reg[11:8];\n"
			<< "\t\thex1 <= return_val_reg[7:4];\n"
			<< "\t\thex0 <= return_val_reg[3:0];\n"
			<< "\tend\n\n";
	}

    Out <<
    "    hex_digits h7( .x(hex7), .hex_LEDs(HEX7));\n" <<
    "    hex_digits h6( .x(hex6), .hex_LEDs(HEX6));\n" <<
    "    hex_digits h5( .x(hex5), .hex_LEDs(HEX5));\n" <<
    "    hex_digits h4( .x(hex4), .hex_LEDs(HEX4));\n" <<
    "    hex_digits h3( .x(hex3), .hex_LEDs(HEX3));\n" <<
    "    hex_digits h2( .x(hex2), .hex_LEDs(HEX2));\n" <<
    "    hex_digits h1( .x(hex1), .hex_LEDs(HEX1));\n" <<
    "    hex_digits h0( .x(hex0), .hex_LEDs(HEX0));\n" <<
    "\n";
    if (LEGUP_CONFIG->getParameterInt("MULTIPUMPING")) {
        Out <<
        "    clock_follower clock_follower_inst (\n" <<
        "        .reset ( reset ),\n" <<
        "        .clk1x ( clk ),\n" <<
        "        .clk2x ( clk2x ),\n" <<
        "        .clk1x_follower ( clk1x_follower )\n" <<
        "    );\n";
    }
    Out <<
    "\n"
    "    top top_inst (\n" <<
    "        .clk (clk),\n";
    if (LEGUP_CONFIG->getParameterInt("MULTIPUMPING")) {
        Out <<
        "        .clk2x (clk2x),\n" <<
        "        .clk1x_follower (clk1x_follower),\n";
    }
    for (std::vector<PropagatingSignal *>::iterator si = signals.begin(); si != signals.end(); ++si) {
      PropagatingSignal *propSignal = *si;

      if (!(propSignal->stopsAtTopLevelModule()))
	  Out << "        ." << propSignal->getName() << " ( " << propSignal->getName() << " ),\n";
    }
    Out <<
    "        .reset (reset),\n";

    if (LEGUP_CONFIG->getParameterInt("INSERT_DEBUG_CORE")) {
    	Out << "        .start (1'b0),\n";
    } else {
    	Out << "        .start (start),\n";
    }
    Out <<
    "        .finish (finish),\n" <<
    "        .return_val (return_val)";

    if (LEGUP_CONFIG->getParameterInt("INSERT_DEBUG_CORE")) {
    	Out << ",\n"
    		<< "        .hlsd_state(hlsd_state),\n"
			<< "        ." << DEBUG_SIGNAL_NAME_ACTIVE_INST << "(pc_module),\n"
			<< "        ." << DEBUG_SIGNAL_NAME_ACTIVE_INST << "(pc_state),\n"
			<< "        .uart_tx(UART_TXD),\n"
			<< "        .uart_rx(UART_RXD)\n";
    } else {
    	Out << "\n";
    }

    Out <<
    "    );\n" <<
    "\n";

    if (LEGUP_CONFIG->getParameterInt("INSERT_DEBUG_CORE")) {
    	Out << "\talways @(posedge clk) begin\n"
			<< "\t\tif (reset)\n"
			<< "\t\t\treturn_val_reg <= 0;\n"
			<< "\t\telse if (finish) begin\n"
			<< "\t\t\treturn_val_reg <= return_val;\n"
			<< "\t\tend\n"
			<< "\tend\n";
    } else {
		Out <<
		"    parameter s_WAIT = 3'b001, s_START = 3'b010, s_EXE = 3'b011,\n" <<
		"                s_DONE = 3'b100;\n" <<
		"\n" <<
		"    // state registers\n" <<
		"    reg [3:0] y_Q, Y_D;\n" <<
		"\n" <<
		"    assign LEDG[3:0] = y_Q;\n" <<
		"\n" <<
		"    // next state\n" <<
		"    always @(*)\n" <<
		"    begin\n" <<
		"        case (y_Q)\n" <<
		"            s_WAIT: if (go) Y_D = s_START; else Y_D = y_Q;\n" <<
		"\n" <<
		"            s_START: Y_D = s_EXE;\n" <<
		"\n" <<
		"            s_EXE: if (!finish) Y_D = s_EXE; else Y_D = s_DONE;\n" <<
		"\n" <<
		"            s_DONE: Y_D = s_DONE;\n" <<
		"\n" <<
		"            default: Y_D = 3'bxxx;\n" <<
		"        endcase\n" <<
		"    end\n" <<
		"\n" <<
		"    // current state\n" <<
		"    always @(posedge clk)\n" <<
		"    begin\n" <<
		"        if (reset) // synchronous clear\n" <<
		"            y_Q <= s_WAIT;\n" <<
		"        else\n" <<
		"            y_Q <= Y_D;\n" <<
		"    end\n" <<
		"\n" <<
		"    always @(posedge clk)\n" <<
		"        if (y_Q == s_EXE && finish)\n" <<
		"            return_val_reg <= return_val;\n" <<
		"        else if (y_Q == s_DONE)\n" <<
		"            return_val_reg <= return_val_reg;\n" <<
		"        else\n" <<
		"            return_val_reg <= 0;\n" <<
		"\n" <<
		"\n" <<
		"    assign start = (y_Q == s_START);\n";
    }

	Out <<
	"endmodule\n" <<
    "\n";

#endif
}

void VerilogWriter::printHex() {
    Out << "module hex_digits(x, hex_LEDs);\n"
        << "    input [3:0] x;\n"
        << "    output [6:0] hex_LEDs;\n"
        << "    \n"
        << "    assign hex_LEDs[0] = (~x[3] & ~x[2] & ~x[1] & x[0]) |\n"
        << "                            (~x[3] & x[2] & ~x[1] & ~x[0]) |\n"
        << "                            (x[3] & x[2] & ~x[1] & x[0]) |\n"
        << "                            (x[3] & ~x[2] & x[1] & x[0]);\n"
        << "    assign hex_LEDs[1] = (~x[3] & x[2] & ~x[1] & x[0]) |\n"
        << "                            (x[3] & x[1] & x[0]) |\n"
        << "                            (x[3] & x[2] & ~x[0]) |\n"
        << "                            (x[2] & x[1] & ~x[0]);\n"
        << "    assign hex_LEDs[2] = (x[3] & x[2] & ~x[0]) |\n"
        << "                            (x[3] & x[2] & x[1]) |\n"
        << "                            (~x[3] & ~x[2] & x[1] & ~x[0]);\n"
        << "    assign hex_LEDs[3] = (~x[3] & ~x[2] & ~x[1] & x[0]) | \n"
        << "                            (~x[3] & x[2] & ~x[1] & ~x[0]) | \n"
        << "                            (x[2] & x[1] & x[0]) | \n"
        << "                            (x[3] & ~x[2] & x[1] & ~x[0]);\n"
        << "    assign hex_LEDs[4] = (~x[3] & x[0]) |\n"
        << "                            (~x[3] & x[2] & ~x[1]) |\n"
        << "                            (~x[2] & ~x[1] & x[0]);\n"
        << "    assign hex_LEDs[5] = (~x[3] & ~x[2] & x[0]) | \n"
        << "                            (~x[3] & ~x[2] & x[1]) | \n"
        << "                            (~x[3] & x[1] & x[0]) | \n"
        << "                            (x[3] & x[2] & ~x[1] & x[0]);\n"
        << "    assign hex_LEDs[6] = (~x[3] & ~x[2] & ~x[1]) | \n"
        << "                            (x[3] & x[2] & ~x[1] & ~x[0]) | \n"
        << "                            (~x[3] & x[2] & x[1] & x[0]);\n"
        << "    \n"
        << "endmodule\n";
}

void VerilogWriter::printInferredRAMModule(bool readonly) {
    assert(!alloc->structsExistInCode() &&
           "Inferred RAMs don't support structs (no byte-enable)");

    std::string type = (readonly) ? "rom" : "ram";
    Out << "module " << type << "_dual_port\n"
        << "(\n"
        << "	clk,\n"
        << "	clken,\n"
        << "	address_a,\n"
        << "	address_b,\n";
    if (!readonly) {
        Out << "	wren_a,\n"
            << "	wren_b,\n"
            << "	data_a,\n"
            << "	data_b,\n"
            << "	byteena_a,\n"
            << "	byteena_b,\n";
    }
    Out << "	q_a,\n"
        << "	q_b\n"
        << ");\n"
        << "\n"
        << "parameter  width_a = 1'd0;\n"
        << "parameter  width_b = 1'd0;\n"
        << "parameter  widthad_a = 1'd0;\n"
        << "parameter  widthad_b = 1'd0;\n"
        << "parameter  numwords_a = 1'd0;\n"
        << "parameter  numwords_b = 1'd0;\n"
        << "parameter  init_file = \"UNUSED.mif\";\n";
    if (!readonly) {
        Out << "parameter  width_be_a = 1'd0;\n"
            << "parameter  width_be_b = 1'd0;\n";
    }
    Out << "parameter  latency = 1;\n"
        << "\n"
        << "input  clk;\n"
        << "input  clken;\n"
        << "input [(widthad_a-1):0] address_a;\n"
        << "input [(widthad_b-1):0] address_b;\n"
        << "output wire [(width_a-1):0] q_a;\n"
        << "output wire [(width_b-1):0] q_b;\n"
        << "reg [(width_a-1):0] q_a_wire;\n"
        << "reg [(width_b-1):0] q_b_wire;\n";
    if (!readonly) {
        Out << "input  wren_a;\n"
            << "input  wren_b;\n"
            << "input [(width_a-1):0] data_a;\n"
            << "input [(width_b-1):0] data_b;\n"
            << "// byte enable is unsupported by inferred RAMs\n"
            << "input [width_be_a-1:0] byteena_a;\n"
            << "input [width_be_b-1:0] byteena_b;\n";
    }
    Out << "\n" <<
        // ramstyle = \"M9K, no_rw_check\"
        "(* ramstyle = \"no_rw_check\", ram_init_file = init_file *) "
        << "reg [width_a-1:0] ram[numwords_a-1:0];\n"
        << "\n"
        << "/* synthesis translate_off */\n"
        << "integer i;\n"
        << "ALTERA_MF_MEMORY_INITIALIZATION mem ();\n"
        << "reg [8*256:1] ram_ver_file;\n"
        << "initial begin\n"
        << "	if (init_file == \"UNUSED.mif\")\n"
        << "    begin\n"
        << "		for (i = 0; i < numwords_a; i = i + 1)\n"
        << "			ram[i] = 0;\n"
        << "    end\n"
        << "	else\n"
        << "    begin\n"
        << "        // modelsim can't read .mif files directly. So use Altera "
           "function to\n"
        << "        // convert them to .ver files\n"
        << "        mem.convert_to_ver_file(init_file, width_a, "
           "ram_ver_file);\n"
        << "        $readmemh(ram_ver_file, ram);\n"
        << "    end\n"
        << "end\n"
        << "/* synthesis translate_on */\n"
        << "\n";
    if (readonly) {
        Out << "always @ (posedge clk)\n"
            << "if (clken)\n"
            << "begin\n"
            << "    q_a_wire <= ram[address_a];\n"
            << "    q_b_wire <= ram[address_b];\n"
            << "end\n";
    } else {
        Out << "always @ (posedge clk)\n"
            << "if (clken)\n"
            << "begin // Port A\n"
            << "if (wren_a)\n"
            << "begin\n"
            << "    ram[address_a] <= data_a;\n"
            << "    q_a_wire <= {width_a{1'bX}};\n" <<
            // The below line doesn't synthesize on Cyclone II if widthad_a <= 3
            //"    q_a_wire <= data_a;\n" <<
            "end\n"
            << "else\n"
            << "    q_a_wire <= ram[address_a];\n";
        if (LEGUP_CONFIG->getParameter("INFERRED_RAM_FORMAT") != "xilinx") {
            // Xilinx expects inferred dual port ram to be in a single always
            // block
            // Altera: separate always blocks
            Out << "end\n"
                << "always @ (posedge clk)\n"
                << "if (clken)\n"
                << "begin // Port b\n";
        }
        Out << "if (wren_b)\n"
            << "begin\n"
            << "    ram[address_b] <= data_b;\n"
            << "    q_b_wire <= {width_b{1'bX}};\n" <<
            // The below line doesn't synthesize on Cyclone II if widthad_a <= 3
            //"    q_b_wire <= data_b;\n" <<
            "end\n"
            << "else\n"
            << "    q_b_wire <= ram[address_b];\n"
            << "end\n";
    }

    Out << "\n" << getMemoryOutputRegisters() << "\n"
        << "\n"
        << "endmodule\n";
}

// generate the necessary output registers to achieve
// the desired memory latency
std::string VerilogWriter::getMemoryOutputRegisters() {
    return "\n\n"
           "integer j;\n"
           "reg [(width_a-1):0] q_a_reg[latency:1], q_b_reg[latency:1];\n"
           "\n"
           "always @(*)\n"
           "begin\n"
           "   q_a_reg[1] <= q_a_wire;\n"
           "   q_b_reg[1] <= q_b_wire;\n"
           "end\n"
           "\n"
           "always @(posedge clk)\n"
           "if (clken)\n"
           "begin\n"
           "   for (j = 1; j < latency; j=j+1)\n"
           "   begin\n"
           "       q_a_reg[j+1] <= q_a_reg[j];\n"
           "       q_b_reg[j+1] <= q_b_reg[j];\n"
           "   end\n"
           "end\n"
           "\n"
           "assign q_a = (clken) ? q_a_reg[latency] : 0;\n"
           "assign q_b = (clken) ? q_b_reg[latency] : 0;\n";
}

void VerilogWriter::printRAMModule() {
    if (LEGUP_CONFIG->getParameterInt("INFERRED_RAMS") &&
        !alloc->structsExistInCode()) {

        // inferred RAMs don't work when you have an uninitialized
        // array. The Quartus synthesis directives are hard to parameterize.
        // Instead just make a zero mif file called UNUSED.mif

        std::string ErrorInfo;
        std::string mifname = "UNUSED.mif";
        raw_fd_ostream File(mifname.c_str(), ErrorInfo, llvm::sys::fs::F_None);

        if (!ErrorInfo.empty()) {
            errs() << "Error: " << ErrorInfo << '\n';
            assert(0);
        }

        File << "Depth = 1;\n"
             << "Width = 64;\n"
             << "Address_radix = dec;\n"
             << "Data_radix = hex;\n"
             << "Content\n"
             << "Begin\n" <<
            // even though we are missing entries here, Quartus will zero fill
            // the rest by default
            "0: 0;\n"
             << "End;\n";

        printInferredRAMModule(/*readonly=*/false);
        printInferredRAMModule(/*readonly=*/true);
    } else {
        assert(!LEGUP_CONFIG->isXilinxBoard());
        printAltSyncRAMModule(/*readonly=*/false);
        printAltSyncRAMModule(/*readonly=*/true);
    }
}

void VerilogWriter::printAltSyncRAMModule(bool readonly) {
    std::string type = (readonly) ? "rom" : "ram";
    RTLModule *m = new RTLModule(type + "_dual_port");

    // initial all the params to specify size of the altsynrams to 0 in the ram
    m->addParam("width_a", "0");
    m->addParam("width_b", "0");
    m->addParam("widthad_a", "0");
    m->addParam("widthad_b", "0");
    m->addParam("numwords_a", "0");
    m->addParam("numwords_b", "0");
    m->addParam("latency", "1");
    m->addParam("init_file", "\"UNUSED\"");

    RTLSignal *clk = m->addIn("clk");
    RTLSignal *clken = m->addIn("clken");

    RTLSignal *addr_a = m->addIn("address_a", RTLWidth("(widthad_a-1)"));
    RTLSignal *addr_b = m->addIn("address_b", RTLWidth("(widthad_b-1)"));
    // these outputs are connected in getMemoryOutputRegisters()
    m->addOut("q_a", RTLWidth("(width_a-1)"));
    m->addOut("q_b", RTLWidth("(width_b-1)"));
    RTLSignal *q_a_wire = m->addWire("q_a_wire", RTLWidth("(width_a-1)"));
    RTLSignal *q_b_wire = m->addWire("q_b_wire", RTLWidth("(width_b-1)"));

    // Altera specific RAM
    RTLModule *a = m->addModule("altsyncram", "altsyncram_component");

    RTLConst *one = m->addConst("1");
    RTLConst *zero = m->addConst("0");
    RTLConst *blank = m->addConst("");

    // need this to avoid:
    // # ** Warning: (vsim-3016) div_const.v(111): Port type is incompatible
    // with connection (port 'clk').
    RTLSignal *clk_wire = m->addWire("clk_wire");
    clk_wire->connect(clk);

    // specify the inputs and outputs for the altsynram module
    a->addIn("clock0")->connect(clk_wire);
    a->addIn("clock1")->connect(one);
    a->addIn("clocken0")->connect(clken);
    a->addIn("clocken1")->connect(one);
    a->addIn("clocken2")->connect(one);
    a->addIn("clocken3")->connect(one);
    a->addIn("aclr0")->connect(zero);
    a->addIn("aclr1")->connect(zero);
    a->addIn("addressstall_a")->connect(zero);
    a->addIn("addressstall_b")->connect(zero);
    a->addOut("eccstatus")->connect(blank);
    a->addIn("address_a")->connect(addr_a);
    a->addIn("address_b")->connect(addr_b);
    a->addIn("rden_a")->connect(clken);
    a->addIn("rden_b")->connect(clken);
    a->addOut("q_a")->connect(q_a_wire);
    a->addOut("q_b")->connect(q_b_wire);
    RTLSignal *byteena_a = a->addIn("byteena_a");
    RTLSignal *byteena_b = a->addIn("byteena_b");

    if (readonly) {
        a->addIn("wren_a")->connect(zero);
        a->addIn("wren_b")->connect(zero);
        a->addIn("data_a")->connect(blank);
        a->addIn("data_b")->connect(blank);
        byteena_a->connect(blank);
        byteena_b->connect(blank);
    } else {
        RTLSignal *we_a = m->addIn("wren_a");
        RTLSignal *we_b = m->addIn("wren_b");
        RTLSignal *data_a = m->addIn("data_a", RTLWidth("(width_a-1)"));
        RTLSignal *data_b = m->addIn("data_b", RTLWidth("(width_b-1)"));
        a->addIn("wren_a")->connect(we_a);
        a->addIn("wren_b")->connect(we_b);
        a->addIn("data_a")->connect(data_a);
        a->addIn("data_b")->connect(data_b);
        if (alloc->usesGenericRAMs()) {
            m->addParam("width_be_a", "0");
            m->addParam("width_be_b", "0");
            RTLSignal *byteena_m_a =
                m->addIn("byteena_a", RTLWidth("width_be_a-1"));
            RTLSignal *byteena_m_b =
                m->addIn("byteena_b", RTLWidth("width_be_b-1"));
            byteena_a->connect(byteena_m_a);
            byteena_b->connect(byteena_m_b);
            a->addParam("width_byteena_a", "width_be_a");
            a->addParam("width_byteena_b", "width_be_b");
        } else {
            // if we don't use generic RAMs, byteenab will always be 1
            byteena_a->connect(one);
            byteena_b->connect(one);
            a->addParam("width_byteena_a", "1");
            a->addParam("width_byteena_b", "1");
        }
    }

    if (LEGUP_CONFIG->getParameterInt("DUAL_PORT_BINDING")) {
        a->addParam("operation_mode", "\"BIDIR_DUAL_PORT\"");
    } else {
        a->addParam("operation_mode", "\"SINGLE_PORT\"");
    }

    // can be OLD_DATA , DONT_CARE or NEW_DATA_NO_NBE_READ
    a->addParam("read_during_write_mode_mixed_ports", "\"OLD_DATA\"");
    a->addParam("init_file", "init_file");
    a->addParam("lpm_hint", "\"ENABLE_RUNTIME_MOD=NO\"");
    a->addParam("lpm_type", "\"altsyncram\"");
    a->addParam("power_up_uninitialized", "\"FALSE\"");

    // the space actually matters for simulation
    if (LEGUP_CONFIG->getDeviceFamily() == "CycloneII") {
        a->addParam("intended_device_family", "\"Cyclone II\"");
    } else if (LEGUP_CONFIG->getDeviceFamily() == "StratixIV") {
        a->addParam("intended_device_family", "\"Stratix IV\"");
    } else if (LEGUP_CONFIG->getDeviceFamily() == "CycloneIV") {
        a->addParam("intended_device_family", "\"Cyclone IV\"");
    } else if (LEGUP_CONFIG->getDeviceFamily() == "CycloneV") {
        a->addParam("intended_device_family", "\"Cyclone V\"");
    } else if (LEGUP_CONFIG->getDeviceFamily() == "StratixV") {
        a->addParam("intended_device_family", "\"Stratix V\"");
    } else {
        llvm_unreachable("Unrecognized device family!");
    }
    a->addParam("clock_enable_input_a", "\"BYPASS\"");
    a->addParam("clock_enable_input_b", "\"BYPASS\"");
    a->addParam("clock_enable_output_a", "\"BYPASS\"");
    a->addParam("clock_enable_output_b", "\"BYPASS\"");
    a->addParam("outdata_aclr_a", "\"NONE\"");
    a->addParam("outdata_aclr_b", "\"NONE\"");
    a->addParam("outdata_reg_a", "\"UNREGISTERED\"");
    a->addParam("outdata_reg_b", "\"UNREGISTERED\"");
    a->addParam("numwords_a", "numwords_a");
    a->addParam("numwords_b", "numwords_b");
    a->addParam("widthad_a", "widthad_a");
    a->addParam("widthad_b", "widthad_b");
    a->addParam("width_a", "width_a");
    a->addParam("width_b", "width_b");
    a->addParam("address_reg_b", "\"CLOCK0\"");
    a->addParam("byteena_reg_b", "\"CLOCK0\"");
    a->addParam("indata_reg_b", "\"CLOCK0\"");
    a->addParam("wrcontrol_wraddress_reg_b", "\"CLOCK0\"");

    // doesn't work on StratixII:
    //"    altsyncram_component.read_during_write_mode_port_a = "DONT_CARE","

    std::string latencyRegisters = getMemoryOutputRegisters();
    m->setBody(latencyRegisters);

    printRTL(m);
    delete m;
}

void VerilogWriter::printMemoryVariablesSignals(std::string busName,
                                                std::string inputPrefix,
                                                std::string outputPrefix,
                                                std::string postfix) {

    Out << outputPrefix << " " << busName << "_enable" << postfix << ";\n";

    Out << outputPrefix << " [`MEMORY_CONTROLLER_ADDR_SIZE-1:0] " << busName
        << "_address" << postfix << ";\n";

    Out << outputPrefix << " " << busName << "_write_enable" << postfix
        << ";\n";

    Out << outputPrefix << " [`MEMORY_CONTROLLER_DATA_SIZE-1:0] " << busName
        << "_in" << postfix << ";\n";

    if (alloc->usesGenericRAMs()) {
        Out << outputPrefix << " [1:0] " << busName << "_size" << postfix
            << ";\n";
    }
    Out << inputPrefix << " [`MEMORY_CONTROLLER_DATA_SIZE-1:0] " << busName
        << "_out" << postfix << ";\n";
    Out << "\n";
}

void VerilogWriter::printMemoryVariables(bool top) {

    std::string outputPrefix, inputPrefix;
    if (!top) {
        outputPrefix = "output reg";
        inputPrefix = "input wire";
    } else {
        outputPrefix = "wire";
        inputPrefix = "wire";
    }

    // this is in top module creating signals for connect memory controller and
    // main
    Out << inputPrefix << " "
        << "memory_controller"
        << "_waitrequest;\n";
    if (alloc->getDbgInfo()->isDebugRtlEnabled()) {
        // Debugger has multiplexed access to memory controller, so another set
        // of
        // memory controller signals are required.
        Out << inputPrefix << " "
            << "main_memory_controller"
            << "_waitrequest;\n";
    }

    printMemoryVariablesSignals("memory_controller", inputPrefix, outputPrefix,
                                "_a");
    printMemoryVariablesSignals("memory_controller", inputPrefix, outputPrefix,
                                "_b");
    if (alloc->getDbgInfo()->isDebugRtlEnabled()) {
        // Debugger has multiplexed access to memory controller, so another set
        // of
        // memory controller signals are required.
        printMemoryVariablesSignals("main_memory_controller", inputPrefix,
                                    outputPrefix, "_a");
        printMemoryVariablesSignals("main_memory_controller", inputPrefix,
                                    outputPrefix, "_b");
    }

    // if synchronization controller is needed
    // print synchronization controller signals
    if (usesSynchronization) {
        printSynchronizationControllerVariables();
    }
}

void VerilogWriter::printVerilogTestbench() {

    RTLModule *m = new RTLModule("main_tb");

    RTLSignal *clk;
    std::string clk_name;

    if (LEGUP_CONFIG->getParameterInt("MULTIPUMPING")) {
        clk_name = "clk_orig";
        clk = m->addWire("clk");
    } else {

        clk_name = "clk";
        clk = m->addReg("clk");
    }
    RTLSignal *reset = m->addReg("reset");
    RTLSignal *start = m->addReg("start");
    RTLSignal *wait = m->addReg("waitrequest");
    RTLSignal *return_val = m->addWire("return_val", RTLWidth("31"));
    RTLSignal *finish = m->addWire("finish");

    RTLSignal *pc_module;
    RTLSignal *pc_state;
    RTLSignal *uart_tx;
    RTLSignal *uart_rx;
    RTLSignal *debugger_state;
    if (alloc->getDbgInfo()->isDebugRtlEnabled()) {
        // Add debugger signals to view active instance and state
        pc_module = m->addWire(
            "pc_module", RTLWidth(alloc->getDbgInfo()->getInstanceIdBits()));
        pc_state = m->addWire("pc_state",
                              RTLWidth(alloc->getDbgInfo()->getStateBits()));
        uart_tx = m->addWire("uart_tx");
        uart_rx = m->addWire("uart_rx");
        debugger_state = m->addWire("debugger_state", RTLWidth(8));
    }

    RTLModule *t = m->addModule("top", "top_inst");
    t->addIn("clk")->connect(clk);

    if (LEGUP_CONFIG->getParameterInt("MULTIPUMPING")) {
        RTLSignal *clk_orig = m->addReg("clk_orig");
        RTLSignal *clk2x = m->addWire("clk2x");
        RTLSignal *clk1x_follower = m->addWire("clk1x_follower");

        t->addIn("clk2x")->connect(clk2x);
        t->addIn("clk1x_follower")->connect(clk1x_follower);

        // create pll
        /*pll	pll_inst (
            .inclk0 ( clk_orig ), // 50 MHz
            .c0 ( clk ), // 50 MHz
            .c1 ( clk2x ) // 100 MHz
            );
            */
        RTLModule *pll = m->addModule("pll", "pll_inst");
        pll->addIn("inclk0")->connect(clk_orig);
        // must have 1x clock coming from the same PLL as the 2x clock to avoid
        // large clock skew that can cause hold-time violations
        pll->addOut("c0")->connect(clk);
        pll->addOut("c1")->connect(clk2x);

        // create clock follower
        /*
        clock_follower clock_follower_inst (
            .reset ( reset ),
            .clk1x ( clk ),
            .clk2x ( clk2x ),
            .clk1x_follower ( clk1x_follower )
        );
        */
        RTLModule *clock_follower =
            m->addModule("clock_follower", "clock_follower_inst");
        clock_follower->addIn("reset")->connect(reset);
        clock_follower->addIn("clk1x")->connect(clk);
        clock_follower->addIn("clk2x")->connect(clk2x);
        clock_follower->addOut("clk1x_follower")->connect(clk1x_follower);
    }

    t->addIn("reset")->connect(reset);
    t->addIn("start")->connect(start);
    t->addIn("waitrequest")->connect(wait);
    t->addOut("finish")->connect(finish);
    t->addOut("return_val")->connect(return_val);

    if (alloc->getDbgInfo()->isDebugRtlEnabled()) {
        t->addOut(DEBUG_SIGNAL_NAME_ACTIVE_INST)->connect(pc_module);
        t->addOut(DEBUG_SIGNAL_NAME_CURRENT_STATE)->connect(pc_state);
        t->addOut("uart_rx")->connect(uart_rx);
        t->addOut("uart_tx")->connect(uart_tx);
        t->addOut("hlsd_state")->connect(debugger_state);
    }

    std::string old_body = m->getBody();
    raw_string_ostream body(old_body);

    body << "\n"
         << "initial \n"
         << "    " << clk_name << " = 0;\n"
         << "always @(" << clk_name << ")\n"
         << "    " << clk_name << " <= #10 ~" << clk_name << ";\n"
         << "\n"
         << "initial begin\n"
         << "//$monitor(\"At t=%t clk=%b %b %b %b %d\", $time, clk, reset, "
         << "start, finish, return_val);\n"
         << "@(negedge clk);\n"
         << "reset <= 1;\n"
         << "@(negedge clk);\n"
         << "reset <= 0;\n"
         << "start <= 1;\n"
         << "@(negedge clk);\n"
         << "start <= 0;\n"
         << "end\n"
         << "\n"
         << "always@(finish) begin\n"
         << "    if (finish == 1) begin\n"
         << "        $display(\"At t=%t clk=%b finish=%b return_val=%d\", "
         << "$time, clk, finish, return_val);\n"
         << "        $display(\"Cycles: %d\", ($time-50)/20);\n"
         << "        $finish;\n"
         << "    end\n"
         << "end\n\n";

    if (LEGUP_CONFIG->getParameterInt("TEST_WAITREQUEST")) {
        // keep the wait request high for 5 cycles, then low for 1 cycle and
        // repeat
        body << "integer counter;\n"
             << "initial begin\n"
             << "counter <= 0;\n"
             << "waitrequest <= 1;\n"
             << "@(negedge clk);\n"
             << "@(negedge clk);\n"
             << "waitrequest <= 0;\n"
             << "end\n"
             << "always @(posedge clk) begin\n"
             << "\tif (counter < 5) begin\n"
             << "\twaitrequest <= 1;\n"
             << "\tcounter <= counter + 1;\n"
             << "\tend\n"
             << "\telse\n"
             << "\tbegin\n"
             << "\twaitrequest <= 0;\n"
             << "\tcounter <= 0;\n"
             << "\tend\n"
             << "end\n\n";
    } else {
        body << "initial begin\n"
             << "waitrequest <= 1;\n"
             << "@(negedge clk);\n"
             << "@(negedge clk);\n"
             << "waitrequest <= 0;\n"
             << "end\n\n";
    }

    m->setBody(body.str());
    printRTL(m);
    delete m;
}

// we should strip the altsyncrams for non-constant global variables
// when hw/sw partitioning
bool VerilogWriter::stripRAM(RAM *R) {
    const Value *V = R->getValue();
    // if pure h/w then don't strip anything
    // if (LEGUP_CONFIG->numAccelerators() == 0) return false;
    if (!LEGUP_CONFIG->isHybridFlow())
        return false;

    if (const GlobalVariable *G = dyn_cast<GlobalVariable>(V)) {
        // don't strip constant values
        // or when it's the PCIE flow
        // or if hybrid flow is used and if it can't be found in the MIPS
        // disassembly
        // it can't be found it can MIPS disassembly if it's a global variable
        // that is only used by HW accelerators
        // in this case the linker will optimize away the global variables from
        // the SW side
        // if (!G->isConstant() && !LEGUP_CONFIG->isPCIeFlow() &&
        // searchMIPSAddress(R) != "") {
        // if (!G->isConstant() && !LEGUP_CONFIG->isPCIeFlow() &&
        // globalAddresses.find(R->getName()) == globalAddresses.end()) {
        if (!G->isConstant() && !LEGUP_CONFIG->isPCIeFlow()) {
            return true;
        }
    }
    return false;
}

void VerilogWriter::printMemCtrlModuleHeader() {
    Out << "`timescale 1 ns / 1 ns\n";
    Out << "module memory_controller"
        << "\n"
        << "("
        << "\n"
        << "\t"
        << "clk,"
        << "\n";
    // if (LEGUP_CONFIG->numAccelerators() > 0) {
    if (LEGUP_CONFIG->isHybridFlow() || LEGUP_CONFIG->isPCIeFlow()) {
        Out << "\tflag_to_sharedMemory_a,\n";
        Out << "\tflag_to_sharedMemory_b,\n";
    }
    Out << "\t"
        << "memory_controller_address_a,"
        << "\n"
        << "\t"
        << "memory_controller_address_b,"
        << "\n"
        << "\t"
        << "memory_controller_enable_a,"
        << "\n"
        << "\t"
        << "memory_controller_enable_b,"
        << "\n"
        << "\t"
        << "memory_controller_write_enable_a,"
        << "\n"
        << "\t"
        << "memory_controller_write_enable_b,"
        << "\n"
        << "\t"
        << "memory_controller_in_a,"
        << "\n"
           "\t"
        << "memory_controller_in_b,"
        << "\n";
    if (alloc->usesGenericRAMs()) {
        Out << "\t"
            << "memory_controller_size_a,"
            << "\n";
        Out << "\t"
            << "memory_controller_size_b,"
            << "\n";
    }

    Out << "\tmemory_controller_waitrequest,\n";

    // Check if we should remove the register on the memory controller
    // output. If so set the output to be the wire instead of the reg.
    if (LEGUP_CONFIG->duplicate_load_reg()) {
        Out << "\t"
            << "memory_controller_out_a,"
            << "\n"
            << "\t"
            << "memory_controller_out_b"
            << "\n"
            << ");"
            << "\n"
            << ""
            << "\n";
    } else {
        Out << "\t"
            << "memory_controller_out_reg_a,"
            << "\n"
            << "\t"
            << "memory_controller_out_reg_b"
            << "\n"
            << ");"
            << "\n"
            << ""
            << "\n";
    }
}

void VerilogWriter::printMemCtrlVariablesSignals(std::string postfix) {
    Out << "input [`MEMORY_CONTROLLER_ADDR_SIZE-1:0] memory_controller_address"
        << postfix << ";\n"
        << "input memory_controller_enable" << postfix << ";\n"
        << "input memory_controller_write_enable" << postfix << ";"
        << "\n";

    // memory_controller_out_prev is used for storing
    // the the previous output value if the memory controller
    // is not reading new values.
    if (alloc->usesGenericRAMs()) {
        Out << "input [64-1:0] memory_controller_in" << postfix << ";"
            << "\n"
            << "input [1:0] memory_controller_size" << postfix << ";"
            << "\n";

        // Check whether we should have an output register to the mem controller
        if (LEGUP_CONFIG->duplicate_load_reg()) {
            // Output is the wire not the reg now
            Out << "reg [64-1:0] memory_controller_out_reg" << postfix << ";"
                << "\n"
                << "reg [64-1:0] memory_controller_out_prev" << postfix << ";"
                << "\n"
                << "output reg [64-1:0] memory_controller_out" << postfix << ";"
                << "\n"
                << "\n";
        } else {
            Out << "output reg [64-1:0] memory_controller_out_reg" << postfix
                << ";"
                << "\n"
                << "reg [64-1:0] memory_controller_out_prev" << postfix << ";"
                << "\n"
                << "reg [64-1:0] memory_controller_out" << postfix << ";"
                << "\n"
                << "\n";
        }
    } else {
        Out << "input [`MEMORY_CONTROLLER_DATA_SIZE-1:0] memory_controller_in"
            << postfix << ";"
            << "\n"
            << "output reg [`MEMORY_CONTROLLER_DATA_SIZE-1:0] "
            << "memory_controller_out_reg" << postfix << ";"
            << "\n"
            << "reg [`MEMORY_CONTROLLER_DATA_SIZE-1:0] "
            << "memory_controller_out_prev" << postfix << ";"
            << "\n"
            << "reg [`MEMORY_CONTROLLER_DATA_SIZE-1:0] memory_controller_out"
            << postfix << ";"
            << "\n"
            << "\n";
    }

    Out << "reg memory_controller_enable_reg" << postfix << ";\n";
}

void VerilogWriter::printMemCtrlVariables() {

    int latency = getGlobalMemLatency();
    Out << "parameter latency = " << latency << ";\n\n";
    std::string ram_latency;
    if (latency == 1) {
        // special case -- we will remove the output register
        ram_latency = "latency";
    } else {
        // typical case -- memory_controller has an output register
        ram_latency = "latency-1";
    }
    Out << "parameter ram_latency = " << ram_latency << ";\n\n";

    Out << "integer j;\n";

    Out << ""
        << "\n"
        << "input clk;"
        << "\n";
    // if (LEGUP_CONFIG->numAccelerators() > 0) {
    if (LEGUP_CONFIG->isHybridFlow() || LEGUP_CONFIG->isPCIeFlow()) {
        Out << "input flag_to_sharedMemory_a;\n";
        Out << "input flag_to_sharedMemory_b;\n";
    }
    Out << "input memory_controller_waitrequest;\n";
    printMemCtrlVariablesSignals("_a");
    printMemCtrlVariablesSignals("_b");
}

void VerilogWriter::printMemCtrlRAMs() {
    for (Allocation::const_ram_iterator i = alloc->ram_begin(),
                                        e = alloc->ram_end();
         i != e; ++i) {
        RAM *R = *i;
        if (stripRAM(R))
            continue;
        if (R->getScope() == RAM::LOCAL)
            continue;
        printRamInstance(R, true);
    }
}

void VerilogWriter::printRamInstance(RAM *R, bool memCtrlInstance) {

    // there can be multiple instances of the RAM
    // if it is used by parallel threads
    unsigned numInstances = R->getNumInstances();
    for (unsigned i = 0; i < numInstances; ++i) {
        std::string name = R->getName();
        if (numInstances != 1)
            name += "_inst" + utostr(i);

        unsigned addresswidth = R->getAddrWidth();
        unsigned datawidth = R->getDataWidth();
        unsigned numwords = R->getElements();

        PhysicalRAM *phyRAM = R->getPhysicalRAM();
        if (LEGUP_CONFIG->getParameterInt("GROUP_RAMS") && phyRAM) {
            static std::set<PhysicalRAM *> visited;
            if (visited.count(phyRAM))
                return;
            visited.insert(phyRAM);

            numwords = phyRAM->getNumWords();
            name = phyRAM->getName();
            addresswidth = requiredBits(numwords - 1);
        }

        Out << "\n";

		// FIXME - TMR for global scope ram
        if (R->getScope() == RAM::GLOBAL) {
            // this is a global RAM

            Out << "reg [" << addresswidth - 1 << ":0] " << name << "_address_a"
                << ";\n";
            Out << "reg [" << addresswidth - 1 << ":0] " << name << "_address_b"
                << ";\n";
            Out << "reg " << name << "_write_enable_a"
                << ";\n";
            Out << "reg " << name << "_write_enable_b"
                << ";\n";
            Out << "reg [" << datawidth - 1 << ":0] " << name << "_in_a"
                << ";\n";
            Out << "reg [" << datawidth - 1 << ":0] " << name << "_in_b"
                << ";\n";
            Out << "wire [" << datawidth - 1 << ":0] " << name << "_out_a"
                << ";\n";
            Out << "wire [" << datawidth - 1 << ":0] " << name << "_out_b"
                << ";\n";

            if (alloc->usesGenericRAMs() && R->isStruct()) {
                // if R is Struct
                Out << "reg [8-1:0] " << name << "_byteena_a;\n";
                Out << "reg [8-1:0] " << name << "_byteena_b;\n";
            }
            Out << "\n";
        }

        std::string comment = getValueStr(R->getValue());
        limitString(comment, 200);
        Out << "// " << comment << '\n';

		std::string address_a = "_address_a";
		std::string address_b = "_address_b";
		std::string write_enable_a = "_write_enable_a";
		std::string write_enable_b = "_write_enable_b";
		std::string in_a = "_in_a";
		std::string in_b = "_in_b";
		std::string byteena_a = "_byteena_a";
		std::string byteena_b = "_byteena_b";
		std::string out_a = "_out_a";
		std::string out_b = "_out_b";
		if (LEGUP_CONFIG->getParameterInt("TMR") && !memCtrlInstance) {
			address_a      += "_v" + currReplica;
			address_b      += "_v" + currReplica;
			write_enable_a += "_v" + currReplica;
			write_enable_b += "_v" + currReplica;
			in_a           += "_v" + currReplica;
			in_b           += "_v" + currReplica;
			byteena_a      += "_v" + currReplica;
			byteena_b      += "_v" + currReplica;
			out_a          += "_r" + currReplica;
			out_b          += "_r" + currReplica;
		}

        std::string type = (R->isROM()) ? "rom" : "ram";
        Out << type << "_dual_port " << name;
		if (LEGUP_CONFIG->getParameterInt("TMR") && !memCtrlInstance)
			Out << "_r" + currReplica;
		Out << " ("
            << "\n"
            << "\t"
            << ".clk( clk ),"
            << "\n"
            << "\t"
            << ".clken( !memory_controller_waitrequest ),"
            << "\n"
            << "\t"
            << ".address_a( " << name << address_a << " ),"
            << "\n"
            << "\t"
            << ".address_b( " << name << address_b << " ),"
            << "\n";
        if (!R->isROM()) {
            Out << "\t"
                << ".wren_a( " << name << write_enable_a << " ),"
                << "\n"
                << "\t"
                << ".wren_b( " << name << write_enable_b << " ),"
                << "\n"
                << "\t"
                << ".data_a( " << name << in_a << " ),"
                << "\n"
                << "\t"
                << ".data_b( " << name << in_b << " ),"
                << "\n";
            if (alloc->usesGenericRAMs()) {
                if (R->isStruct()) { // if R is Struct
                    Out << "\t"
                        << ".byteena_a( " << name << byteena_a << " ),"
                        << "\n";
                    Out << "\t"
                        << ".byteena_b( " << name << byteena_b << " ),"
                        << "\n";
                } else {
                    Out << "\t"
                        << ".byteena_a( 1'b1 ),"
                        << "\n";
                    Out << "\t"
                        << ".byteena_b( 1'b1 ),"
                        << "\n";
                }
            }
        }
        Out << "\t"
            << ".q_a( " << name << out_a << " )"
            << ",\n"
            << "\t"
            << ".q_b( " << name << out_b << " )"
            << "\n"
            << ");\n";

        // parameters
		if (LEGUP_CONFIG->getParameterInt("TMR") && !memCtrlInstance)
			name += "_r" + currReplica;
        Out << "defparam " << name << ".width_a = " << datawidth << ";\n";
        Out << "defparam " << name << ".width_b = " << datawidth << ";\n";
        Out << "defparam " << name << ".widthad_a = " << addresswidth << ";\n";
        Out << "defparam " << name << ".widthad_b = " << addresswidth << ";\n";
        if (!R->isROM()) {
            if (R->isStruct()) {
                Out << "defparam " << name << ".width_be_a = " << 8 << ";\n";
                Out << "defparam " << name << ".width_be_b = " << 8 << ";\n";
            } else if (alloc->usesGenericRAMs()) {
                Out << "defparam " << name << ".width_be_a = " << 1 << ";\n";
                Out << "defparam " << name << ".width_be_b = " << 1 << ";\n";
            }
        }
        Out << "defparam " << name << ".numwords_a = " << numwords << ";\n";
        Out << "defparam " << name << ".numwords_b = " << numwords << ";\n";

        if (R->getScope() == RAM::GLOBAL) {
            Out << "defparam " << name << ".latency = ram_latency;\n";
        } else {
            int latency = ((RAM *)R)->getLatency(alloc);
            Out << "defparam " << name << ".latency = " << latency << ";\n";
        }

        if (R->getInitializer()) {
            Out << "defparam " << name << ".init_file = ";
            Out << "\"" << R->getMifFileName() << "\";\n";
        }
    }
}

void VerilogWriter::printMemoryControllerSignals(std::string postfix) {
    Out << "wire [`MEMORY_CONTROLLER_TAG_SIZE-1:0] tag" << postfix << ";\n";

    // As we are using the select signal to select the output data
    // pre_Tag is no longer needed here.
    Out << "assign tag" << postfix << " = "
        << "memory_controller_address" << postfix << "["
        << "`MEMORY_CONTROLLER_ADDR_SIZE-1"
        << ":"
        << "`MEMORY_CONTROLLER_ADDR_SIZE-`MEMORY_CONTROLLER_TAG_SIZE];\n";

    // detect a TAG of 0 -- this can indicate that we are trying to read a local
    // RAM from the global memory which is impossible. Use:
    //      set_memory_global <ram_name>
    // to force the local ram to global memory
    if (LEGUP_CONFIG->getParameterInt("LOCAL_RAMS")) {
        Out << "always @(*)\n"
            << "if (memory_controller_enable" << postfix << " & (tag" << postfix
            << " == 0))\n"
            << "begin\n"
            << "   $display(\"Error: Null pointer exception! "
            << "Are you trying to read from a local ram? Use: "
            << "'set_memory_global <ram_name>' To debug this error try: "
            << "'set_parameter PRINT_STATES 1'\");\n"
            << "   $finish;\n"
            << "end\n";
    }

    Out << "reg [2:0] prevAddr" << postfix << ";\n";

    if (alloc->usesGenericRAMs()) {
        Out << "reg [1:0] prevSize" << postfix << ";\n";

        // prevSize_and is used for alignment check
        Out << "reg [2:0] prevSize" << postfix << "_and;\n";
    }

    indent0 = std::string(1, '\t');
    indent = std::string(2, '\t');

    Out << "\nalways @(posedge clk)"
        << "\n";
    Out << "if (!memory_controller_waitrequest)"
        << "\n";
    Out << "begin"
        << "\n";
    Out << indent0 << "prevAddr" << postfix << " <= memory_controller_address"
        << postfix << "[2:0];\n";

    if (alloc->usesGenericRAMs()) {
        Out << indent0 << "prevSize" << postfix << " <= memory_controller_size"
            << postfix << ";\n";
    }
    Out << "end"
        << "\n";
}

void VerilogWriter::printMIFFiles() {

    if (LEGUP_CONFIG->getParameterInt("GROUP_RAMS")) {
        for (Allocation::const_phy_ram_iterator i = alloc->phy_ram_begin(),
                                                e = alloc->phy_ram_end();
             i != e; ++i) {
            PhysicalRAM *PR = *i;

            unsigned datawidth = PR->getDataWidth();

            unsigned numwords = PR->getNumWords();
            if (numwords == 0)
                continue;

            std::string name = PR->getName();

            std::string ErrorInfo;
            std::string mifname = name + ".mif";
            raw_fd_ostream File(mifname.c_str(), ErrorInfo,
                                llvm::sys::fs::F_None);

            if (!ErrorInfo.empty()) {
                errs() << "Error: " << ErrorInfo << '\n';
                assert(0);
            }

            RAM::generateMIFHeader(File, numwords, datawidth);

            for (PhysicalRAM::ram_iterator r = PR->ram_begin(),
                                           re = PR->ram_end();
                 r != re; ++r) {
                RAM *R = *r;
                assert(R->getDataWidth() == datawidth);
                R->generateMIFContent(File, R->getName());
            }
            RAM::generateMIFFooter(File);
        }
    }

    for (Allocation::const_ram_iterator i = alloc->ram_begin(),
                                        e = alloc->ram_end();
         i != e; ++i) {
        RAM *R = *i;

        if (R->getPhysicalRAM())
            continue;

        std::string ErrorInfo;
        raw_fd_ostream File(R->getMifFileName().c_str(), ErrorInfo,
                            llvm::sys::fs::F_None);

        if (!ErrorInfo.empty()) {
            errs() << "Error: " << ErrorInfo << '\n';
            assert(0);
        }

        RAM::generateMIFHeader(File, R->getElements(), R->getDataWidth());
        R->generateMIFContent(File, R->getName());
        RAM::generateMIFFooter(File);
    }
}
void VerilogWriter::printMemoryController() {

    // Created select signals for each altsynram. if the input tag matches the
    // altsynram's tag
    // the altsynram's select signal will be 1. Then we store it for 1 cycle,
    // then expand it
    // and AND with the output of altsynram. At the end of mem ctrl, there is an
    // OR gate to collect all outputs.

    if (!alloc->noSharedMemoryController()) {
        printMemCtrlModuleHeader();

        printMemCtrlVariables();
    }

    printMIFFiles();

    if (!alloc->noSharedMemoryController()) {

        if (LEGUP_CONFIG->getParameterInt("LOCAL_RAMS") &&
            alloc->isGlobalRams.empty()) {
            // assert(0);
            Out << "endmodule "
                << "\n";
            return;
        }

        printMemCtrlRAMs();

        printMemoryControllerSignals("_a");
        printMemoryControllerSignals("_b");

        printRAMSignals("_a");
        printRAMSignals("_b");

        Out << ""
            << "\n"
            << "endmodule "
            << "\n\n";
    }
}

void VerilogWriter::printRAMTag(RAM *R, std::string postfix) {
    if (stripRAM(R))
        return;

    // there can be multiple instances of the RAM
    // if it is used by parallel threads
    unsigned numInstances = R->getNumInstances();
    for (unsigned i = 0; i < numInstances; ++i) {
        std::string name = R->getName();
        if (numInstances != 1)
            name += "_inst" + utostr(i);

        unsigned datawidth = R->getDataWidth();
        unsigned addresswidth = R->getAddrWidth();

        if (LEGUP_CONFIG->getParameterInt("GROUP_RAMS")) {

            PhysicalRAM *phyRAM = R->getPhysicalRAM();
            assert(phyRAM);
            name = phyRAM->getName();
            addresswidth = requiredBits(phyRAM->getNumWords() - 1);
        }

        if (!R->isStruct()) {

            /*
           avalon_mem = memory_controller_address[5-1+2:0] + SRAM_offset;
           memory_controller_waitrequest = avalon_waitrequest;
           avalon_read = memory_controller_enable &
           ~memory_controller_write_enable;
           avalon_write = memory_controller_enable &
           memory_controller_write_enable;
           avalon_data = memory_controller_in[32-1:0];
           */

            // ignore lower bits if necessary
            int bytes = datawidth / 8;
            int ignore = (bytes == 0) ? 0 : (int)log2(bytes);

            // ALTSYNRAM1_address_a =
            // memory_controller_address_a[5-1+2:2]&{5{select_ALTSYNRAM1}};
            Out << indent0 << name << "_address" << postfix
                << " = memory_controller_address" << postfix << " ["
                << addresswidth << "-1+" << ignore << ":" << ignore << "]"
                << " & "
                << "{" << addresswidth << "{select_" << name << postfix << "}}"
                << ";\n";

            // ALTSYNRAM1_write_enable_a = memory_controller_write_enable_a &
            // select_ALTSYNRAM1;
            Out << indent0 << name << "_write_enable" << postfix
                << " = memory_controller_write_enable" << postfix
                << " & select_" << name << postfix << ";\n";

            // ALTSYNRAM1_in_a[32-1:0] = memory_controller_in_a[32-1:0] ;
            Out << indent0 << name << "_in" << postfix << " [" << datawidth
                << "-1:0] = memory_controller_in" << postfix << "[" << datawidth
                << "-1:0];\n";

            Out << "\n";

        } else {

            // all structs are 64 bits wide
            assert(datawidth == 64);

            Out << indent0 << name << "_address" << postfix
                << " = memory_controller_address" << postfix << "["
                << addresswidth + 3 << "-1:3] & "
                << "{" << addresswidth << "{select_" << name << postfix << "}}"
                << ";\n";

            Out << indent0 << name << "_write_enable" << postfix
                << " = memory_controller_write_enable" << postfix
                << " & select_" << name << postfix << ";\n";

            Out << indent << "\t\t" << name << "_in" << postfix << " = 0;\n";

            Out << indent << "case(memory_controller_size" << postfix << ")\n";

            Out << indent << "\t3:\n";
            Out << indent << "\tbegin\n";
            Out << indent << "\t\t" << name << "_in" << postfix
                << "[64-1:0] = memory_controller_in" << postfix
                << "[64-1:0];\n";
            Out << indent << "\t\t" << name << "_byteena" << postfix
                << " = 8'b11111111;\n";
            Out << indent << "\tend\n";

            Out << indent << "\t2:\n";
            Out << indent << "\tbegin\n";
            Out << indent << "\t\tif (memory_controller_address" << postfix
                << " [2:2] == 0)\n";
            Out << indent << "\t\tbegin\n";
            Out << indent << "\t\t\t" << name << "_in" << postfix
                << "[32-1:0] = memory_controller_in" << postfix
                << "[32-1:0];\n";
            Out << indent << "\t\t\t" << name << "_byteena" << postfix
                << " = 8'b00001111;\n";
            Out << indent << "\t\tend\n";
            Out << indent << "\t\telse\n";
            Out << indent << "\t\tbegin\n";
            Out << indent << "\t\t\t" << name << "_in" << postfix
                << "[64-1:32] = memory_controller_in" << postfix
                << "[32-1:0];\n";
            Out << indent << "\t\t\t" << name << "_byteena" << postfix
                << " = 8'b11110000;\n";
            Out << indent << "\t\tend\n";
            Out << indent << "\tend\n";

            Out << indent << "\t1:\n";
            Out << indent << "\tbegin\n";
            Out << indent << "\t\tcase (memory_controller_address" << postfix
                << "[2:1])\n";
            for (int c = 0; c < 4; c++) {
                Out << indent << "\t\t\t" << c << ":\n";
                Out << indent << "\t\t\tbegin\n";
                Out << indent << "\t\t\t\t" << name << "_in" << postfix << "["
                    << ((c * 16) + 16) << "-1:" << (c * 16) << "] "
                    << "= memory_controller_in" << postfix << "[16-1:0];\n";
                Out << indent << "\t\t\t\t" << name << "_byteena" << postfix
                    << " "
                    << "= 8'd" << (3 << (c * 2)) << ";\n";
                Out << indent << "\t\t\tend\n";
            }
            Out << indent << "\t\tendcase\n";
            Out << indent << "\tend\n";

            Out << indent << "\t0:\n";
            Out << indent << "\t\tcase (memory_controller_address" << postfix
                << "[2:0])\n";
            for (int i = 0; i < 8; i++) {
                Out << indent << "\t\t\t" << i << ":\n";
                Out << indent << "\t\t\tbegin\n";
                Out << indent << "\t\t\t\t" << name << "_in" << postfix << "["
                    << 8 * (i + 1) << "-1:" << 8 * i << "] = "
                    << "memory_controller_in" << postfix << "[8-1:0];\n";
                Out << indent << "\t\t\t\t" << name << "_byteena" << postfix
                    << " = "
                    << "8'd" << (1 << i) << ";\n";
                Out << indent << "\t\t\tend\n";
            }
            Out << indent << "\tendcase\n";

            Out << indent << "endcase\n";

            Out << "\n";
        }
        // debugging
        // Out << "always @(posedge clk)" << "\n";
        // Out << indent0 <<
        // "$display(\"addr:%x\",memory_controller_address);\n";
        /*
           Out << indent << "$display(\"" << name << "_address" <<
                ": %d we: %d in: %d out: %d\"," <<
           name << "_address," <<
           name << "_write_enable," <<
           name << "_in," <<
           name << "_out);\n";
           */
    }
}

void VerilogWriter::printRAMTagCase(std::string postfix) {

    Out << "always @(*)"
        << "\n";
    Out << "begin"
        << "\n";

    std::set<PhysicalRAM *> visited;
    for (Allocation::const_ram_iterator ram = alloc->ram_begin(),
                                        e = alloc->ram_end();
         ram != e; ++ram) {

        RAM *R = *ram;

        if (R->getScope() == RAM::LOCAL)
            continue;

        if (LEGUP_CONFIG->getParameterInt("GROUP_RAMS")) {
            PhysicalRAM *phyRAM = R->getPhysicalRAM();
            assert(phyRAM);
            if (visited.count(phyRAM))
                continue;
            visited.insert(phyRAM);
        }

        printRAMTag(R, postfix);
    }
    Out << "end"
        << "\n";
}

void VerilogWriter::printRAMSignalsHelper(std::string postfix, std::string name,
                                          unsigned datawidth, std::string tag,
                                          bool isStruct) {
    // wire select_ALTSYNRAM1_a;
    // assign select_ALTSYNRAM1_a = (tag_a ==`TAG_ALTSYNRAM1) ;
    Out << "wire select_" << name << postfix << ";\n";
    Out << "assign "
        << "select_" << name << postfix << " = (tag" << postfix << " == " << tag
        << ");\n";

    // reg select_ALTSYNRAM1_a;
    Out << "reg [ram_latency:0] "
        << "select_" << name << "_reg" << postfix << ";\n";

    // if the altsynram is not storing struct, forward the output with full
    // bitwidth to the
    // big OR at the end of mem ctrl. Otherwise, add more signals for selecting
    // different bytes.
    if (!isStruct) {

        // wire[31:0] memory_controller_ALTSYNRAM1_a;
        // assign memory_controller_ALTSYNRAM1_a = {32{select_ALTSYNRAM1_a}} &
        // ALTSYNRAM1_out_a;
        Out << "wire "
            << "[" << datawidth - 1 << ":0] memory_controller_" << name
            << "_out" << postfix << ";\n";

        Out << "assign "
            << "memory_controller_" << name << "_out" << postfix << " = {"
            << datawidth << "{ select_" << name << "_reg" << postfix
            << "[ram_latency]}} & " << name << "_out" << postfix << ";\n";
        Out << "\n";

    } else {

        Out << "reg "
            << "[" << datawidth - 1 << ":0] memory_controller_" << name
            << "_out" << postfix << ";\n";

        // if the altsynram is storing a struct,
        // memory_controller_ALTSYNRAM_out_struct_a
        // is added to store the full 64-bit output
        // memory_controller_ALTSYNRAM_out_a
        // then selects specific bits from
        // memory_controller_ALTSYNRAM_out_struct_a.
        Out << "wire "
            << "[" << datawidth - 1 << ":0] memory_controller_" << name
            << "_out_struct" << postfix << ";\n";

        Out << "assign "
            << "memory_controller_" << name << "_out_struct" << postfix
            << " = {" << datawidth << "{ select_" << name << "_reg" << postfix
            << "[ram_latency]}} & " << name << "_out" << postfix << ";\n";
        Out << "\n";
    }
}

void VerilogWriter::printRAMSignals(std::string postfix) {
    Out << "\n";
    if (alloc->getNumRAMs() == 0)
        return;

    // if the altsynram is not storing struct, use the signal to check addr
    // alignment
    Out << "reg "
        << "[2:0] select_not_struct" << postfix << ";\n";
    Out << "\n";

    if (LEGUP_CONFIG->getParameterInt("GROUP_RAMS")) {
        for (Allocation::const_phy_ram_iterator i = alloc->phy_ram_begin(),
                                                e = alloc->phy_ram_end();
             i != e; ++i) {
            PhysicalRAM *PR = *i;

            unsigned datawidth = PR->getDataWidth();

            if (PR->getNumWords() == 0)
                continue;

            printRAMSignalsHelper(postfix, PR->getName(), datawidth,
                                  utostr(PR->getTag()), PR->isStruct());
        }
    } else {

        for (Allocation::const_ram_iterator i = alloc->ram_begin(),
                                            e = alloc->ram_end();
             i != e; ++i) {
            RAM *R = *i;
            if (stripRAM(R))
                continue;
            unsigned datawidth = R->getDataWidth();

            if (R->getScope() == RAM::LOCAL)
                continue;

            // there can be multiple instances of the RAM
            // if it is used by parallel threads
            unsigned numInstances = R->getNumInstances();
            for (unsigned i = 0; i < numInstances; ++i) {
                std::string name = R->getName();
                std::string tag = R->getTag();
                if (numInstances != 1) {
                    name += "_inst" + utostr(i);
                    tag += "_inst" + utostr(i);
                }
                printRAMSignalsHelper(postfix, name, datawidth, "`" + tag,
                                      R->isStruct());
            }
        }
    }

    printRAMTagCase(postfix);

    // need to steer output based on previous tag
    printPrevTagCase(postfix);

    if (getGlobalMemLatency() > 1) {
        Out << "\nalways @(posedge clk)\n";
    } else {
        Out << "\nalways @(*)\n";
    }
    Out << "if (!memory_controller_waitrequest)"
        << "\n";
    Out << "begin"
        << "\n";
    Out << "memory_controller_out_reg" << postfix << " <= memory_controller_out"
        << postfix << ";\n";
    Out << "memory_controller_enable_reg" << postfix
        << " <= memory_controller_enable" << postfix << ";\n";
    Out << "end\n";

    Out << "\nalways @(posedge clk)\n";
    Out << "if (!memory_controller_waitrequest)"
        << "\n";
    Out << "for (j = 0; j < ram_latency; j=j+1)\n";
    Out << "begin\n";
    for (Allocation::const_ram_iterator i = alloc->ram_begin(),
                                        e = alloc->ram_end();
         i != e; ++i) {
        RAM *R = *i;
        if (stripRAM(R))
            continue;
        if (R->getScope() == RAM::LOCAL)
            continue;

        unsigned numInstances = R->getNumInstances();
        for (unsigned i = 0; i < numInstances; ++i) {
            std::string name = R->getName();
            if (numInstances != 1)
                name += "_inst" + utostr(i);

            Out << "select_" << name << "_reg" << postfix << "[j+1]"
                                                             " <= select_"
                << name << "_reg" << postfix << "[j];\n";
        }
    }
    Out << "end"
        << "\n";

    Out << "always @(*)\n";
    Out << "begin\n";
    for (Allocation::const_ram_iterator i = alloc->ram_begin(),
                                        e = alloc->ram_end();
         i != e; ++i) {
        RAM *R = *i;
        if (stripRAM(R))
            continue;
        if (R->getScope() == RAM::LOCAL)
            continue;
        unsigned numInstances = R->getNumInstances();
        for (unsigned i = 0; i < numInstances; ++i) {
            std::string name = R->getName();
            if (numInstances != 1)
                name += "_inst" + utostr(i);

            Out << "select_" << name << "_reg" << postfix << "[0]"
                                                             " <= select_"
                << name << postfix << ";\n";
        }
    }
    Out << "end"
        << "\n";

    // debugging
    /*
       Out << "always @(*)\n" <<
       "$display(\"memory_controller_out=%d prevTag=%d\", " <<
       "memory_controller_out, prevTag);\n";
       */
}

void VerilogWriter::printPrevTagCase(std::string postfix) {
    Out << "always @(*)\n";
    Out << "begin"
        << "\n";
    Out << indent0 << "select_not_struct" << postfix << " [2:0] = 3'b0";
    std::set<PhysicalRAM *> visited;
    for (Allocation::const_ram_iterator i = alloc->ram_begin(),
                                        e = alloc->ram_end();
         i != e; ++i) {
        RAM *R = *i;
        if (stripRAM(R))
            continue;
        if (R->isStruct())
            continue;
        unsigned datawidth = R->getDataWidth();
        std::string name = R->getName();

        if (R->getScope() == RAM::LOCAL)
            continue;

        // TODO: refactor this 'visited' lookup
        if (LEGUP_CONFIG->getParameterInt("GROUP_RAMS")) {
            PhysicalRAM *phyRAM = R->getPhysicalRAM();
            assert(phyRAM);
            if (visited.count(phyRAM))
                continue;
            visited.insert(phyRAM);

            name = phyRAM->getName();
        }

        int bytes = datawidth / 8;
        int ignore = (bytes == 0) ? 0 : (int)log2(bytes);
        if (ignore > 0) {
            // We do not support reading a single byte from an integer etc.
            // all pointers must be aligned to the word size of the ram
            // NOTE: memory_controller_address can be temporarily wrong
            // (due to being a combination signal) so we check the
            // prevAddr register instead
            unsigned numInstances = R->getNumInstances();
            for (unsigned i = 0; i < numInstances; ++i) {
                std::string instanceName = "";
                if (numInstances != 1)
                    instanceName = "_inst" + utostr(i);

                Out << " | {" << ignore << "{select_" << name + instanceName
                    << "_reg" << postfix << "[ram_latency]}}";
            }
        }
    }
    Out << ";\n";

    if (!LEGUP_CONFIG->getParameterInt("VSIM_NO_ASSERT")) {
        Out << indent0 << "if (prevAddr" << postfix
            << "[2:0] & select_not_struct" << postfix
            << "[2:0] != 0 && memory_controller_enable" << postfix << ")\n";
        Out << indent0 << "begin\n";
        Out << indent0 << "\t$display(\"Error: memory address not "
            << "aligned to ram word size!\");\n";
        Out << indent0 << "\t$finish;\n";
        Out << indent0 << "end\n\n";

        // check the mem addr alignment
        printAddrAlignmentCheck(postfix);
    }

    // if the altsynram stores struct
    std::set<PhysicalRAM *> visited2;
    for (Allocation::const_ram_iterator i = alloc->ram_begin(),
                                        e = alloc->ram_end();
         i != e; ++i) {

        RAM *R = *i;

        if (stripRAM(R))
            continue;

        if (R->getScope() == RAM::LOCAL)
            continue;

        bool isStruct = false;
        // TODO: refactor this 'visited2' lookup
        std::string name = R->getName();
        if (LEGUP_CONFIG->getParameterInt("GROUP_RAMS")) {
            PhysicalRAM *phyRAM = R->getPhysicalRAM();
            assert(phyRAM);
            if (visited2.count(phyRAM))
                continue;
            visited2.insert(phyRAM);

            name = phyRAM->getName();

            isStruct = phyRAM->isStruct();

        } else {

            isStruct = R->isStruct();
        }

        if (isStruct) {

            unsigned numInstances = R->getNumInstances();
            for (unsigned i = 0; i < numInstances; ++i) {
                std::string instanceName = "";
                if (numInstances != 1)
                    instanceName = "_inst" + utostr(i);

                printPrevAddr(R, postfix, name + instanceName);
                Out << "\n";
            }
        }
    }

    // memory_controller_out_prev_a=memory_controller_out_reg_a&&{64{!memory_controller_enable_reg}
    if (alloc->usesGenericRAMs()) {
        Out << indent0 << "memory_controller_out_prev" << postfix << " = "
            << "memory_controller_out_reg" << postfix << " & "
            << "{ 64{"
            << "!memory_controller_enable_reg" << postfix << "}};\n";
    } else {
        Out << indent0 << "memory_controller_out_prev" << postfix << " = "
            << "memory_controller_out_reg" << postfix << " & "
            << "{ `MEMORY_CONTROLLER_DATA_SIZE {"
            << "!memory_controller_enable_reg" << postfix << "}};\n";
    }

    // memory_controller_out_a =  memory_controller_main_0_array_0_a_out |
    // memory_controller_main_0_array_1_a_out
    Out << indent0 << "memory_controller_out" << postfix << " = 1'b0 ";

    if (getGlobalMemLatency() > 1) {
        Out << "| "
            << "memory_controller_out_prev"
            << postfix; // just in case there is no output
    }
    std::set<PhysicalRAM *> visited3;
    for (Allocation::const_ram_iterator i = alloc->ram_begin(),
                                        e = alloc->ram_end();
         i != e; ++i) {
        RAM *R = *i;

        if (stripRAM(R))
            continue;

        if (R->getScope() == RAM::LOCAL)
            continue;

        std::string name = R->getName();
        if (LEGUP_CONFIG->getParameterInt("GROUP_RAMS")) {
            PhysicalRAM *phyRAM = R->getPhysicalRAM();
            assert(phyRAM);
            if (visited3.count(phyRAM))
                continue;
            visited3.insert(phyRAM);

            name = phyRAM->getName();
        }

        unsigned numInstances = R->getNumInstances();
        for (unsigned i = 0; i < numInstances; ++i) {
            std::string instanceName = "";
            if (numInstances != 1)
                instanceName = "_inst" + utostr(i);

            Out << " | memory_controller_" << name + instanceName << "_out"
                << postfix;
        }
    }
    Out << ";\n";

    Out << "end"
        << "\n";
}

// prevAddr is the bottom 3 bits of the previous address
void VerilogWriter::printPrevAddr(RAM *R, std::string postfix,
                                  std::string name) {
    Out << indent0 << "memory_controller_" << name << "_out" << postfix
        << " = 0;\n";
    Out << indent0 << "case(prevAddr" << postfix << ")\n";

    for (int i = 0; i < 8; i++) {
        int dataSize = 0;
        int prevRamOutLo = 0;
        int prevRamOutHi = 0;
        switch (i) {
        case 0:
            // long long
            dataSize = 64;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            // int
            dataSize = 32;
            break;
        case 5:
        case 6:
            // short
            dataSize = 16;
            break;
        case 7:
            // byte
            dataSize = 8;
            break;
        default:
            llvm_unreachable(0);
        }

        prevRamOutLo = i * 8;
        prevRamOutHi = i * 8 + dataSize;

        Out << indent0 << "\t" << i << ": "
            << "memory_controller_" << name << "_out" << postfix << "["
            << dataSize << "-1:0]"
            << " = memory_controller_" << name << "_out_struct" << postfix
            << "[" << prevRamOutHi << "-1:" << prevRamOutLo << "];\n";
    }
    Out << indent0 << "endcase\n";
}

void VerilogWriter::printAddrAlignmentCheck(std::string postfix) {

    // if( prevSize==0 ) prevSize_and = 3'b000
    // if( prevSize==1 ) prevSize_and = 3'b001
    // if( prevSize==2 ) prevSize_and = 3'b011
    // if( prevSize==3 ) prevSize_and = 3'b111
    // prevSize AND with last 3 bits of prevAddr to check the aligment
    Out << indent0 << "prevSize" << postfix << "_and[0] = prevSize" << postfix
        << "[1] | prevSize" << postfix << "[0];\n" << indent0 << "prevSize"
        << postfix << "_and[1] = prevSize" << postfix << "[1];\n" << indent0
        << "prevSize" << postfix << "_and[2] = prevSize" << postfix
        << "[1] & prevSize" << postfix << "[0];\n";

    Out << indent0 << "if ((prevAddr" << postfix << " & prevSize" << postfix
        << "_and) != 0 && memory_controller_enable" << postfix << ")\n";
    Out << indent0 << "begin\n";
    Out << indent0 << "\t$display(\"Error: "
        << "memory address not aligned to ram word size!\");\n";
    Out << indent0 << "\t$finish;\n";
    Out << indent0 << "end\n";
}

void VerilogWriter::printSynchronizationControllerVariables() {

    // declare signals needed for synchronization controller
    printSynchronizationControllerVariableDeclarations();

    // print flag to indicate access synchronization cores
    Out << "assign flag_to_synchronizer_a = memory_controller_address_a[31];\n"
        << "assign flag_to_synchronizer_b = "
           "memory_controller_address_b[31];\n\n";

    // print shift registers for synchronization cores
    // when the MSB is 1, it means that 2 cycles have passed
    // and that the read from synchronization controller needs to be used
    printSynchronizationControllerShiftReg("_a");
    printSynchronizationControllerShiftReg("_b");

    // mux logic for synchronization controller signals
    // assigns synchronization controller signals and memory controller signals
    // for each port
    printSynchronizationControllerMuxLogic();
}

void VerilogWriter::printSynchronizationControllerMuxLogic() {

    Out << "assign synchronization_controller_address = "
           "flag_to_synchronizer_a? memory_controller_address_a : "
           "memory_controller_address_b;\n"
        << "assign synchronization_controller_enable = flag_to_synchronizer_a "
           "& enable_a & !write_enable_a | flag_to_synchronizer_b & enable_b & "
           "!write_enable_b;\n"
        << "assign synchronization_controller_write_enable = "
           "flag_to_synchronizer_a & enable_a & write_enable_a | "
           "flag_to_synchronizer_b & enable_b & write_enable_b;\n"
        << "assign synchronization_controller_in = flag_to_synchronizer_a? "
           "memory_controller_in_a : memory_controller_in_b;\n"
        << "assign data_from_memory_a = sync_controller_shiftreg_a[1]? "
           "synchronization_controller_out: memory_controller_out_a;\n"
        << "assign data_from_memory_b = sync_controller_shiftreg_b[1]? "
           "synchronization_controller_out: memory_controller_out_b;\n"
        << "assign memory_controller_enable_a = enable_a & "
           "!flag_to_synchronizer_a;\n"
        << "assign memory_controller_enable_b = enable_b & "
           "!flag_to_synchronizer_b;\n"
        << "assign memory_controller_write_enable_a = write_enable_a & "
           "!flag_to_synchronizer_a;\n"
        << "assign memory_controller_write_enable_b = write_enable_b & "
           "!flag_to_synchronizer_b;\n\n";
}

void
VerilogWriter::printSynchronizationControllerShiftReg(std::string postfix) {

    Out << "always @(posedge clk)\n"
        << "begin\n"
        << "    if (!memory_controller_waitrequest)\n"
        << "    begin\n"
        << "        sync_controller_shiftreg" << postfix
        << " <= sync_controller_shiftreg" << postfix << " << 1;\n"
        << "        if (flag_to_synchronizer" << postfix << " & enable"
        << postfix << " & !write_enable" << postfix << ")\n"
        << "            sync_controller_shiftreg" << postfix << "[0] <= 1'b1;\n"
        << "        else\n"
        << "            sync_controller_shiftreg" << postfix << "[0] <= 1'b0;\n"
        << "    end\n"
        << "end\n\n";
}

void VerilogWriter::printSynchronizationControllerVariableDeclarations() {
    Out << "reg [1:0] sync_controller_shiftreg_a;\n"
        << "reg [1:0] sync_controller_shiftreg_b;\n"
        << "wire [`MEMORY_CONTROLLER_DATA_SIZE-1:0] data_from_memory_a;\n"
        << "wire [`MEMORY_CONTROLLER_DATA_SIZE-1:0] data_from_memory_b;\n"
        << "wire enable_a;\n"
        << "wire enable_b;\n"
        << "wire write_enable_a;\n"
        << "wire write_enable_b;\n"
        <<

        "wire [`MEMORY_CONTROLLER_ADDR_SIZE-1:0] "
        "synchronization_controller_address;\n"
        << "wire synchronization_controller_enable;\n"
        << "wire synchronization_controller_write_enable;\n"
        << "wire [`MEMORY_CONTROLLER_DATA_SIZE-1:0] "
           "synchronization_controller_in;\n"
        << "wire [`MEMORY_CONTROLLER_DATA_SIZE-1:0] "
           "synchronization_controller_out;\n\n";
}

void VerilogWriter::printSynchronizationControllerInstance() {

    Out << "synchronization_controller synchronization_controller_inst(\n"
        << "\t.clk( clk ),\n"
        << "\t.reset( reset ),\n"
        << "\t.synchronization_controller_address( "
           "synchronization_controller_address ),\n"
        << "\t.synchronization_controller_enable( "
           "synchronization_controller_enable ),\n"
        << "\t.synchronization_controller_write_enable( "
           "synchronization_controller_write_enable ),\n"
        << "\t.synchronization_controller_in( synchronization_controller_in "
           "),\n";

    // Check if we should remove the register on the synchronization controller
    // output. If so set the output to be the wire instead of the reg.
    if (LEGUP_CONFIG->duplicate_load_reg()) {
        Out << "\t.synchronization_controller_out( "
               "synchronization_controller_out )\n"
            << ");\n\n";
    } else {
        Out << "\t"
            << ".synchronization_controller_out_reg_reg( "
               "synchronization_controller_out )\n"
            << ");\n\n";
    }
}

void VerilogWriter::printSynchronizationController() {

    // print module declaration
    printSyncCtrlModuleHeader();

    // print module signals
    printSyncCtrlVariables();

    // print wires for synchronization cores
    printSyncCtrlCoresVariables();

    // print steering logic
    printSyncCtrlSignals();

    // print each of the mutex/barriers instantiations
    printSyncCtrlCoreInstances();

    Out << "endmodule "
        << "\n\n";
}

void VerilogWriter::printSyncCtrlModuleHeader() {
    Out << "`timescale 1 ns / 1 ns\n";
    Out << "module synchronization_controller"
        << "\n"
        << "("
        << "\n"
        << "\t"
        << "clk,"
        << "\n"
        << "\t"
        << "reset,"
        << "\n"
        << "\t"
        << "synchronization_controller_address,"
        << "\n"
        << "\t"
        << "synchronization_controller_enable,"
        << "\n"
        << "\t"
        << "synchronization_controller_write_enable,"
        << "\n"
        << "\t"
        << "synchronization_controller_in,"
        << "\n";

    // Check if we should remove the register on the synchronization controller
    // output. If so set the output to be the wire instead of the reg.
    if (LEGUP_CONFIG->duplicate_load_reg()) {
        Out << "\t"
            << "synchronization_controller_out"
            << "\n"
            << ");\n\n";
    } else {
        Out << "\t"
            << "synchronization_controller_out_reg_reg"
            << "\n"
            << ");\n\n";
    }
}

void VerilogWriter::printSyncCtrlVariables() {
    Out << "input clk;\n";
    Out << "input reset;\n";

    Out << "input [`MEMORY_CONTROLLER_ADDR_SIZE-1:0] "
           "synchronization_controller_address;\n"
        << "input synchronization_controller_enable;\n"
        << "input synchronization_controller_write_enable;\n";

    // synchronization_controller_out_prev is used for storing
    // the the previous output value if the synchronization controller
    // is not reading new values.
    Out << "input [`MEMORY_CONTROLLER_DATA_SIZE-1:0] "
           "synchronization_controller_in;\n";

    // Check whether we should have an output register to the mem controller
    if (LEGUP_CONFIG->duplicate_load_reg()) {
        // Output is the wire not the reg now
        Out << "reg [`MEMORY_CONTROLLER_DATA_SIZE-1:0] "
               "synchronization_controller_out_reg;\n"
            << "output reg [`MEMORY_CONTROLLER_DATA_SIZE-1:0] "
               "synchronization_controller_out;\n\n"
            << "reg [`MEMORY_CONTROLLER_DATA_SIZE-1:0] "
               "synchronization_controller_out_reg_reg;\n\n";
    } else {
        Out << "reg [`MEMORY_CONTROLLER_DATA_SIZE-1:0] "
               "synchronization_controller_out_reg;\n"
            << "output reg [`MEMORY_CONTROLLER_DATA_SIZE-1:0] "
               "synchronization_controller_out_reg_reg;\n"
            << "reg [`MEMORY_CONTROLLER_DATA_SIZE-1:0] "
               "synchronization_controller_out;\n\n";
    }
}

// print steering logic for synchronization cores
void VerilogWriter::printSyncCtrlSignals() {

    bool lockUsed = syncMap.find("lock") != syncMap.end();
    bool barrierUsed = syncMap.find("barrier") != syncMap.end();

    if (lockUsed) {
        Out << "wire [6:0] tag_lock;\n";
        Out << "assign tag_lock = "
            << "synchronization_controller_address[11:5];\n";

        Out << "wire lock_access;\n";
        Out << "assign lock_access = "
            << "synchronization_controller_address[18:12] == 0;\n";
    }

    if (barrierUsed) {
        Out << "wire [6:0] tag_barrier;\n";
        Out << "assign tag_barrier = "
            << "synchronization_controller_address[18:12];\n";

        Out << "wire barrier_access;\n";
        Out << "assign barrier_access = "
            << "synchronization_controller_address[11:5] == 0;\n";
    }

    Out << "wire synchronization_status_address;\n";
    Out << "assign synchronization_status_address = "
        << "synchronization_controller_address[4];\n\n";

    int index;
    std::string instanceName, syncType;
    for (std::map<std::string, int>::iterator it = syncMap.begin();
         it != syncMap.end(); it++) {
        syncType = (*it).first;
        index = (*it).second;
        for (int i = 0; i < index; i++) {
            instanceName = syncType + "_inst" + utostr(i);
            // wire select_lock;
            // assign select_lock = (tag_lock == 0) ;
            Out << "wire select_" << instanceName << ";\n";
            Out << "assign "
                << "select_" << instanceName << " = (tag_" << syncType
                << " == ";
            if (syncType == "lock") {
                Out << utostr(i) << ")";
            } else if (syncType == "barrier") {
                // for barriers, the tag starts at 1
                Out << utostr(i + 1) << ")";
            }
            // make sure it's not accessing another type of
            // synchronization core
            Out << " & " << syncType << "_access;\n";

            // wire synchronization_controller_syncCore_out;
            // assign synchronization_controller_syncCore_out = select_syncCore
            // & syncCore_out_a;

            if (syncType == "barrier") {
                Out << "wire [`MEMORY_CONTROLLER_DATA_SIZE-1:0] "
                       "synchronization_controller_" << instanceName
                    << "_out;\n";

                Out << "assign "
                    << "synchronization_controller_" << instanceName
                    << "_out = {`MEMORY_CONTROLLER_DATA_SIZE{select_"
                    << instanceName << "}} & " << instanceName << "_out;\n";
            } else {
                Out << "wire synchronization_controller_" << instanceName
                    << "_out;\n";

                Out << "assign "
                    << "synchronization_controller_" << instanceName
                    << "_out = select_" << instanceName << " & " << instanceName
                    << "_out;\n";
            }
            Out << "\n";
        }
    }

    for (std::map<std::string, int>::iterator it = syncMap.begin();
         it != syncMap.end(); it++) {
        syncType = (*it).first;
        printSyncTagCase(syncType);
    }

    Out << "always @(*)\n";
    Out << "begin"
        << "\n";
    Out << "\tsynchronization_controller_out = 1'b0";
    // synchronization_controller_out_a =  synchronization_controller_Core1_out
    // | synchronization_controller_Core2_out;
    for (std::map<std::string, int>::iterator it = syncMap.begin();
         it != syncMap.end(); it++) {
        syncType = (*it).first;
        index = (*it).second;
        for (int i = 0; i < index; i++) {
            instanceName = syncType + "_inst" + utostr(i);
            Out << " | synchronization_controller_" << instanceName << "_out";
        }
    }
    Out << ";\n";
    Out << "end"
        << "\n\n";

    // register the output twice to match the 2 cycle memory access latency
    Out << "always @(posedge clk)\n";
    Out << "begin"
        << "\n";
    Out << "\tsynchronization_controller_out_reg <= "
           "synchronization_controller_out;\n";
    Out << "\tsynchronization_controller_out_reg_reg <= "
           "synchronization_controller_out_reg;\n";
    Out << "end"
        << "\n\n";
}

void VerilogWriter::printSyncTag(const std::string syncType, const int index) {

    std::string instanceName = syncType + "_inst" + utostr(index);
    // syncCore_address = synchronization_controller_address & select_syncCore;
    Out << "\t" << instanceName
        << "_address = synchronization_status_address & "
        << "select_" << instanceName << ";\n";

    // syncCore_enable = synchronization_controller_enable & select_syncCore;
    Out << "\t" << instanceName << "_enable"
        << " = synchronization_controller_enable & "
        << "select_" << instanceName << ";\n";

    // syncCore_write_enable = synchronization_controller_write_enable &
    // select_syncCore;
    Out << "\t" << instanceName << "_write_enable"
        << " = synchronization_controller_write_enable & "
        << "select_" << instanceName << ";\n";

    // syncCore_in_a = synchronization_controller_in;
    Out << "\t" << instanceName << "_in"
        << " = synchronization_controller_in;\n";
}

void VerilogWriter::printSyncTagCase(std::string syncType) {

    Out << "always @(*)"
        << "\n";
    Out << "begin"
        << "\n";

    int index = syncMap[syncType];
    for (int i = 0; i < index; i++) {
        printSyncTag(syncType, i);
    }

    Out << "end\n\n";
}

// print wires needed for each synchronization core
void VerilogWriter::printSyncCtrlCoresVariables() {

    std::string instanceName, syncType;
    int index;
    // create wires for each lock core
    for (std::map<std::string, int>::iterator it = syncMap.begin();
         it != syncMap.end(); it++) {
        syncType = (*it).first;
        index = (*it).second;
        for (int i = 0; i < index; i++) {
            instanceName = syncType + "_inst" + utostr(i);
            Out << "reg " << instanceName << "_address;\n";
            Out << "reg " << instanceName << "_enable;\n";
            Out << "reg " << instanceName << "_write_enable;\n";
            if (syncType == "barrier") {
                Out << "reg [`MEMORY_CONTROLLER_DATA_SIZE-1:0] " << instanceName
                    << "_in;\n";
                Out << "wire [`MEMORY_CONTROLLER_DATA_SIZE-1:0] "
                    << instanceName << "_out;\n";
            } else {
                Out << "reg " << instanceName << "_in;\n";
                Out << "wire " << instanceName << "_out;\n";
            }
        }
    }
}

// instantiate each synchronization core
void VerilogWriter::printSyncCtrlCoreInstances() {

    // instantiate lock modules
    int index = 0;
    std::string syncType;
    // for (std::set<int>::iterator it = syncMap["lock"].begin(); it !=
    // syncMap["lock"].end(); it++) {
    for (std::map<std::string, int>::iterator it = syncMap.begin();
         it != syncMap.end(); it++) {
        syncType = (*it).first;
        index = (*it).second;
        for (int i = 0; i < index; i++) {
            if (syncType == "lock")
                printLockInstance(i);
            else if (syncType == "barrier")
                printBarrierInstance(i);
        }
    }
}

void VerilogWriter::printTop(const Function *F) {

    for (Allocation::const_define_iterator i = alloc->define_begin(),
                                           e = alloc->define_end();
         i != e; ++i) {
        const std::string name = i->first;
        const std::string value = i->second;
        std::string comment = alloc->getDefineComment(name);
        if (!comment.empty()) {
            limitString(comment, 200);
            Out << "// " << comment << "\n";
        }

        Out << "`define " << name << " " << value << "\n";
    }

    // print top for hybrid flow
    if (LEGUP_CONFIG->isHybridFlow() || LEGUP_CONFIG->isPCIeFlow()) {
        printTopHybrid(F, alloc->getDataSize());
        return;
    }

    Out << "\n";
    Out << "// Turn off warning 'ignoring unsupported system task'\n";
    Out << "// altera message_off 10175\n";
    Out << "\n";

    Out << "`timescale 1 ns / 1 ns\n";
    Out << "module top\n"
        << "\t(\n"
        << "\t\tclk,\n";

    PropagatingSignals *ps = alloc->getPropagatingSignals();
    std::vector<PropagatingSignal *> signals =
        ps->getPropagatingSignalsForFunctionNamed("main");
    for (std::vector<PropagatingSignal *>::iterator si = signals.begin();
         si != signals.end(); ++si) {

        PropagatingSignal *propSignal = *si;
        if (!(propSignal->stopsAtTopLevelModule()))
            Out << "\t\t" << propSignal->getName() << ",\n";
    }

    if (LEGUP_CONFIG->getParameterInt("MULTIPUMPING")) {
        Out << "\t\tclk2x,\n"
            << "\t\tclk1x_follower,\n";
    }
    Out << "\t\treset,\n"
        << "\t\tstart,\n"
        << "\t\tfinish,\n"
        << "\t\twaitrequest,\n"
        << "\t\treturn_val";

    if (alloc->getDbgInfo()->isDebugRtlEnabled()) {
        // Add serial connections for debugger control, plus some signals to
        // debug the debugger.
        Out << ",\n"
            << "\t\tuart_rx,\n"
            << "\t\tuart_tx,\n"
            << "\t\t" << DEBUG_SIGNAL_NAME_ACTIVE_INST << ",\n"
            << "\t\t" << DEBUG_SIGNAL_NAME_CURRENT_STATE << ",\n"
            << "\t\thlsd_state";
    }

    Out << "\n"
        << "\t);\n";

    Out << "input clk;\n";
    if (LEGUP_CONFIG->getParameterInt("MULTIPUMPING")) {
        Out << "input clk2x;\n";
        Out << "input clk1x_follower;\n";
    }

    for (std::vector<PropagatingSignal *>::iterator si = signals.begin();
         si != signals.end(); ++si) {

        PropagatingSignal *propSignal = *si;
        if (propSignal->stopsAtTopLevelModule())
            continue;

        if (propSignal->getType() == "input") {
            Out << "input ";
        } else { // implies output here
            Out << "output ";
        }

        Out << propSignal->getSignal()->getWidth().str() << " "
            << propSignal->getName() << ";\n";
    }

    Out << "input reset;\n";
    Out << "input start;\n";

    // NC changes...
    if (LEGUP_CONFIG->getParameterInt("INSPECT_DEBUG") ||
        LEGUP_CONFIG->getParameterInt("INSPECT_ONCHIP_BUG_DETECT_DEBUG")) {
        Out << "output wire finish /* synthesis keep */;\n";
    } else {
        Out << "output wire finish;\n";
    }

    Out << "input waitrequest;\n";
    Out << "output wire [31:0] return_val;\n";

    if (alloc->getDbgInfo()->isDebugRtlEnabled()) {
        // Signals for active instance and current state
        Out << "input uart_rx;\n"
            << "output uart_tx;\n"
            << "output [" << (alloc->getDbgInfo()->getInstanceIdBits() - 1)
            << ":0] " << DEBUG_SIGNAL_NAME_ACTIVE_INST << ";\n"
            << "reg [" << (alloc->getDbgInfo()->getInstanceIdBits() - 1)
            << ":0] " << DEBUG_SIGNAL_NAME_ACTIVE_INST << "_r;\n"
            << "output [" << (alloc->getDbgInfo()->getStateBits() - 1) << ":0] "
            << DEBUG_SIGNAL_NAME_CURRENT_STATE << ";\n"
            << "reg [" << (alloc->getDbgInfo()->getStateBits() - 1) << ":0] "
            << DEBUG_SIGNAL_NAME_CURRENT_STATE << "_r;\n"
            << "output [7:0] hlsd_state;\n"
            << "\n";
    }

    if (!alloc->noSharedMemoryController()) {
        printMemoryVariables(true);
    }

    // NC changes...
    if (LEGUP_CONFIG->getParameterInt("INSPECT_DEBUG") ||
        LEGUP_CONFIG->getParameterInt("INSPECT_ONCHIP_BUG_DETECT_DEBUG")) {
        Out << "wire [32:0] cnt /* synthesis syn_noprune */";
        Out << ";\n";
        Out << "reg [32:0] counter /* synthesis syn_noprune */;\n";
        Out << "reg [1:0] start_cnt;\n";
        Out << "initial begin\n";
        Out << "counter <= 32'b0;\n";
        Out << "start_cnt <= 1'b0;\n";
        Out << "end\n";
        Out << "always @ (posedge clk )\n";
        Out << "begin\n";
        Out << "if (start)\n";
        Out << "start_cnt = 1'b1;\n";
        Out << "if (start_cnt)\n";
        Out << "counter = counter + 1'b1;\n";
        Out << "end\n";
        Out << "assign cnt = counter;\n";
    }

    if (alloc->getDbgInfo()->isDebugRtlEnabled()) {
        Out << "wire debugger_start;\n"
            << "wire program_clk;\n"
            << "wire program_reset;\n";

        Function *fpMain = alloc->getModule()->getFunction("main");
        GenerateRTL *main = alloc->getGenerateRTL(fpMain);

        if (alloc->getDbgInfo()->getOptionTraceRegs()) {
            // Forward registers to be traced from main to top-level ports
            Out << "wire [" << (alloc->getDbgInfo()->getRegsTraceBits() - 1)
                << ":0] " << DEBUG_SIGNAL_NAME_TRACE_REGS << ";\n"
                << "wire " << DEBUG_SIGNAL_NAME_TRACE_REGS_EN << "_a;\n"
                << "wire " << DEBUG_SIGNAL_NAME_TRACE_REGS_EN << "_b;\n";

            vector<RTLDebugPort *> *ports = main->getDbgTracePorts();
            for (vector<RTLDebugPort *>::iterator port_it = ports->begin(),
                                                  port_end = ports->end();
                 port_it != port_end; ++port_it) {
                RTLDebugPort *port = *port_it;
                Out << "wire [" << port->getSignal()->getWidth().getHi() << ":"
                    << port->getSignal()->getWidth().getLo() << "] "
                    << port->getSignal()->getName() << ";\n";
            }
        }

        if (alloc->getDbgInfo()->getOptionPreserveOneHot()) {
            // Forward all "current_state" signals from main to top-level ports
            vector<RTLDebugPort *> *ports = main->getDbgStatePorts();
            for (auto port_it = ports->begin(), port_end = ports->end(); port_it != port_end; ++port_it) {
                RTLDebugPort *port = *port_it;
                Out << "wire [" << port->getSignal()->getWidth().getHi() << ":"
                    << port->getSignal()->getWidth().getLo() << "] "
                    << port->getSignal()->getName() << ";\n";
                Out << "reg [" << port->getSignal()->getWidth().getHi() << ":"
                    << port->getSignal()->getWidth().getLo() << "] "
                    << port->getSignal()->getName() << "_r;\n";
            }
        }

        Out << "\n"
            << "assign main_memory_controller_waitrequest = waitrequest;\n";
    }

    Out << "/* synthesis translate_off */\n"
        << "assign memory_controller_waitrequest = waitrequest;\n"
        << "/* synthesis translate_on */\n\n";
    /*
        if (LEGUP_CONFIG->getParameterInt("TEST_WAITREQUEST")) {
            // keep the wait request high for 5 cycles, then low for 1 cycle and
            // repeat
            Out << "integer counter;\n";
            Out << "always @(posedge clk) begin\n";
            Out << "\n";
            Out << "    if (counter < 5) begin\n";
            Out << "    memory_controller_waitrequest <= 1;\n";
            Out << "    counter = counter + 1;\n";
            Out << "    end\n";
            Out << "    else\n";
            Out << "    begin\n";
            Out << "    memory_controller_waitrequest <= 0;\n";
            Out << "    counter = 0;\n";
            Out << "    end\n";
            Out << "end\n";
        } else {
            Out << "assign memory_controller_waitrequest = 0;\n\n";
        }
    */
    if (alloc->getDbgInfo()->isDebugRtlEnabled()) {
        Out << "always @ (posedge clk) begin\n";
        Out << "\t" << DEBUG_SIGNAL_NAME_ACTIVE_INST
            << "_r <= " << DEBUG_SIGNAL_NAME_ACTIVE_INST << ";\n";
        Out << "\t" << DEBUG_SIGNAL_NAME_CURRENT_STATE
            << "_r <= " << DEBUG_SIGNAL_NAME_CURRENT_STATE << ";\n";

        // Add registers to all state signals
        Function *fpMain = alloc->getModule()->getFunction("main");
        GenerateRTL *main = alloc->getGenerateRTL(fpMain);
        vector<RTLDebugPort *> *ports = main->getDbgStatePorts();
        for (auto port_it = ports->begin(), port_end = ports->end(); port_it != port_end; ++port_it) {
            RTLDebugPort *port = *port_it;
            Out << "\t" << port->getSignal()->getName() << "_r <= " << port->getSignal()->getName() << ";\n";
        }
        Out << "end\n";

        // Add debugger core
        printDebuggerInstance();

        // Add muxing of state machines into a single state
        if (alloc->getDbgInfo()->getOptionPreserveOneHot())
            printDbgStateMuxer();

        // Instantiate trace scheduler for register tracing
        if (alloc->getDbgInfo()->getOptionTraceRegs())
            printTraceSchedulerInstance();
    }

    // instantiate memory controller
    if (!alloc->noSharedMemoryController()) {
        printMemoryControllerInstance();
    }

    // instantiate synchronization controller
    if (usesSynchronization) {
        printSynchronizationControllerInstance();
    }

    // check if the main function uses Pthreads
    bool usesPthreads = LEGUP_CONFIG->usesPthreadFunction("main");

    // instantiate main module
    printMainInstance(usesPthreads);

    // print debugger
    if (LEGUP_CONFIG->isDebuggerUsed()) {
        // make wires to match signal names used in debugger (signal names match
        // hybrid case)
        printDebugModuleSignals("_a");
        // instantiate debugger
        printDebugModuleInstance("_a");
        if (LEGUP_CONFIG->getParameterInt("DUAL_PORT_BINDING")) {
            printDebugModuleSignals("_b");
            printDebugModuleInstance("_b");
        }
    }

    Out << "endmodule\n\n";
}

void VerilogWriter::printTopHybrid(const Function *F, unsigned dataSize) {

    // this indicates whether this is a parallel function
    // if so, it will use polling instead of stalling
    // a parallel function does not always uses ParallelAPI, since a pthread
    // with no locks/barriers will not use parallel API.
    bool isParallel = LEGUP_CONFIG->isParallelAccel(*F);

    if (!LEGUP_CONFIG->isPCIeFlow()) {
        parseMIPSdisassembly();
    }

    printMemTag(dataSize);

    printAvalonInterface(F->getName());

    int AddressBusWidth = getAvalonBusWidth(F);

    printIODeclarations(AddressBusWidth);

    std::vector<int> arg_bitwidth;

    bool return64 = printSignalDeclarations(F, arg_bitwidth);

    int NumParams = F->arg_size();

    printAssignStatements(NumParams, return64, isParallel);

    printArgsReceivers(NumParams, arg_bitwidth);

    printStartDoneSignals();

    printMemorySignals();

    bool voidtype = printReturnValSignals(return64, F, isParallel);

    printMemoryControllerInstance();

    printAccelInstance(F, NumParams, voidtype);

    // print debugger
    if (LEGUP_CONFIG->isDebuggerUsed()) {
        printDebugModuleInstance("_a");
        if (LEGUP_CONFIG->getParameterInt("DUAL_PORT_BINDING")) {
            printDebugModuleInstance("_b");
        }
    }

    printModelsimSignals(voidtype);
}

void VerilogWriter::printDebugModuleSignals(std::string postfix) {
    Out << "// synthesis translate_off\n"
        << "wire enable" << postfix << ";\n"
        << "wire write_enable" << postfix << ";\n"
        << "wire [`MEMORY_CONTROLLER_ADDR_SIZE-1:0] memory_address" << postfix
        << ";\n"
        << "wire [`MEMORY_CONTROLLER_DATA_SIZE-1:0] data_to_memory" << postfix
        << ";\n"
        << "wire [`MEMORY_CONTROLLER_DATA_SIZE-1:0] data_from_memory" << postfix
        << ";\n\n";

    Out << "assign enable" << postfix << " = memory_controller_enable"
        << postfix << ";\n"
        << "assign write_enable" << postfix
        << " = memory_controller_write_enable" << postfix << ";\n"
        << "assign memory_address" << postfix << " = memory_controller_address"
        << postfix << ";\n"
        << "assign data_to_memory" << postfix << " = memory_controller_in"
        << postfix << ";\n"
        << "assign data_from_memory" << postfix << " = memory_controller_out"
        << postfix << ";\n"
        << "// synthesis translate_on\n\n";
}

void VerilogWriter::printDebugModuleInstance(std::string postfix) {

    // creating readdatavalid signal to be 2 cycles after read is asserted
    Out << "// synthesis translate_off\n"
        << "reg [1:0] readdatavalid" << postfix << ";\n"
        << "always @(posedge clk)\n"
        << "begin\n"
        << "\tif (!memory_controller_waitrequest)\n"
        << "\tbegin\n"
        << "\t\treaddatavalid" << postfix << " <= readdatavalid" << postfix
        << " << 1;\n"
        << "\t\tif (enable" << postfix << " & !write_enable" << postfix << ")\n"
        << "\t\t\treaddatavalid" << postfix << "[0] <= 1'b1;\n"
        << "\t\telse\n"
        << "\t\t\treaddatavalid" << postfix << "[0] <= 1'b0;\n"
        << "\tend\n"
        << "end\n\n";

    /*
        "reg readdatavalid_temp" << postfix << ";\n" <<
        "reg readdatavalid" << postfix << ";\n" <<
        "reg [1:0] readdatavalid" << postfix << ";\n" <<
        "always @(posedge clk)\n" <<
        "begin\n" <<
        "\treaddatavalid_temp" << postfix << " <= 0;\n" <<
        "\treaddatavalid" << postfix << " <= 0;\n" <<
        "\tif (enable" << postfix << " & ~write_enable" << postfix << "&
       ~memory_controller_waitrequest)\n" <<
        "\t\treaddatavalid_temp" << postfix << " <= 1;\n" <<
        "\tif (~memory_controller_waitrequest)\n" <<
        "\t\treaddatavalid" << postfix << " <= readdatavalid_temp" << postfix <<
       ";\n" <<
        "end\n\n";
    */
    // instantiate debugger
    Out << "legup_memory_transaction_monitor legup_debugger_instance" << postfix
        << " (\n"
        << "\t.clk (clk),\n"
        << "\t.address (memory_address" << postfix << "),\n"
        << "\t.read (enable" << postfix << " & !write_enable" << postfix
        << "),\n"
        << "\t.write (enable" << postfix << " & write_enable" << postfix
        << "),\n"
        << "\t.readdata (data_from_memory" << postfix << "),\n"
        << "\t.writedata (data_to_memory" << postfix << "),\n"
        << "\t.readdatavalid (readdatavalid" << postfix << "[1]),\n"
        << "\t.waitrequest (memory_controller_waitrequest)\n"
        << ");\n"
        << "defparam\n"
        << "\tlegup_debugger_instance" << postfix
        << ".CAPTURE_TRANSACTIONS = " << LEGUP_CONFIG->isDebuggerCaptureMode()
        << ",\n"
        << "\tlegup_debugger_instance" << postfix
        << ".READ_ADDRESS_FILENAME = \"read_addresses" << postfix << ".dat\",\n"
        << "\tlegup_debugger_instance" << postfix
        << ".READ_DATA_FILENAME = \"read_data" << postfix << ".dat\",\n"
        << "\tlegup_debugger_instance" << postfix
        << ".WRITE_ADDRESS_FILENAME = \"write_addresses" << postfix
        << ".dat\",\n"
        << "\tlegup_debugger_instance" << postfix
        << ".WRITE_DATA_FILENAME = \"write_data" << postfix << ".dat\";\n";

    Out << "// synthesis translate_on\n\n";
}

// this function parses the disassembly file (objdump) which contain all the
// addresses for global variables
// the variables names and their associated addresses are saved in a map
void VerilogWriter::parseMIPSdisassembly() {

    // get the file name
    const char *dis = NULL;
    std::string Disassembly;
    Disassembly = std::getenv("LEGUP_ACCELERATOR_FILENAME");
    Disassembly += ".src";
    dis = Disassembly.c_str();
    std::string var_name, var_addr;

    // TODO: LLVM 3.5 update: this:
    /*
    OwningPtr<MemoryBuffer> MB;
    error_code ec;
    // open file
    if(!(ec = MemoryBuffer::getFile(dis, MB))) {

        // store file content in a buffer
        MemoryBuffer *F = MB.take();
        StringRef Buffer = F->getBuffer();
    */

    // TODO: LLVM 3.5 update: gets replaced by this:
    auto MB = MemoryBuffer::getFile(dis);
    if (MB) {

        StringRef Buffer = MB.get()->getBuffer();
        // TODO: end LLVM 3.5 update

        // iterate through all RAMs
        for (Allocation::const_ram_iterator i = alloc->ram_begin(),
                                            e = alloc->ram_end();
             i != e; ++i) {
            RAM *R = *i;
            const Value *V = R->getValue();
            // if it's global variable
            if (const GlobalVariable *G = dyn_cast<GlobalVariable>(V)) {

                // don't need to store constants
                if (G->isConstant())
                    continue;

                // get the name of the variable
                // this is the actual name used in LLVM IR (to be used for
                // parsing in MIPS assembly)
                // separate names need to be used for parsing vs printing
                // verilog
                // since some llvm variables may have name.number format where
                // as in verilog it doesn't allow having . (dot)
                // in LegUp, . is replaced with _ (underscore)
                var_name = V->getName();
                var_name.insert(
                    0, "<"); // making into the format in MIPS disassembly
                var_name.append(">:");

                // search buffer for the variable
                size_t index = Buffer.find(var_name);
                if (index == StringRef::npos) {
                    errs() << "\nError: Address for global variable "
                           << var_name << " not found in disassembly!\n\n";
                    assert(0);
                }

                // index holds the starting position of the variable string
                // now we need to get the address which is written before the
                // variable name

                // go back in buffer until no more white space is found
                // this position is the position of the last number of the
                // address we are looking for
                while (Buffer[index] == ' ') {
                    index--;
                }
                size_t lastPos = index;
                // now go back in buffer until new line is found
                // this position is the position of the first number of the
                // address we are looking for
                while (Buffer[index] != '\n') {
                    index--;
                }
                size_t firstPos = index;

                var_addr = Buffer.substr(firstPos + 1, lastPos - firstPos - 1);

                // store the variable and its address is a map
                // the key here is the verilog name that will be printing
                globalAddresses[R->getName()] = var_addr;
            }
        }
    } else {
        assert(0 && "Disassembly file not found!\n");
    }
}

void VerilogWriter::printGlobalTags(RAM *R) {

    std::string var_name, var_addr, verilog_name;
    // this is the name to be printed in Verilog
    verilog_name = R->getName();
    if (globalAddresses.find(verilog_name) == globalAddresses.end()) {
        errs() << "\nError: Address for global variable " << verilog_name
               << " not found!\n\n";
        assert(0);
    }
    var_addr = globalAddresses[verilog_name];

    verilog_name.insert(0, "TAG_g_"); // make it into the proper TAG format
    verilog_name.append("_a");
    var_addr.insert(0, "32'h");
    Out << "`define " << verilog_name << " " << var_addr << "\n";
}

void VerilogWriter::printLocalTags(RAM *R, unsigned dataSize,
                                   unsigned &tagIndex) {

    unsigned numInstances = R->getNumInstances();
    for (unsigned i = 0; i < numInstances; ++i) {

        std::string tagName = R->getTag();
        if (numInstances != 1)
            tagName += "_inst" + utostr(i);

        if (tagIndex > 127) { // the biggest tag (number of local RAMs)
                              // supported is 127
            assert(
                0 &&
                "Too many local memories! The largest tag supported is 511!\n");
        }

        Out << "`define " << tagName << " `MEMORY_CONTROLLER_TAG_SIZE'd"
            << tagIndex << "\n";
        tagIndex++;
    }

    Out << "`define " << R->getTagAddrName() << " " << R->getTagAddr() << "\n";
}

void VerilogWriter::printMemTag(unsigned dataSize) {

    // since two tags are reserved for NULL and processor
    unsigned tagIndex = 2;
    RAM *R;

    // to print out memory controller tags
    for (Allocation::const_ram_iterator i = alloc->ram_begin(),
                                        e = alloc->ram_end();
         i != e; ++i) {
        R = *i;
        Out << "// " << getValueStr(R->getValue()) << '\n';
        if (stripRAM(R)) { // print memory address only if its a global variable
                           // which is not a constant
            printGlobalTags(R);
        } else {
            unsigned dataSize = std::max(dataSize, R->getDataWidth());
            printLocalTags(R, dataSize, tagIndex);
        }
    }

    Out << "`define CPU_ACCEL_PORT 32'h00000000"
        << "\n";
}

void VerilogWriter::printAvalonInterface(std::string ModuleName) {

    stripInvalidCharacters(ModuleName);
    Out << "`timescale 1 ns / 1 ns"
        << "\n";
    Out << "module " << ModuleName << "_top\n"
        << "\t(\n"
        << "\t\tcsi_clockreset_clk,\n"
        << "\t\tcsi_clockreset_reset,\n"
        << "\t\t//Slave interface to talk to processor\n"
        << "\t\t//Inputs\n"
        << "\t\tavs_s1_address,\n"
        << "\t\tavs_s1_read,\n"
        << "\t\tavs_s1_write,\n"
        << "\t\tavs_s1_writedata,\n"
        << "\t\t//Outputs\n"
        << "\t\tavs_s1_readdata,\n"
        << "\t\t//Memory\n"
        << "\t\tavm_ACCEL_readdata,\n"
        << "\t\tavm_ACCEL_waitrequest,\n"
        << "\t\tavm_ACCEL_address,\n"
        << "\t\tavm_ACCEL_writedata,\n"
        << "\t\tavm_ACCEL_write,\n"
        << "\t\tavm_ACCEL_read";

    Out << "\t);\n";
}

int VerilogWriter::getAvalonBusWidth(const Function *F) {

    int NumArg32bit = 0;
    int NumArg64bit = 0;
    for (Function::const_arg_iterator it = F->arg_begin(), e = F->arg_end();
         it != e; ++it) { // get number of 32 bit and 64 bit arguments
        if (const IntegerType *ITy = dyn_cast<IntegerType>(it->getType())) {
            if (ITy->getBitWidth() == 64) {
                NumArg64bit++;
            } else if (ITy->getBitWidth() <= 32) {
                NumArg32bit++;
            }
        } else if (it->getType()->isFloatTy()) { // if argument is a float
            NumArg32bit++;
        } else if (it->getType()->isDoubleTy()) { // if argument is a double
            NumArg64bit++;
        } else if (dyn_cast<PointerType>(
                       it->getType())) { // if argument is a pointer
            NumArg32bit++;
        } else if (dyn_cast<ArrayType>(
                       it->getType())) { // if argument is an array
            NumArg32bit++;
        } else {
            assert(0 && "Unsupported Argument for Accelerator\n");
        }
    }
    return (ceil(log(NumArg32bit + 2 * NumArg64bit + 3) /
                 log(2))); // define avalon slave address bus width according to
                           // number of arguments
}

void VerilogWriter::printIODeclarations(int AddressBusWidth) {
    Out << "\ninput csi_clockreset_clk;\n"
        << "input csi_clockreset_reset;\n"
        <<

        "input [" << AddressBusWidth - 1 << ":0]avs_s1_address;\n"
        << "input avs_s1_read;\n"
        << "input avs_s1_write;\n"
        <<

        "input [31:0] avs_s1_writedata;\n"
        << "output reg [31:0] avs_s1_readdata;\n"
        <<

        "input [127:0] avm_ACCEL_readdata;\n"
        << "output [127:0] avm_ACCEL_writedata;\n"
        << "input avm_ACCEL_waitrequest;\n"
        <<

        "output wire [`MEMORY_CONTROLLER_ADDR_SIZE-1:0] avm_ACCEL_address;\n"
        << "output wire avm_ACCEL_write;\n"
        << "output wire avm_ACCEL_read;\n";
}

bool VerilogWriter::printSignalDeclarations(const Function *F,
                                            std::vector<int> &arg_bitwidth) {

    const Type *ret = F->getReturnType();
    bool return64 = false;
    Out << "wire finish;\n";
    if (!(ret->isVoidTy())) {
        if (ret->isPointerTy()) {
            Out << "wire [31:0] return_val_wire;\n";
            Out << "reg [31:0] return_val;\n";
        } else {
            unsigned bitwidth = getBitWidth(ret);
            if (bitwidth == 64) {
                Out << "wire read_data_asserted_hi;\n";
                return64 = true;
            }

            Out << "wire [" << bitwidth - 1 << ":0] return_val_wire;\n";
            Out << "reg [" << bitwidth - 1 << ":0] return_val;\n";
        }
    }

    Out << "wire clk;\n"
        << "wire reset;\n"
        << "wire start;\n"
        << "wire stall_cpu;\n"
        << "wire unstall_cpu;\n" <<
        //	"reg unstall_cpu;\n"<<
        "wire memory_controller_waitrequest;\n" <<
        //	"reg memory_controller_waitrequest_reg;\n"<<
        "wire av_waitrequest;\n"
        << "reg start_reg;\n"
        << "reg finish_reg;\n"
        << "reg finish_reg_dly;\n"
        << "reg done;\n"
        << "wire read_data_asserted_lo;\n"
        << "wire read_status_asserted;\n" <<
        //"reg [1:0] memory_controller_state;\n"<<
        "reg memory_controller_state;\n" <<

        //"localparam MEMORY_ACCESS_LATENCY = 2;\n";
        "localparam MEMORY_ACCESS_LATENCY = 1;\n";
    printMemorySignalDeclarations("_a");
    if (LEGUP_CONFIG->getParameterInt("DUAL_PORT_BINDING")) {
        printMemorySignalDeclarations("_b");
    }

    Out << "localparam state_a = 1'b0;\n"
        << "localparam state_b = 1'b1;\n";
    /*
    Out << "localparam stateIDLE = 2'b00;\n"<<
    "localparam stateFIRST = 2'b01;\n"<<
    "localparam stateSECOND = 2'b10;\n";*/

    printArgumentSignalDeclarations(F, arg_bitwidth);
    return return64;
}

void
VerilogWriter::printArgumentSignalDeclarations(const Function *F,
                                               std::vector<int> &arg_bitwidth) {

    int NumParams = F->arg_size();
    int argIdx = 1;
    int data_size;
    if (NumParams > 0) {

        for (Function::const_arg_iterator it = F->arg_begin(), e = F->arg_end();
             it != e; ++it) {
            data_size = 31;
            if (const IntegerType *ITy = dyn_cast<IntegerType>(it->getType())) {
                if (ITy->getBitWidth() == 64) // if data type is long long
                    data_size = 63;
            } else if (it->getType()->isDoubleTy()) {
                data_size = 63;
            }
            arg_bitwidth.push_back(data_size);
            Out << "reg [" << data_size << ":0]ARG" << argIdx << ";\n";
            Out << "reg ARG" << argIdx << "_ready;\n";
            argIdx++;
        }
        Out << "wire parameter_ready;\n\n";
    }
}

void VerilogWriter::printMemorySignalDeclarations(std::string postfix) {

    Out << "wire [`MEMORY_CONTROLLER_ADDR_SIZE-1:0] memory_address" << postfix
        << ";\n"
        << "reg [`MEMORY_CONTROLLER_ADDR_SIZE-1:0] memory_address_reg"
        << postfix << ";\n";
    // 	"reg [`MEMORY_CONTROLLER_ADDR_SIZE-1:0] memory_address_reg_reg" <<
    // postfix << ";\n";
    if (LEGUP_CONFIG->getParameterInt("DUAL_PORT_BINDING")) {
        // Out << "reg [`MEMORY_CONTROLLER_ADDR_SIZE-1:0]
        // memory_controller_address" << postfix << ";\n";
        Out << "wire [`MEMORY_CONTROLLER_ADDR_SIZE-1:0] "
               "memory_controller_address" << postfix << ";\n";
    } else {
        Out << "wire [`MEMORY_CONTROLLER_ADDR_SIZE-1:0] "
               "memory_controller_address" << postfix << ";\n";
    }
    Out << "wire memory_controller_enable" << postfix << ";\n"
        << "wire memory_controller_write_enable" << postfix << ";\n"
        << "wire [`MEMORY_CONTROLLER_DATA_SIZE-1:0] memory_controller_in"
        << postfix << ";\n"
        << "wire [`MEMORY_CONTROLLER_DATA_SIZE-1:0] memory_controller_out"
        << postfix << ";\n"
        << "wire [1:0] memory_controller_size" << postfix << ";\n"
        << "wire [1:0] memory_size" << postfix << ";\n"
        << "reg [1:0] memory_controller_size_reg" << postfix << ";\n"
        <<

        "reg [`MEMORY_CONTROLLER_DATA_SIZE-1:0] sdram_data" << postfix << ";\n"
        << "reg [`MEMORY_CONTROLLER_DATA_SIZE-1:0] sdram_data_reg" << postfix
        << ";\n"
        << "reg [`MEMORY_CONTROLLER_DATA_SIZE-1:0] sdram_data_reg_reg"
        << postfix << ";\n"
        <<

        "reg [`MEMORY_CONTROLLER_DATA_SIZE-1:0] data_from_memory" << postfix
        << ";\n"
        << "wire [`MEMORY_CONTROLLER_DATA_SIZE-1:0] data_to_memory" << postfix
        << ";\n"
        << "reg [`MEMORY_CONTROLLER_DATA_SIZE-1:0] data_to_memory_reg"
        << postfix << ";\n"
        << "reg [`MEMORY_CONTROLLER_DATA_SIZE-1:0] data_from_memory_reg"
        << postfix << ";\n"
        <<

        "reg sdram_data_ready" << postfix << ";\n"
        << "wire flag_to_sdram" << postfix << ";\n"
        << "wire flag_to_sharedMemory" << postfix << ";\n"
        << "reg flag_to_sharedMemory_reg" << postfix << ";\n";
    if (usesSynchronization) {
        Out << "wire flag_to_mutexbarrier" << postfix << ";\n";
        Out << "reg flag_to_mutexbarrier_reg" << postfix << ";\n";
    }
    Out << "wire enable" << postfix << ";\n"
        << "reg enable_reg" << postfix << ";\n"
        << "wire write_enable" << postfix << ";\n"
        << "reg write_enable_reg" << postfix << ";\n"
        << "wire gnt" << postfix << ";\n";

    Out << "//shift register to indicate whether to take data from main memory "
           "or block RAM\n"
        << "reg [MEMORY_ACCESS_LATENCY-1:0] SDRAM_shiftreg" << postfix << ";\n"
        << "reg [MEMORY_ACCESS_LATENCY:0] BRAM_shiftreg" << postfix << ";\n"
        << "reg [MEMORY_ACCESS_LATENCY-1:0] SDRAM_shiftreg_changed" << postfix
        << ";\n"
        << "reg [MEMORY_ACCESS_LATENCY-1:0] BRAM_shiftreg_changed" << postfix
        << ";\n";

    if (LEGUP_CONFIG->getParameterInt("DUAL_PORT_BINDING")) {
        Out << "reg [MEMORY_ACCESS_LATENCY-1:0] doubleSDRAM_shiftreg" << postfix
            << ";\n"
            << "reg [MEMORY_ACCESS_LATENCY-1:0] doubleSDRAM_shiftreg_changed"
            << postfix << ";\n";
    }
}

void VerilogWriter::printACCELassignStatements(bool isParallel) {

    if (usesSynchronization) {
        // if locks or barriers are used, send in memory address for portA or
        // portB or send in cache address
        if (LEGUP_CONFIG->getParameterInt("DUAL_PORT_BINDING")) {
            Out << "assign avm_ACCEL_address = (flag_to_mutexbarrier_reg_a && "
                   "gnt_a)? memory_address_reg_a : (flag_to_mutexbarrier_reg_b "
                   "&& gnt_b)? memory_address_reg_b : `CPU_ACCEL_PORT;\n";
        } else {
            Out << "assign avm_ACCEL_address = (flag_to_mutexbarrier_reg_a)? "
                   "memory_address_reg_a : `CPU_ACCEL_PORT;\n";
        }
    } else {
        Out << "assign avm_ACCEL_address = `CPU_ACCEL_PORT;\n";
    }

    // when polling
    if (isParallel) {
        if (LEGUP_CONFIG->getParameterInt("DUAL_PORT_BINDING")) {
            Out << "assign avm_ACCEL_write = (gnt_a & write_enable_reg_a";
            Out << ") | (gnt_b & write_enable_reg_b";
            Out << ");\n" <<
                //"assign avm_ACCEL_read = (gnt_a & enable_a & !write_enable_a";
                "assign avm_ACCEL_read = (gnt_a & !write_enable_reg_a";
            // Out << ") | (gnt_b & enable_b & !write_enable_b";
            Out << ") | (gnt_b & !write_enable_reg_b";
            Out << ");";
        } else {
            Out << "assign avm_ACCEL_write = enable_reg_a & write_enable_reg_a "
                   "& flag_to_sharedMemory_reg_a;\n"
                << "assign avm_ACCEL_read = enable_reg_a & ~write_enable_reg_a "
                   "& (flag_to_sharedMemory_reg_a);";
        }
        // when stalling
    } else {
        if (LEGUP_CONFIG->getParameterInt("DUAL_PORT_BINDING")) {
            // Out << "assign avm_ACCEL_write = (gnt_a & write_enable_a) |
            // (gnt_b & write_enable_b) | (start | finish);\n" <<
            // added finish_reg to avoid Avalon violation
            Out << "assign avm_ACCEL_write = (gnt_a & write_enable_reg_a) | "
                   "(gnt_b & write_enable_reg_b) | (stall_cpu | unstall_cpu);\n"
                <<
                //"assign avm_ACCEL_read = (gnt_a & enable_reg_a &
                //!write_enable_reg_a) | (gnt_b & enable_reg_b &
                //!write_enable_reg_b);\n";
                "assign avm_ACCEL_read = (gnt_a & !write_enable_reg_a) | "
                "(gnt_b & !write_enable_reg_b);\n";
        } else {
            // Out << "assign avm_ACCEL_write = enable_a & write_enable_a &
            // flag_to_sharedMemory_a | start |finish;\n" <<
            // Out << "assign avm_ACCEL_write = enable_reg_a &
            // write_enable_reg_a & flag_to_sharedMemory_reg_a | start |finish |
            // finish_reg;\n" <<
            Out << "assign avm_ACCEL_write = enable_reg_a & write_enable_reg_a "
                   "& flag_to_sharedMemory_reg_a | stall_cpu | unstall_cpu;\n"
                << "assign avm_ACCEL_read = enable_reg_a & ~write_enable_reg_a "
                   "& flag_to_sharedMemory_reg_a;\n";
        }
    }

    if (LEGUP_CONFIG->getParameterInt("DUAL_PORT_BINDING")) {
        Out << "//send address portion\n"
            << "assign avm_ACCEL_writedata[31:0] = gnt_a? memory_address_reg_a "
               ": memory_address_reg_b;\n"
            << "//send data portion\n"
            << "assign avm_ACCEL_writedata[95:32] = gnt_a? "
               "data_to_memory_reg_a : data_to_memory_reg_b;\n"
            << "//send size portion\n"
            << "assign avm_ACCEL_writedata[96] = gnt_a? "
               "(memory_controller_size_reg_a == 2'b00) : "
               "(memory_controller_size_reg_b == 2'b00); //8-bit\n"
            << "assign avm_ACCEL_writedata[97] = gnt_a? "
               "(memory_controller_size_reg_a == 2'b01) : "
               "(memory_controller_size_reg_b == 2'b01); //16-bit\n"
            << "assign avm_ACCEL_writedata[98] = gnt_a? "
               "(memory_controller_size_reg_a == 2'b11) : "
               "(memory_controller_size_reg_b == 2'b11); //64-bit\n";
    } else {
        Out << "//send address portion\n"
            << "assign avm_ACCEL_writedata[31:0] = memory_address_reg_a;\n"
            << "//send data portion\n"
            << "assign avm_ACCEL_writedata[95:32] = data_to_memory_reg_a;\n"
            << "//send size portion\n"
            << "assign avm_ACCEL_writedata[96] = (memory_controller_size_reg_a "
               "== 2'b00); //8-bit\n"
            << "assign avm_ACCEL_writedata[97] = (memory_controller_size_reg_a "
               "== 2'b01); //16-bit\n"
            << "assign avm_ACCEL_writedata[98] = (memory_controller_size_reg_a "
               "== 2'b11); //64-bit\n";
    }

    Out << "//send flush signal (unused for accelerator)\n"
        << "assign avm_ACCEL_writedata[99] = 1'b0;\n";

    if (!isParallel) {
        Out << "//to stall processor\n"
            << "assign avm_ACCEL_writedata[100] = stall_cpu;\n"
            << "//to unstall processor\n"
            << "assign avm_ACCEL_writedata[101] = unstall_cpu;\n";
    } else {
        Out << "assign avm_ACCEL_writedata[100] = 1'b0;\n"
            << "assign avm_ACCEL_writedata[101] = 1'b0;\n";
    }

    Out << "assign avm_ACCEL_writedata[127:102] = 28'd0;\n";
}

void VerilogWriter::printMemoryAssignStatements(int NumParams, bool return64,
                                                bool isParallel) {

    if (!LEGUP_CONFIG->getParameterInt("DUAL_PORT_BINDING")) {
        Out << "assign gnt_a = enable_reg_a;\n";
    } else {
        // when both ports access Avalon at the same cycle, port A is always
        // given access first, then port B is given next.
        Out << "assign gnt_a = (flag_to_sharedMemory_reg_a && "
               "flag_to_sharedMemory_reg_b) ? enable_reg_a && "
               "(memory_controller_state == state_a) : enable_reg_a && "
               "flag_to_sharedMemory_reg_a;\n";
        Out << "assign gnt_b = (flag_to_sharedMemory_reg_a && "
               "flag_to_sharedMemory_reg_b) ? enable_reg_b && "
               "(memory_controller_state == state_b) : enable_reg_b && "
               "flag_to_sharedMemory_reg_b;\n";
        // Out << "assign gnt_a = (flag_to_sharedMemory_reg_a &&
        // flag_to_sharedMemory_reg_b) ? enable_reg_a &&
        // flag_to_sharedMemory_reg_a && (memory_controller_state !=
        // stateSECOND) : enable_reg_a && flag_to_sharedMemory_reg_a;\n";
        // Out << "assign gnt_b = (flag_to_sharedMemory_reg_a &&
        // flag_to_sharedMemory_reg_b) ? enable_reg_b &&
        // flag_to_sharedMemory_reg_b && (memory_controller_state ==
        // stateSECOND) : enable_reg_b && flag_to_sharedMemory_reg_b;\n";
    }

    // Out << "assign av_waitrequest = (avm_ACCEL_read | avm_ACCEL_write) &
    // avm_ACCEL_waitrequest;\n";
    Out << "assign av_waitrequest = avm_ACCEL_waitrequest;\n";
    // Out << "assign memory_controller_waitrequest = avm_ACCEL_waitrequest";
    Out << "assign memory_controller_waitrequest = av_waitrequest";

    // when both ports access Avalon in the same state
    // keep the waitrequest high until the second access is done
    if (LEGUP_CONFIG->getParameterInt("DUAL_PORT_BINDING")) {
        Out << " || (memory_controller_state == state_a) && "
               "(flag_to_sharedMemory_reg_a && flag_to_sharedMemory_reg_b)";
    }
    //	if (LEGUP_CONFIG->getParameterInt("DUAL_PORT_BINDING")) {
    //		Out << " || (memory_controller_state == stateFIRST)";
    //	}
    Out << ";\n";

    printMemoryControllerAssignStatements("_a");

    if (LEGUP_CONFIG->getParameterInt("DUAL_PORT_BINDING")) {
        printMemoryControllerAssignStatements("_b");
    }

    Out << "assign start = (avs_s1_address == 2) & (avs_s1_writedata[31:0] == "
           "1) & (avs_s1_write == 1'b1)";

    if (NumParams > 0) {
        Out << " & parameter_ready";
    }
    Out << ";\n";

    Out << "assign read_data_asserted_lo = (avs_s1_address == 0) & "
           "(avs_s1_read == 1'b1) & (done == 1'b1);\n";
    if (return64) {
        Out << "assign read_data_asserted_hi = (avs_s1_address == 1) & "
               "(avs_s1_read == 1'b1) & (done == 1'b1);\n";
    }

    printoffChipMemoryFlags("_a");
    if (LEGUP_CONFIG->getParameterInt("DUAL_PORT_BINDING")) {
        printoffChipMemoryFlags("_b");
    }

    // for polling
    if (isParallel) {
        Out << "assign read_status_asserted = (avs_s1_address == 2) & "
               "(avs_s1_read == 1'b1) & (done == 1'b1);\n";
    }
}

void VerilogWriter::printoffChipMemoryFlags(std::string postfix) {
    if (LEGUP_CONFIG->isPCIeFlow()) {
        Out << "assign flag_to_sdram" << postfix << " = memory_address"
            << postfix << "[30] | memory_address" << postfix << "[29];\n";
    } else {
        // This is the old DE2 way
        // Out << "assign flag_to_sdram" << postfix << " = (memory_address"
        //    << postfix << "[31:23] == `TAG_PROCESSOR);\n";
        //
        // This will hopefully be the new way
        // Out << "assign flag_to_sdram" << postfix << " = ~memory_address"
        //    << postfix << "[31];\n";
        //
        // Temporarily setting
        Out << "assign flag_to_sdram" << postfix << " = memory_address"
            << postfix << "[30];\n";
    }
    if (usesSynchronization) {
        Out << "assign flag_to_mutexbarrier" << postfix << " = memory_address"
            << postfix << "[31];\n";
    }

#if 0
    if (LEGUP_CONFIG->getDeviceFamily() == "CycloneII") {
        Out << "assign flag_to_sdram" << postfix << " = (memory_address"
            << postfix << "[31:23] == `TAG_PROCESSOR);\n";
        if (usesSynchronization) {
            Out << "assign flag_to_mutexbarrier" << postfix
                << " = memory_address" << postfix << "[31];\n";
        }
    } else if (LEGUP_CONFIG->getDeviceFamily() == "StratixIV") {
        Out << "assign flag_to_sdram" << postfix << " = memory_address"
            << postfix << "[30]";

        // PCIe Flow has an on chip ram that is accessible from
        // 0x2000_0000 to 0x200b_ffff
        //
        if (LEGUP_CONFIG->isPCIeFlow()) {
            Out << " | memory_address" << postfix << "[29];\n";
        } else {
            Out << ";\n";
        }
        if (usesSynchronization) {
            Out << "assign flag_to_mutexbarrier" << postfix
                << " = memory_address" << postfix << "[31];\n";
        }
    } else if (LEGUP_CONFIG->getDeviceFamily() == "CycloneIV") {
        Out << "assign flag_to_sdram" << postfix << " = memory_address"
            << postfix << "[30];\n";
        if (usesSynchronization) {
            Out << "assign flag_to_mutexbarrier" << postfix
                << " = memory_address" << postfix << "[31];\n";
        }
    } else if (LEGUP_CONFIG->getDeviceFamily() == "CycloneV") {
        // TODO: This may not be correct
        Out << "assign flag_to_sdram" << postfix << " = (memory_address"
            << postfix << "[31:23] == `TAG_PROCESSOR);\n";
        if (usesSynchronization) {
            Out << "assign flag_to_mutexbarrier" << postfix
                << " = memory_address" << postfix << "[31];\n";
        }
    } else {
        llvm_unreachable("Unrecognized device family!");
    }
#endif

    Out << "assign flag_to_sharedMemory" << postfix << " = flag_to_sdram"
        << postfix;
    if (usesSynchronization) {
        Out << " || flag_to_mutexbarrier" << postfix;
    }
    Out << ";\n";
}

void VerilogWriter::printMemoryControllerAssignStatements(std::string postfix) {
    if (!LEGUP_CONFIG->getParameterInt("DUAL_PORT_BINDING")) {
        Out << "assign memory_controller_address" << postfix
            << " = memory_address" << postfix << ";\n";
    } else {
        Out << "assign memory_controller_address" << postfix
            << " = (!memory_controller_waitrequest)? memory_address" << postfix
            << " & {32{!flag_to_sharedMemory" << postfix
            << "}} : memory_address_reg" << postfix
            << " & {32{!flag_to_sharedMemory_reg" << postfix << "}};\n";
    }

    Out << "assign memory_controller_enable" << postfix
        << " = (!memory_controller_waitrequest)? enable" << postfix
        << " && !flag_to_sharedMemory" << postfix << " : 0;\n";
    Out << "assign memory_controller_write_enable" << postfix
        << " = (!memory_controller_waitrequest)? write_enable" << postfix
        << " && !flag_to_sharedMemory" << postfix << " : 0;\n";
    Out << "assign memory_size" << postfix
        << " = (!memory_controller_waitrequest)? memory_controller_size"
        << postfix << " : memory_controller_size_reg" << postfix << ";\n";
    Out << "assign memory_controller_in" << postfix << " = data_to_memory"
        << postfix << ";\n";
}

void VerilogWriter::printAssignStatements(int NumParams, bool return64,
                                          bool isParallel) {

    if (NumParams > 0) {
        Out << "assign parameter_ready = ";
        for (int i = 1; i <= NumParams; i++) {
            Out << "ARG" << i << "_ready";
            if (i == NumParams) {
                continue;
            }
            Out << " && ";
        }

        Out << ";\n";
    }

    Out << "assign clk = csi_clockreset_clk;\n"
        << "assign reset = csi_clockreset_reset;\n";

    printACCELassignStatements(isParallel);

    printMemoryAssignStatements(NumParams, return64, isParallel);
}

void VerilogWriter::printArgsReceivers(int NumParams,
                                       const std::vector<int> &arg_bitwidth) {

    int argIdx = 1;
    int addrIdx = 3;
    int data_size;
    for (int i = 0; i < NumParams; i++) {
        data_size = arg_bitwidth[i];
        if (data_size == 31) {
            Out << "\nalways @(posedge clk)\n"
                << "begin\n"
                << "\tif (start || reset)\n"
                << "\tbegin\n"
                << "\t\tARG" << argIdx << "_ready <= 1'b0;\n"
                << "\tend\n"
                << "\telse if ((avs_s1_address == " << addrIdx
                << ") & (avs_s1_write))\n"
                << "\tbegin\n"
                << "\t\tARG" << argIdx << "[31:0] <= avs_s1_writedata[31:0];\n"
                << "\t\tARG" << argIdx << "_ready <= 1'b1;\n"
                << "\tend\n"
                << "end\n";
            addrIdx++;
            argIdx++;
        } else if (data_size == 63) {
            Out << "\nalways @(posedge clk)\n"
                << "begin\n"
                << "\tif ((avs_s1_address == " << addrIdx
                << ") & (avs_s1_write))\n"
                << "\tbegin\n"
                << "\t\tARG" << argIdx << "[31:0] <= avs_s1_writedata[31:0];\n"
                << "\tend\n"
                << "end\n";
            addrIdx++;

            Out << "\nalways @(posedge clk)\n"
                << "begin\n"
                << "\tif (start || reset)\n"
                << "\tbegin\n"
                << "\t\tARG" << argIdx << "_ready <= 1'b0;\n"
                << "\tend\n"
                << "\telse if ((avs_s1_address == " << addrIdx
                << ") & (avs_s1_write))\n"
                << "\tbegin\n"
                << "\t\tARG" << argIdx << "[63:32] <= avs_s1_writedata[31:0];\n"
                << "\t\tARG" << argIdx << "_ready <= 1'b1;\n"
                << "\tend\n"
                << "end\n";
            addrIdx++;
            argIdx++;
        } else {
            assert(0 && "Parameter data bitwidth undefined!\n");
        }
    }
}

void VerilogWriter::printStartDoneSignals() {

    Out << "\nalways @(posedge clk)\n"
        << "begin\n"
        << "\tstart_reg <= start;\n"
        << "\tif (reset)\n"
        << "\t\tfinish_reg <= 1'b0;\n"
        << "\telse\n"
        << "\t\tfinish_reg <= done & !memory_controller_waitrequest;\n"
        << "\tfinish_reg_dly <= finish_reg;\n"
        << "end\n";
    /*
    Out << "\nalways @(posedge clk)\n"<<
    "begin\n"<<
    "\tstart_reg <= start;\n"<<
    "\tif (reset)\n"<<
    "\t\tfinish_reg <= 1'b0;\n"<<
    "\telse if (!av_waitrequest)\n"<<
    "\t\tfinish_reg <= done;\n"<<
    "\tfinish_reg_dly <= finish_reg;\n"<<
    "end\n";
*/
    Out << "\n"
        << "always @(posedge clk)\n"
        << "begin\n"
        << "\tif (reset || start)\n"
        << "\t\tdone <= 1'b0;\n"
        << "\telse if (finish)\n"
        << "\t\tdone <= 1'b1;\n"
        << "end\n"
        << "\n";
    /*
        Out << "\n"<<
        "always @(posedge clk)\n"<<
        "begin\n"<<
        "\tmemory_controller_waitrequest_reg <=
       memory_controller_waitrequest;\n"<<
        "\tunstall_cpu <= finish_reg & memory_controller_waitrequest_reg &
       !memory_controller_waitrequest;\n"<<
        "end\n\n";
    */
    Out << "assign unstall_cpu = finish_reg & !finish_reg_dly;\n";
    Out << "assign stall_cpu = start;\n\n";
}

void VerilogWriter::printMemorySignals() {
    /*
        Out << "always @(posedge clk)\n"<<
        "begin\n"<<
        "\tif (!memory_controller_waitrequest)\n"<<
        "\tbegin\n"<<
        "\t\tmemory_address_reg_reg_a <= memory_address_reg_a;\n";
        if (LEGUP_CONFIG->getParameterInt("DUAL_PORT_BINDING")) {
            Out << "\t\tmemory_address_reg_reg_b <= memory_address_reg_b;\n";
        }
        Out << "\tend\n"<<
        "end\n\n";
    */
    printMemoryDataReadySignals();
    printMemoryShiftRegisters("_a");
    if (LEGUP_CONFIG->getParameterInt("DUAL_PORT_BINDING")) {
        printMemoryShiftRegisters("_b");
    }
    printMemoryDataSignals();

    printMemoryRegisters("_a");
    if (LEGUP_CONFIG->getParameterInt("DUAL_PORT_BINDING")) {
        printMemoryRegisters("_b");
    }
}

void VerilogWriter::printMemoryDataReadySignals() {

    if (LEGUP_CONFIG->getParameterInt("DUAL_PORT_BINDING")) {
        Out << "always @(*)\n"
            << "begin\n" <<
            //"\tsdram_data_ready_a <= gnt_a & !avm_ACCEL_waitrequest";
            "\tsdram_data_ready_a <= gnt_a & !av_waitrequest";
        Out << ";\n"
            << "end\n\n"
            <<

            "always @(*)\n"
            << "begin\n" <<
            //"\tsdram_data_ready_b <= gnt_b & !avm_ACCEL_waitrequest";
            "\tsdram_data_ready_b <= gnt_b & !av_waitrequest";
        Out << ";\n"
            << "end\n\n";
    } else {
        Out << "always @(*)\n";
        Out << "begin\n";
        Out << "\tsdram_data_ready_a <= avm_ACCEL_read & "
               "flag_to_sharedMemory_a & !av_waitrequest;\n";
        Out << "end\n\n";
    }
}

void VerilogWriter::printMemoryDataSignals() {

    printMemoryDataReceivers("_a");
    if (LEGUP_CONFIG->getParameterInt("DUAL_PORT_BINDING")) {
        printMemoryDataReceivers("_b");
        printMemoryStateMachine();
    }
}

void VerilogWriter::printMemoryStateMachine() {
    // print memory controller state machine for 2 Avalon memory accesses per
    // cycle
    // state_a is the initial state, where there may be two memory accesses from
    // both ports
    // if there are, then it's the state when port a would be given access
    // state_b is the state where port b is given access
    Out << "always @(posedge clk)\n"
        << "begin\n"
        << "\tif (reset) begin\n"
        << "\t\tmemory_controller_state <= state_a;\n"
        << "\tend else begin\n"
        << "\t\tcase (memory_controller_state)\n"
        << "\t\t\tstate_a: begin\n";
    Out << "\t\t\t\tif (flag_to_sharedMemory_reg_a && "
           "flag_to_sharedMemory_reg_b && !av_waitrequest)";
    Out << "\n"
        << "\t\t\t\t\tmemory_controller_state <= state_b;\n"
        << "\t\t\tend\n"
        << "\t\t\tstate_b: begin\n" <<
        //"\t\t\t\tif (!avm_ACCEL_waitrequest)\n"<<
        "\t\t\t\tif (!av_waitrequest)\n"
        << "\t\t\t\t\tmemory_controller_state <= state_a;\n"
        << "\t\t\tend\n"
        << "\t\tendcase\n"
        << "\tend\n"
        << "end\n";
    /*
    Out << "always @(posedge clk)\n"<<
    "begin\n"<<
    "\tif (reset) begin\n"<<
    "\t\tmemory_controller_state <= stateIDLE;\n"<<
    "\tend else begin\n"<<
    "\t\tcase (memory_controller_state)\n"<<
    "\t\t\tstateIDLE: begin\n";
    Out << "\t\t\t\tif (flag_to_sharedMemory_reg_a &&
    flag_to_sharedMemory_reg_b)";
    Out << "\n"<<
    "\t\t\t\t\tmemory_controller_state <= stateFIRST;\n"<<
    "\t\t\tend\n"<<
    "\t\t\tstateFIRST: begin\n"<<
    //"\t\t\t\tif (!avm_ACCEL_waitrequest)\n"<<
    "\t\t\t\tif (!av_waitrequest)\n"<<
    "\t\t\t\t\tmemory_controller_state <= stateSECOND;\n"<<
    "\t\t\tend\n"<<
    "\t\t\tstateSECOND: begin\n"<<
    //"\t\t\t\tif (!avm_ACCEL_waitrequest)\n"<<
    "\t\t\t\tif (!av_waitrequest)\n"<<
    "\t\t\t\t\tmemory_controller_state <= stateIDLE;\n"<<
    "\t\t\tend\n"<<
    "\t\tendcase\n"<<
    "\tend\n"<<
    "end\n";*/
}

void VerilogWriter::printMemoryDataReceivers(std::string postfix) {

    Out << "\nalways @(posedge clk)\n"
        << "begin\n"
        << "\tif (sdram_data_ready" << postfix << ")\n"
        << "\t\tsdram_data" << postfix << " <= avm_ACCEL_readdata[63:0];\n"
        << "end\n\n";

    Out << "always @(*)\n"
        << "begin\n"
        << "\tdata_from_memory" << postfix << " = data_from_memory_reg"
        << postfix << ";\n";
    if (LEGUP_CONFIG->getParameterInt("DUAL_PORT_BINDING")) {
        Out << "\tif (doubleSDRAM_shiftreg" << postfix
            << "[MEMORY_ACCESS_LATENCY-1] == 1'b1 && "
               "doubleSDRAM_shiftreg_changed" << postfix << ")\n"
            << "\t\tdata_from_memory" << postfix << " = sdram_data_reg"
            << postfix << ";\n";
    }
    Out << "\tif (SDRAM_shiftreg" << postfix
        << "[MEMORY_ACCESS_LATENCY-1] == 1'b1 && SDRAM_shiftreg_changed"
        << postfix << ")\n"
        << "\t\tdata_from_memory" << postfix << " = sdram_data" << postfix
        << ";\n"
        << "\telse if (BRAM_shiftreg" << postfix
        << "[MEMORY_ACCESS_LATENCY] == 1'b1 && BRAM_shiftreg_changed" << postfix
        << ")\n"
        << "\t\tdata_from_memory" << postfix << " = memory_controller_out"
        << postfix << ";\n";
    Out << "end\n\n";

    Out << "always @(posedge clk)\n"
        << "begin\n"
        << "if (reset)\n"
        << "\tdata_from_memory_reg" << postfix << " <= 0;\n"
        << "else\n"
        << "\tdata_from_memory_reg" << postfix << " <= data_from_memory"
        << postfix << ";\n";
    Out << "end\n\n"
        <<

        "always @(posedge clk)\n"
        << "begin\n";
    if (LEGUP_CONFIG->getParameterInt("DUAL_PORT_BINDING")) {
        // Out << "\tif (!(memory_controller_waitrequest && gnt" << postfix <<
        // "))\n";
        Out << "\tif (!memory_controller_waitrequest)\n";
    } else {
        Out << "\tif (!memory_controller_waitrequest)\n";
    }
    Out << "\t\tsdram_data_reg" << postfix << " <= sdram_data" << postfix
        << ";\n"
        << "end\n\n";

    if (LEGUP_CONFIG->getParameterInt("DUAL_PORT_BINDING")) {
        Out << "always @(posedge clk)\n"
            << "begin\n" <<
            //"\tif (!(memory_controller_waitrequest && gnt" << postfix <<
            //"))\n"<<
            "\tif (!memory_controller_waitrequest)\n"
            << "\t\tsdram_data_reg_reg" << postfix << " <= sdram_data_reg"
            << postfix << ";\n"
            << "end\n\n";
    }
}

void VerilogWriter::printMemoryRegisters(std::string postfix) {

    Out << "always @(posedge clk)\n"
        << "begin\n"
        << "\tif (reset)\n"
        << "\tbegin\n"
        << "\t\tflag_to_sharedMemory_reg" << postfix << " <= 0;\n";
    if (usesSynchronization) {
        Out << "\t\tflag_to_mutexbarrier_reg" << postfix << " <= 0;\n";
    }
    Out << "\t\tenable_reg" << postfix << " <= 0;\n"
        << "\t\tmemory_address_reg" << postfix << " <= 0;\n"
        << "\t\twrite_enable_reg" << postfix << " <= 0;\n"
        << "\t\tdata_to_memory_reg" << postfix << " <= 0;\n"
        << "\t\tmemory_controller_size_reg" << postfix << " <= 0;\n"
        << "\tend\n"
        << "\telse if (!memory_controller_waitrequest)\n"
        << "\tbegin\n"
        << "\t\tflag_to_sharedMemory_reg" << postfix
        << " <= flag_to_sharedMemory" << postfix << ";\n";
    if (usesSynchronization) {
        Out << "\t\tflag_to_mutexbarrier_reg" << postfix
            << " <= flag_to_mutexbarrier" << postfix << ";\n";
    }
    Out << "\t\tenable_reg" << postfix << " <= enable" << postfix << ";\n"
        << "\t\tmemory_address_reg" << postfix << " <= memory_address"
        << postfix << ";\n"
        << "\t\twrite_enable_reg" << postfix << " <= write_enable" << postfix
        << ";\n"
        << "\t\tdata_to_memory_reg" << postfix << " <= data_to_memory"
        << postfix << ";\n"
        << "\t\tmemory_controller_size_reg" << postfix
        << " <= memory_controller_size" << postfix << ";\n"
        << "\tend\n"
        << "end\n";
}

void VerilogWriter::printMemoryShiftRegisters(std::string postfix) {

    Out << "always @(posedge clk)\n"
        << "begin\n"
        << "\tSDRAM_shiftreg_changed" << postfix << " <= 1'b0;\n";
    if (LEGUP_CONFIG->getParameterInt("DUAL_PORT_BINDING")) {
        Out << "\tif (!memory_controller_waitrequest)\n";
    } else {
        Out << "\tif (!memory_controller_waitrequest)\n";
    }
    Out << "\tbegin\n"
        << "\t\tSDRAM_shiftreg_changed" << postfix << " <= 1'b1;\n"
        << "\t\tSDRAM_shiftreg" << postfix << " <= SDRAM_shiftreg" << postfix
        << " << 1;\n"
        << "\t\tif (avm_ACCEL_read";
    if (LEGUP_CONFIG->getParameterInt("DUAL_PORT_BINDING")) {
        Out << " && gnt" << postfix;
    }

    Out << ")\n"
        << "\t\t\tSDRAM_shiftreg" << postfix << "[0] <= 1'b1;\n"
        << "\t\telse\n"
        << "\t\t\tSDRAM_shiftreg" << postfix << "[0] <= 1'b0;\n"
        << "\tend\n"
        << "end\n\n"
        <<

        "always @(posedge clk)\n"
        << "begin\n"
        << "\tBRAM_shiftreg_changed" << postfix << " <= 1'b0;\n";
    if (LEGUP_CONFIG->getParameterInt("DUAL_PORT_BINDING")) {
        Out << "\tif (!memory_controller_waitrequest)\n";
        // Out << "\tif (!(memory_controller_waitrequest &&
        // flag_to_sharedMemory" << postfix << "))\n";
    } else {
        Out << "\tif (!memory_controller_waitrequest)\n";
    }

    Out << "\tbegin\n"
        << "\t\tBRAM_shiftreg_changed" << postfix << " <= 1'b1;\n"
        << "\t\tBRAM_shiftreg" << postfix << " <= BRAM_shiftreg" << postfix
        << " << 1;\n"
        << "\t\tif (memory_controller_enable" << postfix
        << " && !memory_controller_write_enable" << postfix;
    Out << ")\n"
        << "\t\t\tBRAM_shiftreg" << postfix << "[0] <= 1'b1;\n"
        << "\t\telse\n"
        << "\t\t\tBRAM_shiftreg" << postfix << "[0] <= 1'b0;\n"
        << "\tend\n"
        << "end\n\n";

    if (LEGUP_CONFIG->getParameterInt("DUAL_PORT_BINDING")) {
        Out << "always @(posedge clk)\n"
            << "begin\n"
            << "\tdoubleSDRAM_shiftreg_changed" << postfix << " <= 1'b0;\n";
        Out << "\tif (!memory_controller_waitrequest)\n";
        Out << "\tbegin\n"
            << "\t\tdoubleSDRAM_shiftreg_changed" << postfix << " <= 1'b1;\n"
            << "\t\tdoubleSDRAM_shiftreg" << postfix
            << " <= doubleSDRAM_shiftreg" << postfix << " << 1;\n" <<
            //"\t\tif (memory_controller_state == stateSECOND";
            "\t\tif (memory_controller_state == state_b";
        Out << ")\n"
            << "\t\t\tdoubleSDRAM_shiftreg" << postfix << "[0] <= 1'b1;\n"
            << "\t\telse\n"
            << "\t\t\tdoubleSDRAM_shiftreg" << postfix << "[0] <= 1'b0;\n"
            << "\tend\n"
            << "end\n\n";
    }
}

bool VerilogWriter::printReturnValSignals(bool return64, const Function *F,
                                          bool isParallel) {

    const Type *ret = F->getReturnType();
    // CPU reading from Accelerator
    bool voidtype = true;
    if (ret->getTypeID() != Type::VoidTyID) {
        voidtype = false;
    }

    if (!voidtype) {
        Out << "\n"
            << "always @(posedge clk)\n"
            << "begin\n"
            << "\tif (finish)\n"
            << "\t\treturn_val <= return_val_wire;\n"
            << "end\n"
            << "\n\n";
    }

    // If this is a parallel accelerator
    if (isParallel) {
        // print out always block for polling
        Out << "always @(*)\n"
            << "begin\n";
        Out << "\tif (read_status_asserted) //for polling\n"
            << "\t\tavs_s1_readdata <= 1;\n";
        // if this is not a void function
        if (!voidtype) {
            // print out signals to return data
            if (return64) {
                Out << "\telse if (read_data_asserted_lo) //to return data\n"
                    << "\t\tavs_s1_readdata = return_val[31:0];\n";
                Out << "\telse if (read_data_asserted_hi)\n"
                    << "\t\tavs_s1_readdata = return_val[63:32];\n";
            } else {
                Out << "\telse if (read_data_asserted_lo) //to return data\n"
                    << "\t\tavs_s1_readdata = return_val;\n";
            }
        }
        Out << "\telse\n"
            << "\t\tavs_s1_readdata = 0;\n"
            << "end\n\n";
        // if this is a sequential accelerator
    } else {
        // if this is not a void function
        if (!voidtype) {
            // print out always block to return data
            Out << "always @(*)\n"
                << "begin\n";
            if (return64) {
                Out << "\tif (read_data_asserted_lo) //to return data\n"
                    << "\t\tavs_s1_readdata = return_val[31:0];\n";
                Out << "\telse if (read_data_asserted_hi)\n"
                    << "\t\tavs_s1_readdata = return_val[63:32];\n";
            } else {
                Out << "\tif (read_data_asserted_lo) //to return data\n"
                    << "\t\tavs_s1_readdata = return_val;\n";
            }
            Out << "\telse\n"
                << "\t\tavs_s1_readdata = 0;\n"
                << "end\n\n";
        }
    }

    return voidtype;
}

void VerilogWriter::printMainInstance(const bool usesPthreads) {

    // instantiate main

    if (alloc->getDbgInfo()->isDebugRtlEnabled()) {
        Out << "main #(." << DEBUG_PARAM_NAME_PARENT_INST << "(0)) main_inst("
            << "\n";
        Out << "\t"
            << ".clk( program_clk ),"
            << "\n";
    } else {
        Out << "main main_inst("
            << "\n";
        Out << "\t"
            << ".clk( clk ),"
            << "\n";
    }

    Out << "\t"
        << ".clk2x( clk2x ),"
        << "\n";
    Out << "\t"
        << ".clk1x_follower( clk1x_follower ),"
        << "\n";

    if (alloc->getDbgInfo()->isDebugRtlEnabled()) {
        Out << "\t"
            << ".reset( program_reset ),"
            << "\n"
            << "\t"
            << ".start( debugger_start ),"
            << "\n";
    } else {
        Out << "\t"
            << ".reset( reset ),"
            << "\n"
            << "\t"
            << ".start( start ),"
            << "\n";
    }

    Out << "\t"
        << ".finish( finish ),"
        << "\n"
        << "\t"
        << ".return_val( return_val )";

    // if it uses Pthreads, the port names are appended with _arbiter_
    // this is because memory accesses for pthreads instances need to through
    // arbiter
    // but regular memory accesses outside the pthread instances also need to go
    // through arbiter
    // since pthreads are non-blocking and the FSM will continue to execute
    // after
    // starting pthread instances

    // When using synchronization, the memory_controller_out signals are named
    // differently
    // since it needs to be muxed between memory_controller and synchronization
    // controller

    if (usesPthreads || usesSynchronization) {
        if (usesPthreads) {
            Out << ",\n\t"
                << ".memory_controller_address_arbiter_a( "
                   "memory_controller_address_a ),"
                << "\n"
                << "\t"
                << ".memory_controller_address_arbiter_b( "
                   "memory_controller_address_b ),"
                << "\n";
            if (usesSynchronization) {
                Out << "\t"
                    << ".memory_controller_enable_arbiter_a( enable_a ),"
                    << "\n"
                    << "\t"
                    << ".memory_controller_enable_arbiter_b( enable_b ),"
                    << "\n"
                    << "\t"
                    << ".memory_controller_write_enable_arbiter_a( "
                       "write_enable_a ),"
                    << "\n"
                    << "\t"
                    << ".memory_controller_write_enable_arbiter_b( "
                       "write_enable_b ),"
                    << "\n";
            } else {
                Out << "\t"
                    << ".memory_controller_enable_arbiter_a( "
                       "memory_controller_enable_a ),"
                    << "\n"
                    << "\t"
                    << ".memory_controller_enable_arbiter_b( "
                       "memory_controller_enable_b ),"
                    << "\n"
                    << "\t"
                    << ".memory_controller_write_enable_arbiter_a( "
                       "memory_controller_write_enable_a ),"
                    << "\n"
                    << "\t"
                    << ".memory_controller_write_enable_arbiter_b( "
                       "memory_controller_write_enable_b ),"
                    << "\n";
            }
            Out << "\t"
                << ".memory_controller_waitrequest_arbiter( "
                   "memory_controller_waitrequest ),"
                << "\n"
                << "\t"
                << ".memory_controller_in_arbiter_a( memory_controller_in_a ),"
                << "\n"
                << "\t"
                << ".memory_controller_in_arbiter_b( memory_controller_in_b ),"
                << "\n";
        } else {
            if (usesSynchronization) {
                Out << ",\n\t"
                    << ".memory_controller_address_a( "
                       "memory_controller_address_a ),"
                    << "\n"
                    << "\t"
                    << ".memory_controller_address_b( "
                       "memory_controller_address_b ),"
                    << "\n"
                    << "\t"
                    << ".memory_controller_enable_a( enable_a ),"
                    << "\n"
                    << "\t"
                    << ".memory_controller_enable_b( enable_b ),"
                    << "\n"
                    << "\t"
                    << ".memory_controller_write_enable_a( write_enable_a ),"
                    << "\n"
                    << "\t"
                    << ".memory_controller_write_enable_b( write_enable_b ),"
                    << "\n"
                    << "\t"
                    << ".memory_controller_waitrequest( "
                       "memory_controller_waitrequest ),"
                    << "\n"
                    << "\t"
                    << ".memory_controller_in_a( memory_controller_in_a ),"
                    << "\n"
                    << "\t"
                    << ".memory_controller_in_b( memory_controller_in_b ),"
                    << "\n";
            }
        }

        if (alloc->usesGenericRAMs()) {
            if (usesPthreads) {
                Out << "\t"
                    << ".memory_controller_size_arbiter_a( "
                       "memory_controller_size_a ),"
                    << "\n";
                Out << "\t"
                    << ".memory_controller_size_arbiter_b( "
                       "memory_controller_size_b ),"
                    << "\n";
            } else {
                Out << "\t"
                    << ".memory_controller_size_a( memory_controller_size_a ),"
                    << "\n";
                Out << "\t"
                    << ".memory_controller_size_b( memory_controller_size_b ),"
                    << "\n";
            }
        }

        if (usesPthreads) {
            if (usesSynchronization) {
                Out << "\t"
                    << ".memory_controller_out_arbiter_a( data_from_memory_a ),"
                    << "\n";
                Out << "\t"
                    << ".memory_controller_out_arbiter_b( data_from_memory_b )";
            } else {
                Out << "\t"
                    << ".memory_controller_out_arbiter_a( "
                       "memory_controller_out_a ),"
                    << "\n";
                Out << "\t"
                    << ".memory_controller_out_arbiter_b( "
                       "memory_controller_out_b )";
            }
        } else {
            if (usesSynchronization) {
                Out << "\t"
                    << ".memory_controller_out_a( data_from_memory_a ),"
                    << "\n";
                Out << "\t"
                    << ".memory_controller_out_b( data_from_memory_b )";
            }
        }
    } else {

        // Get a pointer to the PropagatingSignals singleton
        //
        PropagatingSignals *ps = alloc->getPropagatingSignals();
        std::vector<PropagatingSignal *> signals =
            ps->getPropagatingSignalsForFunctionNamed("main");

        // Iterate through main's propagating signals and print them
        //
        for (std::vector<PropagatingSignal *>::iterator si = signals.begin();
             si != signals.end(); ++si) {

            PropagatingSignal *propSignal = *si;

            // A bit of a hack for the debugger, since the memory controller
            // signals coming out of 'main' need to be prefixed with 'main_'
            std::string signalName = propSignal->getName();
            if (alloc->getDbgInfo()->isDebugRtlEnabled()) {
                if (stringStartsWith(signalName, "memory_controller")) {
                    signalName = "main_" + signalName;
                }
            }
            Out << ",\n\t." << propSignal->getName() << "(" << signalName
                << ")";
        }
    }

    if (alloc->getDbgInfo()->isDebugRtlEnabled()) {

        Out << ",\n\t." << DEBUG_SIGNAL_NAME_ACTIVE_INST << "("
            << DEBUG_SIGNAL_NAME_ACTIVE_INST << ")";

        Function *fpMain = alloc->getModule()->getFunction("main");
        GenerateRTL *main = alloc->getGenerateRTL(fpMain);

        if (alloc->getDbgInfo()->getOptionPreserveOneHot()) {
            // Connect all state machine signals from main to top
            vector<RTLDebugPort *> *ports = main->getDbgStatePorts();
            for (vector<RTLDebugPort *>::iterator port_it = ports->begin(),
                                                  port_end = ports->end();
                 port_it != port_end; ++port_it) {
                RTLDebugPort *port = *port_it;
                Out << ",\n\t." << port->getSignal()->getName() << "("
                    << port->getSignal()->getName() << ")";
            }
        } else {
            Out << ",\n\t." << DEBUG_SIGNAL_NAME_CURRENT_STATE << "("
                << DEBUG_SIGNAL_NAME_CURRENT_STATE << ")";
        }

        if (alloc->getDbgInfo()->getOptionTraceRegs()) {
            // Connect all register signals from main to top
            vector<RTLDebugPort *> *dbgTracePorts = main->getDbgTracePorts();
            for (vector<RTLDebugPort *>::iterator
                     port_it = dbgTracePorts->begin(),
                     port_end = dbgTracePorts->end();
                 port_it != port_end; ++port_it) {
                RTLDebugPort *port = *port_it;
                Out << ",\n\t." << port->getSignal()->getName() << "("
                    << port->getSignal()->getName() << ")";
            }
        }
    }

    Out << "\n);\n\n";
}

void VerilogWriter::printMemoryControllerInstance() {

    if (LEGUP_CONFIG->getParameterInt("DUAL_PORT_BINDING")) {
        Out << "memory_controller memory_controller_inst (\n"
            << "\t.clk( clk ),\n";
        // if (LEGUP_CONFIG->numAccelerators() > 0) {
        if (LEGUP_CONFIG->isHybridFlow() || LEGUP_CONFIG->isPCIeFlow()) {
            Out << "\t.flag_to_sharedMemory_a( flag_to_sharedMemory_a ),\n";
            Out << "\t.flag_to_sharedMemory_b( flag_to_sharedMemory_b ),\n";
        }
        Out << "\t.memory_controller_enable_a( memory_controller_enable_a ),\n"
            << "\t.memory_controller_enable_b( memory_controller_enable_b ),\n"
            << "\t.memory_controller_address_a( memory_controller_address_a "
               "),\n"
            << "\t.memory_controller_address_b( memory_controller_address_b "
               "),\n"
            << "\t.memory_controller_write_enable_a( "
               "memory_controller_write_enable_a ),\n"
            << "\t.memory_controller_write_enable_b( "
               "memory_controller_write_enable_b ),\n"
            << "\t.memory_controller_in_a( memory_controller_in_a ),\n"
               "\t.memory_controller_in_b( memory_controller_in_b ),\n";
        if (alloc->usesGenericRAMs()) {
            // if (LEGUP_CONFIG->numAccelerators() > 0) {
            if (LEGUP_CONFIG->isHybridFlow() || LEGUP_CONFIG->isPCIeFlow()) {
                Out << "\t.memory_controller_size_a( memory_size_a ),\n"
                    << "\t.memory_controller_size_b( memory_size_b ),\n";
            } else {
                Out << "\t.memory_controller_size_a( memory_controller_size_a "
                       "),\n"
                    << "\t.memory_controller_size_b( memory_controller_size_b "
                       "),\n";
            }
        }
        // If we remove the output register of the memory controller, the port
        // names
        // are slightly different
        if (LEGUP_CONFIG->duplicate_load_reg()) {
            Out << "\t.memory_controller_waitrequest( "
                   "memory_controller_waitrequest ),\n"
                << "\t.memory_controller_out_a( memory_controller_out_a ),\n"
                << "\t.memory_controller_out_b( memory_controller_out_b )\n"
                << ");\n\n";
        } else {
            Out << "\t.memory_controller_waitrequest( "
                   "memory_controller_waitrequest ),\n"
                << "\t.memory_controller_out_reg_a( memory_controller_out_a "
                   "),\n"
                << "\t.memory_controller_out_reg_b( memory_controller_out_b )\n"
                << ");\n\n";
        }
    } else {
        Out << "memory_controller memory_controller_inst (\n"
            << "\t.clk( clk ),\n";
        // if (LEGUP_CONFIG->numAccelerators() > 0) {
        if (LEGUP_CONFIG->isHybridFlow() || LEGUP_CONFIG->isPCIeFlow()) {
            Out << "\t.flag_to_sharedMemory_a( flag_to_sharedMemory_a ),\n";
            Out << "\t.flag_to_sharedMemory_b( 1'b0 ),\n";
        }
        Out << "\t.memory_controller_enable_a( memory_controller_enable_a ),\n"
            << "\t.memory_controller_enable_b( 1'b0 ),\n"
            << "\t.memory_controller_address_a( memory_controller_address_a "
               "),\n"
            << "\t.memory_controller_address_b( "
               "{`MEMORY_CONTROLLER_ADDR_SIZE{1'b0}} ),\n"
            << "\t.memory_controller_write_enable_a( "
               "memory_controller_write_enable_a ),\n"
            << "\t.memory_controller_write_enable_b( 1'b0 ),\n"
            << "\t.memory_controller_in_a( memory_controller_in_a ),\n"
               "\t.memory_controller_in_b( "
               "{`MEMORY_CONTROLLER_DATA_SIZE{1'b0}} ),\n";
        if (alloc->usesGenericRAMs()) {
            Out << "\t.memory_controller_size_a( memory_controller_size_a ),\n"
                << "\t.memory_controller_size_b( 2'b00 ),\n";
        }
        // If we remove the output register of the memory controller, the port
        // names
        // are slightly different
        if (LEGUP_CONFIG->duplicate_load_reg()) {
            Out << "\t.memory_controller_waitrequest( "
                   "memory_controller_waitrequest ),\n"
                << "\t.memory_controller_out_a( memory_controller_out_a ),\n"
                << "\t.memory_controller_out_b( )\n"
                << ");\n\n";
        } else {
            Out << "\t.memory_controller_waitrequest( "
                   "memory_controller_waitrequest ),\n"
                << "\t.memory_controller_out_reg_a( memory_controller_out_a "
                   "),\n"
                << "\t.memory_controller_out_reg_b( )\n"
                << ");\n\n";
        }
    }
}

void VerilogWriter::printAccelInstance(const Function *F, int NumParams,
                                       bool voidtype) {

    int argumentNumber = 1;

    RTLModule *mod = alloc->getModuleForFunction(F);
    Out << mod->getBody() << "\n";
    std::string moduleName = mod->getName();
    stripInvalidCharacters(moduleName);
    Out << moduleName << " " << moduleName << "_inst ("
        << "\n";

    // CAUTION: This loop does not increment itself.  Any "continue"s
    // must increment i.
    for (RTLModule::const_signal_iterator i = mod->port_begin(),
                                          e = mod->port_end();
         i != e;) {

        const RTLSignal *s = *i;
        std::string name = s->getName();

        // Some of the signals at this level do not have actual connections in
        // the RTL.  We must check to see if the signal has a driver and then
        // provide some special handling if it doesn't.
        //
        std::string driverName =
            s->getNumDrivers() ? s->getDriver(0)->getName() : s->getName();

        bool signalIsArgument =
            driverName.find("arg_") == 0 &&
            name.find("memory_controller") == std::string::npos;
        if (signalIsArgument) {
            driverName = "ARG" + IntToString(argumentNumber);
            ++argumentNumber;
        }

        if (name == "return_val" && !voidtype)
            driverName = "return_val_wire";
        else if (name == "finish")
            driverName = "finish";
        else if (name == "clk2x")
            driverName = "1'b0";
        else if (name == "clk1x_follower")
            driverName = "1'b0";

        // memory_controller_waitrequest is the proper name, so we don't want to
        // change it.  If the signal is any other memory_controller signal
        // we want to modify its name to match the special convention for the
        // hybrid flow.
        //
        // NOTE for future refactorers: you can check if a signal is a memory
        // signal with the propagating signals data structure in Allocation.cpp
        //
        // The following code was left as a string test to remind all users of
        // how fragile the code that follows is.  The string test should be
        // changed when there is a safer way to connect the memory signals in
        // the hybrid flow.
        //
        //                                                         - Mathew hall
        //
        else if ((name.find("memory_controller_waitrequest") ==
                  std::string::npos) &&
                 (name.find("memory_controller") != std::string::npos)) {

            // Unfortunately, we must convert the standard memory signal names
            // to a custom scheme for the hybrid flow.
            //

            char lastCharacter = *name.rbegin();
            if (!(LEGUP_CONFIG->getParameterInt("DUAL_PORT_BINDING")) &&
                lastCharacter == 'b') {
                ++i;
                continue;
            }

            // This code is fragile.  If the following assertion led you here
            // then somebody probably changed the suffix style for memory
            // signals.
            //
            assert(lastCharacter == 'a' || lastCharacter == 'b');

            if (name.find("_write_enable_") != std::string::npos)
                driverName = "write_enable";
            else if (name.find("_enable_") != std::string::npos)
                driverName = "enable";
            else if (name.find("_address_") != std::string::npos)
                driverName = "memory_address";
            else if (name.find("_size_") != std::string::npos)
                driverName = "memory_controller_size";
            else if (name.find("_in_") != std::string::npos)
                driverName = "data_to_memory";
            else if (name.find("_out_") != std::string::npos)
                driverName = "data_from_memory";
            else
                assert(0); // Is there some new memory signal?

            driverName += (lastCharacter == 'a') ? "_a" : "_b";
        }

        if (!driverName.empty()) {
            Out << "\t";
            Out << "." << name << "(" << driverName << ")"
                << ((++i == e) ? "\n);" : ",\n");
        } else {
            ++i;
        }
    }
}

void VerilogWriter::printModelsimSignals(bool voidtype) {

    // modelsim display statement for time and return value
    Out << "// synthesis translate_off\n"
        << "always@(finish) begin\n"
        << "\tif (finish == 1) begin\n"
        << "\t\t$display(\"At t=%t clk=%b finish=%b";
    if (!voidtype)
        Out << " return_val=%d\",";
    else
        Out << "\",";

    Out << " $time, clk, finish";
    if (!voidtype)
        Out << ", return_val_wire);\n";
    else
        Out << ");\n";

    Out << "\tend\n"
        << "end\n"
        << "// synthesis translate_on\n\n";
    Out << "endmodule\n\n";
}

void VerilogWriter::printModuleHeader() {
    Out << "`timescale 1 ns / 1 ns\n";
    std::string moduleName = rtl->getName();
    stripInvalidCharacters(moduleName);
    // Out << "module " << rtl->getName() << "\n" <<
    Out << "module " << moduleName << "\n"
        << "("
        << "\n";

    for (RTLModule::const_signal_iterator i = rtl->port_begin(),
                                          e = rtl->port_end();
         i != e; ++i) {
        Out << "\t" << (*i)->getName();
        if (i != rtl->port_end() - 1)
            Out << ",";
        Out << "\n";
    }
    Out << ");"
        << "\n\n";

    if (alloc->getDbgInfo()->isDebugRtlEnabled()) {
        // Add Verilog function that determines the Instance ID of this module,
        // given
        // its parent instance ID.
        const map<int, int> *instanceMapping = rtl->dbgGetInstanceMapping();
        if (instanceMapping->begin() != instanceMapping->end()) {
            int instanceBitMax = alloc->getDbgInfo()->getInstanceIdBits() - 1;
            Out << "function [" << instanceBitMax << ":0] "
                << DEBUG_VERILOG_FUNC_NAME_INSTANCE_MAPPING << ";\n";
            Out << "\tinput [" << instanceBitMax << ":0] parentInstance;\n";
            Out << "\tbegin\n";
            Out << "\t\t" << DEBUG_VERILOG_FUNC_NAME_INSTANCE_MAPPING
                << " = 0;\n";
            for (map<int, int>::const_iterator it = instanceMapping->begin(),
                                               it_end = instanceMapping->end();
                 it != it_end; ++it) {
                if (it == instanceMapping->begin())
                    Out << "\t\tif ";
                else
                    Out << "\t\telse if ";
                Out << "(parentInstance == " << it->first << ")\n";
                Out << "\t\t\t" << DEBUG_VERILOG_FUNC_NAME_INSTANCE_MAPPING
                    << " = " << it->second << ";\n";
            }
            Out << "\tend\n";
            Out << "endfunction\n\n";
        }
    }

    for (RTLModule::const_signal_iterator i = rtl->param_begin(),
                                          e = rtl->param_end();
         i != e; ++i) {
        printDeclaration(*i);
        Out << "\n";
    }

	// added for voter's bit-wise comparison
	Out << "integer i;\n";

    if (rtl->param_begin() != rtl->param_end()) {
        Out << "\n";
    }

    for (RTLModule::const_signal_iterator i = rtl->port_begin(),
                                          e = rtl->port_end();
         i != e; ++i) {
        printDeclaration(*i);
        Out << "\n";
    }

    for (RTLModule::const_signal_iterator i = rtl->signals_begin(),
                                          e = rtl->signals_end();
         i != e; ++i) {
        printDeclaration(*i, (moduleName=="main_tb"));
        Out << "\n";
    }
}

bool VerilogWriter::isLocalMemSignal(const RTLSignal *signal) {
	for (Allocation::const_ram_iterator i = alloc->ram_begin();
	        i != alloc->ram_end(); ++i) {
		const RAM *R = *i;
		const char* portNames[6] = { "_address_a", "_address_b",
		                             "_write_enable_a", "_write_enable_b",
		                             "_in_a", "_in_b" };//,
		                             //"_out_a", "_out_b" };
        std::vector<std::string> ports(portNames, portNames+6);
        for (std::vector<std::string>::iterator p = ports.begin(), pe =
                ports.end(); p != pe; ++p) {
            std::string port = *p;
			std::string name = R->getName() + port;
			if (signal->getName() == name)
				return true;
		}
	}

	return false;
}

void VerilogWriter::printDeclaration(const RTLSignal *signal, bool testBench) {
    std::string type = signal->getType();
    if (!type.empty()) {
        if (type == "wire" &&
            (signal->getNumConditions() != 0 || signal->getNumDrivers() != 0)) {
            Out << "reg ";
        } else if (type == "output" && (signal->getNumConditions() != 0 ||
                                        signal->getNumDrivers() != 0)) {
            Out << "output reg ";
        } else {
            Out << type << " ";
        }
    }
    Out << signal->getWidth().str() << " ";
    Out << signal->getName();

    if (LEGUP_CONFIG->getParameterInt("CASE_FSM") &&
        signal->getName() == "cur_state")
        Out << ";\nreg " << signal->getWidth().str()
            << " next_state"; // need an extra variable if the FSM is
                              // implemented with case(..)
    if (!signal->getValue().empty()) {
        Out << " = ";
        printVerilogBitwidthPrefix(signal);
        Out << signal->getValue();
    }

    if (type == "wire" && rtl->synthesis_keep_signal(signal->getName())) {
        Out << "/* synthesis keep */";
    }

    Out << ";";

	// TMR signal declaration
    if (LEGUP_CONFIG->getParameterInt("TMR") && !testBench &&
	    !type.empty() && (type == "wire" || type == "reg")) {

		std::string hi = signal->getWidth().getHi();
		std::string lo = signal->getWidth().getLo();

    	Out << "\n";
		printTmrSignal(signal, "_r");

		int voterMode = LEGUP_CONFIG->getParameterInt("VOTER_MODE");
		bool isReg = signal->isReg();
		bool isBackward = signal->getBackward();
		// FIXME - For the local mem, always non-registered voters are inserted
		bool isRegVoter = (!isLocalMemSignal(signal) && 
		                   (signal->getName()!="cur_state" && voterMode==4));

		if ((signal->getName()=="cur_state") ||
		    (isLocalMemSignal(signal)) ||
		    ((voterMode==1) && isReg) ||
		    ((voterMode==2) && isReg && isBackward) ||
		    ((voterMode==3) && isBackward) ||
		    ((voterMode==4) && isReg && isBackward) ||
		    ((voterMode==5) && isBackward)) {
			printTmrSignal(signal, "_v");
			printTmrVoter(signal->getName(), lo, hi, isRegVoter);
		}

    	if (LEGUP_CONFIG->getParameterInt("CASE_FSM") &&
    	    signal->getName() == "cur_state") {
    	    Out << "reg " << signal->getWidth().str()
    	        << " next_state_r0,"
    	        << " next_state_r1,"
    	        << " next_state_r2 /*synthesis keep*/;\n";
    	    //Out << "reg " << signal->getWidth().str()
    	    //    << " next_state_v0,"
    	    //    << " next_state_v1,"
    	    //    << " next_state_v2 /*synthesis keep*/;\n";
    	    //Out << "reg [1:0] next_state_errid; /*synthesis noprune*/\n";

			//Out << "\n//Voter FSM Feedback\n";
			//printTmrVoter("next_state", lo, hi);
		}
    }
}

void VerilogWriter::printRTL(const RTLModule *rtl) {

    this->rtl = rtl;

    std::string name = rtl->getName();
    // Only print an include if verilog is already defined (custom)
    if (LEGUP_CONFIG->isCustomVerilog(name)) {
        return;
    }
    printModuleHeader();

    for (RTLModule::const_module_iterator i = rtl->instances_begin(),
                                          e = rtl->instances_end();
         i != e; ++i) {
        Out << "\n";
        printModuleInstance(Out, *i);
    }
    Out << "\n";

    if (LEGUP_CONFIG->getParameterInt("PRINT_FUNCTION_START_FINISH")) {
        Out << "always @(posedge start)\n"
            << "\t$display(\"Cycle: %d Function: " << name
            << " Start\", ($time-50)/20);\n"
            << "always@(posedge finish)\n"
            << "\t$display(\"Cycle: %d Function: " << name
            << " Finish\", ($time-50)/20);\n";
    }

    if (LEGUP_CONFIG->getParameterInt("LOCAL_RAMS")) {
        Out << "// Local Rams\n";

    	if (LEGUP_CONFIG->getParameterInt("TMR")) {
			for (int tmrIter=0; tmrIter<3; tmrIter++) {
				useReplicaNumberForAllVariables = true;
				currReplica = utostr(tmrIter);
        		for (RTLModule::const_ram_iterator i = rtl->local_ram_begin(),
        		                                   e = rtl->local_ram_end();
        		     i != e; ++i) {
        		    RAM *R = *i;
        		    Out << "\n";
        		    printRamInstance(R);
        		}
				useReplicaNumberForAllVariables = false;
			}
		} else {
        	for (RTLModule::const_ram_iterator i = rtl->local_ram_begin(),
        	                                   e = rtl->local_ram_end();
        	     i != e; ++i) {
        	    RAM *R = *i;
        	    Out << "\n";
        	    printRamInstance(R);
			}
        }
        Out << "\n";
    }

    Out << rtl->getPreamble() << "\n";

    // print unsynthesizable statements ($display, $finish, etc)
    printSignal(rtl->getUnsynthesizableSignal());

    // print signals
    for (RTLModule::const_signal_iterator i = rtl->signals_begin(),
                                          e = rtl->signals_end();
         i != e; ++i) {
    	if (LEGUP_CONFIG->getParameterInt("TMR")) {
			RTLSignal *s = *i;
			useReplicaNumberForAllVariables = true;
			for (int tmrIter=0; tmrIter<3; tmrIter++) {
				currReplica = utostr(tmrIter);
				printSignal(s);
			}
			useReplicaNumberForAllVariables = false;
		} else
        	printSignal(*i);
    }

	// print voters for feedback signals
	Out << "\n\n//voter\n";
	if (LEGUP_CONFIG->getParameterInt("TMR")) {
    	for (RTLModule::const_signal_iterator i = rtl->signals_begin(),
    	                                      e = rtl->signals_end();
    	     i != e; ++i) {
			if (isTmrSig(*i))
				printVoter(*i);
		}
	}

	// print output port (signal)
	Out << "\n\n//port drive\n";
    for (RTLModule::const_signal_iterator i = rtl->port_begin(),
                                          e = rtl->port_end();
         i != e; ++i) {
       	printSignal(*i);
    }

    Out << rtl->getBody() << "\n";

    Out << "endmodule "
        << "\n";
}

std::string VerilogWriter::bitsForSignalAndModuleName(const RTLSignal *sig,
                                                      std::string name) {

    PropagatingSignals *ps = alloc->getPropagatingSignals();
    std::string bits = "";

    std::vector<PropagatingSignal *> signals =
        ps->getPropagatingSignalsForFunctionNamed(name);

    if (!signals.empty()) {
        for (std::vector<PropagatingSignal *>::iterator it = signals.begin();
             it != signals.end(); ++it) {
            if ((*it)->getName() == sig->getName()) {
                bits = (*it)->getSignal()->getWidth().str();
            }
        }
    }
    return bits;
}

void VerilogWriter::printModuleInstance(std::stringstream &Out,
                                        const RTLModule *mod) {
    Out << mod->getBody() << "\n";
    Out << mod->getName() << " " << mod->getInstName() << " ("
        << "\n";
    for (RTLModule::const_signal_iterator i = mod->port_begin(),
                                          e = mod->port_end();
         i != e; ++i) {

        const RTLSignal *s = *i;
        unsigned sigWidth = s->getWidth().numBits(rtl, alloc);
        const RTLSignal *d = s->getDriver(0);
        assert(d && "Connection doesn't exist!\n");
        Out << "\t." << s->getName() << " (";
        printValue(d, sigWidth, true);
        Out << bitsForSignalAndModuleName(d, mod->getName());
        Out << ")";
        if (i != mod->port_end() - 1) {
            Out << ",";
        }
        Out << "\n";
    }
    Out << ");"
        << "\n\n";

    for (RTLModule::const_signal_iterator i = mod->param_begin(),
                                          e = mod->param_end();
         i != e; ++i) {
        if (i == mod->param_begin()) {
            Out << "defparam"
                << "\n\t";
        } else {
            Out << ",\n\t";
        }
        Out << mod->getInstName() << "." << (*i)->getName() << " = "
            << (*i)->getValue();
        if (i == mod->param_end() - 1) {
            Out << ";\n";
        }
    }
}

// recursively check if a signal is driven by a constant ie.
// a = 0;
// note, this would also be a constant:
// main_legup_memset_4_exit_7 = `TAG_main_0_previous_a + 4 * 32'd14;
bool VerilogWriter::isConst(const RTLSignal *sig) {
    if (sig->isConst())
        return true;
    if (sig->isOp()) {
        const RTLOp *op = (const RTLOp *)sig;
        for (unsigned i = 0; i < op->getNumOperands(); i++) {
            if (!isConst(op->getOperand(i))) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

// need this otherwise always block doesn't always trigger ie.
// always(*)
//    a = 0;
// will never trigger and signal 'a' will always be 'X'
void VerilogWriter::printAlwaysTrigger(const RTLSignal *signal,
                                       const RTLSignal *driver) {
	std::string destName = signal->getName();
	if(isTmrSig(signal))
		destName += "_r" + currReplica;

    if (isConst(driver)) {
        Out << "if (reset) begin " << destName << " = 0; end\n";
    }
}

void
VerilogWriter::caseConditions(const RTLSignal *condition,
                              std::vector<const RTLSignal *> &clausesToKeep) {

    if (!condition->isOp())
        return;

    const RTLOp *op = (const RTLOp *)condition;

    if (op->getOperand(0)->isOp())
        caseConditions(op->getOperand(0), clausesToKeep);

    if (op->getOperand(1)->isOp())
        caseConditions(op->getOperand(1), clausesToKeep);

    if (!op->getOperand(0)->isOp() && !op->getOperand(1)->isOp()) {

        if ((op->getOperand(0)->getName() != "cur_state") &&
            (op->getOperand(0)->getName() != "memory_controller_waitrequest")) {
            clausesToKeep.push_back(condition);
        }
    }
}

void VerilogWriter::getStateName(const RTLSignal *condition,
                                 std::string &param) {
    if (!condition->isOp())
        return;

    const RTLOp *op = (const RTLOp *)condition;

    if (op->getOperand(0)->isOp())
        getStateName(op->getOperand(0), param);

    if (op->getOperand(1)->isOp())
        getStateName(op->getOperand(1), param);

    if (!op->getOperand(0)->isOp() && !op->getOperand(1)->isOp()) {

        if (op->getOperand(0)->getName() == "cur_state")
            param = op->getOperand(1)->getName(); // extract the FSM state param
    }
}

void VerilogWriter::printCaseFSM(const RTLSignal *signal,
                                 std::string assignOp) {

    std::map<std::string, std::vector<int>>
        FSMCases; // build up an association between state names and conditions

    unsigned numConditions = signal->getNumConditions();
    unsigned sigWidth = signal->getWidth().numBits(rtl, alloc);
    assert(numConditions > 0);

    for (unsigned i = 0; i < numConditions; ++i) {
        const RTLSignal *driver = signal->getDriver(i);
        const RTLSignal *condition = signal->getCondition(i);
        assert(driver);

        std::string param;
        getStateName(condition, param);
        FSMCases[param].push_back(i);
    }

	std::string curState = "cur_state";
	std::string nextState = "next_state";
	if (isTmrSig(signal)) {
		curState += "_r" + currReplica;
		nextState += "_r" + currReplica;
	}

    // first we print out the always block for the sequential logic
    //Out << "if (reset == 1'b1)\n";
    //Out << "\t" << curState
	//    << " <= LEGUP_0;\n"; // janders DANGER: this relies on the
    //                         // first state of the FSM having a
    //                         // specific name LEGUP_0
    if (alloc->getPropagatingSignals()->functionUsesMemory(rtl->getName())) {
        Out << "\telse if (memory_controller_waitrequest == 1'd1)\n";
        Out << "\t\t" << curState
		    << " <= " << curState << ";\n";
    }
    Out << "\telse\n";
    Out << "\t\t" << curState
	    << " <= " << nextState << ";\n";
    Out << "end\n\n";

    Out << "always @(*)\n"; // now this is the always block for the "next state"
                            // logic -- completely combinational
    Out << "begin\n";
    Out << nextState << " = " << curState << ";\n"; // needed to avoid "inferred
	                                                // latches" in synthesis
    if (!LEGUP_CONFIG->getParameterInt("CASEX"))
        Out << "case(";
    else
        Out << "casex(";
    printValue(signal);
    Out << ")  // synthesis parallel_case  \n"; // janders -- use the ALTR
                                                // pragma

    for (std::map<std::string, std::vector<int>>::iterator i = FSMCases.begin();
         i != FSMCases.end(); ++i) {

        if (i->first == "")
            continue;

        if (!LEGUP_CONFIG->getParameterInt("CASEX"))
            Out << i->first << ":"
                << "\n";
        else
            Out << i->first << "_X"
                << ":"
                << "\n";

        int count = 0;

        if (i->second.size() == 2) {
            // unconditional transition in the state diagram
            // how do we know this?  because any state has "at a minimum" 2
            // conditions:
            // one when the "wait" signal is asserted (1) and one when the
            // "wait" signal is 0.
            // we handled the case for the "wait" signal being 1 in the
            // sequential always block above.
            for (std::vector<int>::iterator i2 = i->second.begin();
                 i2 != i->second.end(); ++i2) {
                if ((signal->getDriver(*i2)->getName() == i->first) && !count) {
                    count++;
                    // janders -- the reason we are checking !count here is that
                    // some circuits actually have states where they get "stuck"
                    // (infinite loop in HW).
                    // e.g. dhrystone has a state where, when we enter it, we
                    // continue
                    // to spin on that state, even if the wait request for the
                    // memory controller
                    // is logic-0
                    continue;
                } else {
                    Out << "\t\t" << nextState << " = ";
                   	printValue(signal->getDriver(*i2), sigWidth);
                    Out << ";\n";
                    break;
                }
            }
        } else {
            for (std::vector<int>::iterator i2 = i->second.begin();
                 i2 != i->second.end(); ++i2) {

                if (signal->getDriver(*i2)->getName() == i->first)
                    continue; // handled in the sequential always block above

                std::vector<const RTLSignal *> clauses;
                const RTLSignal *condition = signal->getCondition(*i2);
                caseConditions(condition, clauses); // extracts the conditions
                                                    // we care about (ignoring
                                                    // ones that are handled in
                                                    // the sequential always
                                                    // block above)

                if (!count)
                    Out << "\tif (";
                else
                    Out << "\telse if (";

                for (std::vector<const RTLSignal *>::iterator i3 =
                         clauses.begin();
                     i3 != clauses.end();) {
                    printValue(*i3);
                    ++i3;
                    if (i3 != clauses.end())
                        Out << " && ";
                }

                Out << ")\n";
                Out << "\t\t" << nextState << " = ";
                printValue(signal->getDriver(*i2), sigWidth);
                Out << ";\n";
                count++;
            }
        }
    }
    Out << "default:\n\t" << nextState << " = " << curState << ";\n";
    Out << "endcase\n\n";
}

// NC changes...
// New func added
// this method is only used to generate outputs to use for debugging purpose
void
VerilogWriter::printSignalConditionForInstruction(const RTLSignal *signal,
                                                  const Instruction *Instr) {
    std::string assignOp = "<=";

    if (signal != rtl->getUnsynthesizableSignal() && !signal->isReg()) {
        assignOp = "=";
    }

    for (unsigned i = 0; i < signal->getNumConditions(); ++i) {

        const Instruction *I = signal->getInst(i);
        if (I != Instr)
            continue;

        printIndividualCondition(signal, i, assignOp, false);
    }
}

// NC changes... new function added to print only one individual condition for a
// signal
void VerilogWriter::printIndividualCondition(const RTLSignal *signal,
                                             int conditionNum,
                                             std::string assignOp,
                                             bool printCmnts) {

    bool reg = signal->isReg();
    unsigned sigWidth = signal->getWidth().numBits(rtl, alloc);
    int numConditions = signal->getNumConditions();

    const RTLSignal *driver = signal->getDriver(conditionNum);
    const RTLSignal *condition = signal->getCondition(conditionNum);
    const std::string driverBits = driver->getDriverBits();
    assert(driver);
    const Instruction *I = signal->getInst(conditionNum);

    if (printCmnts)
        printComments(I, "\t");

    // don't "else" if the signal is not synthesizable, if it is a reg or
    // if it has a default driver
    bool no_else = (signal == rtl->getUnsynthesizableSignal()) || reg ||
                   signal->getDefaultDriver();

    bool ifcond = false;
    if (no_else) {
		if (reg)
			Out << "\telse if("; //'if' reset 'else if'
		else
        	Out << "\tif (";
       	printValue(condition);
        Out << ") ";
        ifcond = true;
    } else { // add "else"s
        if (numConditions > 1) {
            if (numConditions > 1 && conditionNum == numConditions - 1) {
                Out << "\telse ";
                Out << "/* if (";
                printValue(condition);
                Out << ") */ ";
            } else {
                if (conditionNum > 0) {
                    Out << "\telse if (";
                } else {
					if (reg)
						Out << "\telse if("; //'if' reset 'else if'
					else
                    	Out << "\tif (";
                }
                printValue(condition);
                Out << ") ";
            }
            ifcond = true;
        } else {
            printAlwaysTrigger(signal, driver);
        }
    }

    if (ifcond)
        Out << "begin\n";
    Out << "\t\t";

	std::string destName = signal->getName();
	if(isTmrSig(signal))
		destName += "_r" + currReplica;

    if (signal != rtl->getUnsynthesizableSignal()) {
        Out << destName << driverBits << " " << assignOp << " ";
    }
    printValue(driver, sigWidth);//, signal->driveFromVoter());
    Out << ";\n";

    // debugging
    // assert that we never assign an X to a variable
    // Note: need === instead of ==
    // xor (^) catches cases where the X is only in part of the signal
    if (!LEGUP_CONFIG->getParameterInt("VSIM_NO_ASSERT") && reg &&
        signal->getCheckXs()) {
        // only check after the module has started
        // the start signal is high for one cycle then goes low
        Out << "\t\tif (start == 1'b0 && ^(";
        // Out << "\t\tif (reset == 1'b0 && ^(";
        printValue(driver, sigWidth);
        Out << ") === 1'bX) begin "
            << "$display (\"ERROR: Right hand side is 'X'. Assigned to "
            << destName << "\");"
            << " $finish; end\n";
    }

    if (ifcond)
        Out << "\tend\n";
}

//
void VerilogWriter::printConditions(const RTLSignal *signal,
                                    std::string assignOp) {
    if (LEGUP_CONFIG->getParameterInt("CASE_FSM") &&
        (signal->getName() == "cur_state")) { // janders handling for the FSM
        printCaseFSM(signal, assignOp);
        return;
    }

    unsigned numConditions = signal->getNumConditions();
    assert(numConditions > 0);

    for (unsigned i = 0; i < numConditions; ++i) {
        printIndividualCondition(signal, i, assignOp, true);
    }
}

void VerilogWriter::printSignal(const RTLSignal *signal) {
    if (signal->getNumDrivers() == 0)
        return;

    bool reg = signal->isReg();
    unsigned sigWidth = signal->getWidth().numBits(rtl, alloc);

	std::string destName = signal->getName();
	if(isTmrSig(signal))
		destName += "_r" + currReplica;

    // use non-blocking assignment for  logic
    std::string assignOp = "<=";
    if (signal == rtl->getUnsynthesizableSignal()) {
        Out << "/* Unsynthesizable Statements */\n";
        Out << "always @(posedge clk)\n";
        Out << "\tif (!memory_controller_waitrequest) begin\n";
    } else {
        if (reg) {
           	Out << "always @(posedge clk) begin\n";
           	Out << "\tif (reset)\n";
           	Out << "\t\t" << destName << " " << assignOp;
			if (signal->getName()=="cur_state")
				Out << " LEGUP_0;\n";
			else
				Out << " 0;\n";
            const RTLSignal *defaultDriver = signal->getDefaultDriver();
            if (defaultDriver) {
                // ie. memory_controller_enable as a default of 0
                Out << "\telse\n\t\t" << destName << " " << assignOp << " ";
                printValue(defaultDriver, sigWidth);
                Out << ";\n";
            }
        } else {
            Out << "always @(*) begin\n";
            // use blocking assignment for combinational logic
            assignOp = "=";
            const RTLSignal *defaultDriver = signal->getDefaultDriver();
            if (defaultDriver) {
                // ie. memory_controller_enable as a default of 0
                Out << "\t" << destName << " " << assignOp << " ";
                printValue(defaultDriver, sigWidth);
                Out << ";\n";
            }
        }
    }

    // there is only a single driver
    if (signal->getNumConditions() == 0) {

        printComments(signal->getInst(0));

        const RTLSignal *driver = signal->getDriver(0);
        assert(driver);
		if (reg)
        	Out << "\telse\n\t\t" << destName << " " << assignOp << " ";
		else
        	Out << "\t" << destName << " " << assignOp << " ";
        printValue(driver, sigWidth);//, signal->driveFromVoter());
        Out << ";\n";
        printAlwaysTrigger(signal, driver);
    } else {
        printConditions(signal, assignOp);
    }

    Out << "end\n";
}

void VerilogWriter::printTmrSignal(const RTLSignal *sig, std::string postfix) {
	std::string type = sig->getType();
	if (type == "wire" &&
	    (sig->getNumConditions() != 0 || sig->getNumDrivers() != 0 || postfix == "_v")) {
	    Out << "reg ";
	} else {
	    Out << type << " ";
	}

   	Out << sig->getWidth().str() << " ";
   	Out << sig->getName() << postfix << "0, ";
   	Out << sig->getName() << postfix << "1, ";
   	Out << sig->getName() << postfix << "2 ";
	//if ((postfix=="_r" && type=="reg") || 
	//    (LEGUP_CONFIG->getParameterInt("TMR") &&
	//     LEGUP_CONFIG->getParameterInt("VOTER_MODE")==1 &&
	//     sig->getBackward())) //sig->isPhi()))
	//	Out << "/*synthesis preserve*/;\n";
	//else 
		Out << "/*synthesis keep*/;\n";

	//if (postfix=="_r") {
	//	Out << "reg [1:0] " << sig->getName() << "_errid"
	//	    << " /*synthesis noprune*/;\n";
	//}

	//FIXME - module boundary is not triplicated
	// boradcasting module's finish signal
	if (postfix=="_r" && type=="wire" && requireBroadcasting(sig)) {
		for(int i=0; i<3; i++) {
			Out << "assign " << sig->getName() << "_r" << i
			    << " = " << sig->getName() << ";\n";
		}
	}
}

bool VerilogWriter::requireBroadcasting(const RTLSignal *signal) {
    for (RTLModule::const_module_iterator i = rtl->instances_begin(),
                                          e = rtl->instances_end();
         i != e; ++i) {
		const RTLModule *mod = *i;
		std::string moduleFinishName = mod->getName() + "_finish";
		if(moduleFinishName == signal->getName())
			return true;
    }

	return false;
}

void VerilogWriter::printVoter(const RTLSignal *signal) {
    if (signal->getNumDrivers() == 0)
        return;

    std::string hi = signal->getWidth().getHi();
    std::string lo = signal->getWidth().getLo();
	std::string sigName = signal->getName();

	bool bitWidthOne = (hi=="");

	std::string r0 = bitWidthOne? sigName + "_r0" : sigName + "_r0[i]";
	std::string r1 = bitWidthOne? sigName + "_r1" : sigName + "_r1[i]";
	std::string r2 = bitWidthOne? sigName + "_r2" : sigName + "_r2[i]";
	std::string defValue = (sigName=="cur_state")? "LEGUP_0" : "0";
	std::string v = bitWidthOne? sigName : sigName + "[i]";

	Out << "always @(*) begin\n";
	if (!bitWidthOne)
		Out << "\tfor(i=" << lo << "; i<=" << hi << "; i=i+1) begin\n";
	Out << "\t" << v << "\n"
	    << "\t\t= (" << r0 << "==" << r1 << ")? " << r0 << "\n"
		<< "\t\t: " << r2 << ";\n";
	//    << "\t\t: (" << r0 << "==" << r2 << ")? " << r0 << "\n"
	//    << "\t\t: (" << r1 << "==" << r2 << ")? " << r1 << "\n"
	//	<< "\t\t: " << defValue << ";\n";
	if (!bitWidthOne)
		Out << "\tend\n";
	Out << "end\n";

	//Out << "always @(posedge clk or posedge reset) begin\n";
	//Out << "\tif (reset) begin\n";
	//Out << "\t\t" << sigName << "_errid" << " <= 0;\n";
	//Out << "\tend else begin\n";
	//Out << "\t\tif ((" << sigName << "_r0" << "==" << sigName << "_r1" << ") &&\n"
	//    << "\t\t    (" << sigName << "_r0" << "==" << sigName << "_r2" << ") &&\n"
	//    << "\t\t    (" << sigName << "_r1" << "==" << sigName << "_r2" << ")) "
	//    << sigName << "_errid <= " << "2'b11;\n";
	//Out << "\t\telse if (" << sigName << "_r0" << "!=" << sigName << "_r1" << ") "
	//    << sigName << "_errid <= " << "2'b00;\n";
	//Out << "\t\telse if (" << sigName << "_r0" << "!=" << sigName << "_r1" << ") "
	//    << sigName << "_errid <= " << "2'b01;\n";
	//Out << "\t\telse "//if (" << sigName << "_r0" << "!=" << sigName << "_r1" << ") "
	//    << sigName << "_errid <= " << "2'b10;\n";
	//Out << "\tend\n";
	//Out << "end\n";
}

void VerilogWriter::printTmrVoter(std::string sigName, std::string lo, std::string hi, bool isRegVoter) {
	bool bitWidthOne = (hi=="");

	std::string r0 = bitWidthOne? sigName + "_r0" : sigName + "_r0[i]";
	std::string r1 = bitWidthOne? sigName + "_r1" : sigName + "_r1[i]";
	std::string r2 = bitWidthOne? sigName + "_r2" : sigName + "_r2[i]";
	std::string v0 = bitWidthOne? sigName + "_v0" : sigName + "_v0[i]";
	std::string v1 = bitWidthOne? sigName + "_v1" : sigName + "_v1[i]";
	std::string v2 = bitWidthOne? sigName + "_v2" : sigName + "_v2[i]";
	std::string defValue =
	    (sigName=="cur_state" || sigName=="next_state")? "LEGUP_0" : "0";

	if (isRegVoter)
		Out << "always @(posedge clk) begin\n";
	else
		Out << "always @(*) begin\n";
	if (!bitWidthOne)
		Out << "\tfor(i=" << lo << "; i<=" << hi << "; i=i+1) begin\n";
	Out << "\t\t" << v0 << " = (" << r0 << "==" << r1 << ")? " << r0 << "\n"
	    << "\t\t\t: " << r2 << ";\n";
	Out << "\t\t" << v1 << " = (" << r0 << "==" << r1 << ")? " << r0 << "\n"
	    << "\t\t\t: " << r2 << ";\n";
	Out << "\t\t" << v2 << " = (" << r0 << "==" << r1 << ")? " << r0 << "\n"
	    << "\t\t\t: " << r2 << ";\n";
	if (!bitWidthOne)
		Out << "\tend\n";
	Out << "end\n";

	//Out << "always @(posedge clk or posedge reset) begin\n";
	//Out << "\tif (reset) begin\n";
	//Out << "\t\t" << sigName << "_errid" << " <= 0;\n";
	//Out << "\tend else begin\n";
	//Out << "\t\tif ((" << sigName << "_r0" << "==" << sigName << "_r1" << ") &&\n"
	//    << "\t\t    (" << sigName << "_r0" << "==" << sigName << "_r2" << ") &&\n"
	//    << "\t\t    (" << sigName << "_r1" << "==" << sigName << "_r2" << ")) "
	//    << sigName << "_errid <= " << "2'b11;\n";
	//Out << "\t\telse if (" << sigName << "_r0" << "!=" << sigName << "_r1" << ") "
	//    << sigName << "_errid <= " << "2'b00;\n";
	//Out << "\t\telse if (" << sigName << "_r0" << "!=" << sigName << "_r1" << ") "
	//    << sigName << "_errid <= " << "2'b01;\n";
	//Out << "\t\telse "//if (" << sigName << "_r0" << "!=" << sigName << "_r1" << ") "
	//    << sigName << "_errid <= " << "2'b10;\n";
	//Out << "\tend\n";
	//Out << "end\n";
}

void VerilogWriter::printComments(const Instruction *I, std::string prefix) {
    if (!I)
        return;
    // static std::map<const Instruction *I, std::string> cache;
    const BasicBlock *b = I->getParent();
    if (b) {
        Out << prefix << "/* " << b->getParent()->getName().str() << ": "
            << getLabel(b) << "*/\n";
    }

    Out << prefix << "/* " << getValueStr(I);

    // for pipelined instructions print metadata
    if (!getMetadataStr(I, "legup.pipeline.start_time").empty()) {
        int start_time = getMetadataInt(I, "legup.pipeline.start_time");
        int avail_time = getMetadataInt(I, "legup.pipeline.avail_time");
        int stage = getMetadataInt(I, "legup.pipeline.stage");
        int II = getPipelineII(I->getParent());

        Out << "\n" << prefix << "start_time: " << start_time
            << " avail_time: " << avail_time << " stage: " << stage
            << " II: " << II << " start_ii_state = " << start_time << " % "
            << II << " = " << (start_time % II)
            << " avail_ii_state = " << avail_time << " % " << II << " = "
            << (avail_time % II);
    }

    Out << "*/\n";
}

void VerilogWriter::printVerilogOpcode(RTLOp::Opcode opcode) {
    switch (opcode) {
    case RTLOp::Add:
        Out << " + ";
        break;
    case RTLOp::Sub:
        Out << " - ";
        break;
    case RTLOp::Mul:
        Out << " * ";
        break;
    case RTLOp::Rem:
        Out << " % ";
        break;
    case RTLOp::Div:
        Out << " / ";
        break;
    case RTLOp::And:
        Out << " & ";
        break;
    case RTLOp::LAnd:
        Out << " && ";
        break;
    case RTLOp::Or:
        Out << " | ";
        break;
    case RTLOp::Xor:
        Out << " ^ ";
        break;
    case RTLOp::Shl:
        Out << " <<< ";
        break;
    case RTLOp::Shr:
        Out << " >>> ";
        break;
    case RTLOp::EQ:
        Out << " == ";
        break;
    case RTLOp::NE:
        Out << " != ";
        break;
    case RTLOp::LT:
        Out << " < ";
        break;
    case RTLOp::LE:
        Out << " <= ";
        break;
    case RTLOp::GT:
        Out << " > ";
        break;
    case RTLOp::GE:
        Out << " >= ";
        break;
    /*case RTLOp::OEQ:  Out << " == ";  break;
    case RTLOp::UNE:  Out << " != ";  break;
    case RTLOp::OLT:  Out << " < ";   break;
    case RTLOp::OLE:  Out << " <= ";  break;
    case RTLOp::OGT:  Out << " > ";   break;
    case RTLOp::OGE:  Out << " >= ";  break;*/
    default:
        llvm_unreachable("Invalid operator type!");
    }
}

// print bitwidth prefix. ie. 6'd
void VerilogWriter::printVerilogBitwidthPrefix(const RTLSignal *sig) {
    // add bitwidth
    if (isNumeric(sig->getValue())) {
        rtl->verifyBitwidth(sig, alloc);
        unsigned bits = sig->getWidth().numBits(rtl, alloc);
        // numbers are 32 bits wide by default
        if (bits != 32)
            Out << bits << "'d";
    }
}

// printValue with optional w and zeroExtend parameters
// printValue(sig) calls printValue(sig,0,false)
// printValue(sig,w) calls printvalue(sig,w,false)
// w is the width of the driven signal.  This is useful to figure out how much
// we need to sign or
// zero extend the driver.
// zeroExtend is a flag that says whether we need to manually zeroExtend
// signals.
void VerilogWriter::printValue(const RTLSignal *sig, unsigned w,
                               bool zeroExtend) {

    assert(sig && "sig is NULL");
    if (sig->isOp()) {
        printVerilogOperator((const RTLOp *)sig, w);
    } else if (sig->getType() == "parameter") {
        Out << sig->getName();
    } else {
        if (sig->getValue().empty()) {
            if (LEGUP_CONFIG->getParameterInt("MB_MINIMIZE_HW")) {
                printValueMinBW(sig, w, zeroExtend);
            } else {
                // normal variable ie. cur_state
                Out << sig->getName();
				if (useReplicaNumberForAllVariables && isTmrSig(sig)) {
					if (sig->driveFromVoter())
						Out << "_v" << currReplica;
					else
						Out << "_r" << currReplica;
				}
            }
        } else {
            // constant ie. 4'd1
            printVerilogBitwidthPrefix(sig);
            Out << sig->getValue();
        }
    }
}

void VerilogWriter::printValueMinBW(const RTLSignal *sig, unsigned w,
                                    bool zeroExtend) {

    //    unsigned w2 = sig->getNativeWidth().numBits(rtl,alloc);
    //    if(w2>=2) {
    //        w=w2;
    // w = min(w,w2);
    /*        if(w>w2 && sig->getSigned()) {
                errs() << "w: "<<utostr(w)<<" w2: "<<utostr(w2)
                       << " for signal:
       "<<sig->getName()<<sig->getValue()<<"\n";
            }*/
    //    }

    bool USE_LSB = LEGUP_CONFIG->getParameterInt("MB_MINIMIZE_LSB");
    std::string hi = sig->getWidth().getHi();
    std::string lo = sig->getWidth().getLo();
    unsigned extendFrom = 0, LSB = 0;
    if (USE_LSB) {
        if (hi != "")
            stringstream(hi) >> extendFrom;
        stringstream(lo) >> LSB;
    } else
        extendFrom = sig->getWidth().numBits(rtl, alloc) - 1;
    bool isSigned = sig->getWidth().getSigned();
    // normal variable ie. cur_state
    unsigned minW = extendFrom + 1;
    if (w > 0 && (LSB > 0 || (minW < w))) {
        // if minW is 1, signal needs to printed something like this:
        // "{32{sig_name}}" if the
        // signal is signed
        if (minW == 1) {
            if (isSigned)
                Out << "{" << utostr(w) << "{" << sig->getName() << "}}";
            else if (zeroExtend)
                Out << "{" << utostr(w - 1) << "'d0," << sig->getName() << "}";
            else
                Out << sig->getName();
        } else {
            // Opening bracket needed in all cases
            Out << "{";
            // The first part of a signal with extendFrom==19 should be printed
            // like this:
            //"{12{sig_name[19]}},", if the signal is signed
            if (minW < w) {
                if (isSigned)
                    Out << "{" << utostr(w - minW) << "{" << sig->getName()
                        << "[" << utostr(extendFrom) << "]"
                        << "}},";
                else if (zeroExtend)
                    Out << utostr(w - minW) << "'d0,";
            }
            // The second part of the same signal can be like this if LSB==0:
            // sig_name
            if (LSB == 0)
                Out << sig->getName();
            // if LSB>0, the second part of the signal should be like this:
            // sig_name[19:LSB]
            // if LSB>0, the third part of the signal should be zeroes
            else {
                Out << sig->getName() << "[" << utostr(extendFrom) << ":"
                    << utostr(LSB) << "]," << utostr(LSB) << "'d0";
            }
            Out << "}";
        }
    }

    else {
        Out << sig->getName();
    }
}

void VerilogWriter::printDisplayString(std::string display_string) {
    // Make sure \n gets represented properly
    replaceAll(display_string, "\n", "\\n");

    // ensure escaped % does not look like %x, %p, %u, etc...
    replaceAll(display_string, "%%", "%?");

    replaceAll(display_string, "%016llx", "%0x");

    // convert printf format specifiers to $display format
    // specifiers
    // %0x prints the minimum hex characters - seems to match C better
    replaceAll(display_string, "%x", "%0x");
    replaceAll(display_string, "%p", "%0x");
    replaceAll(display_string, "%u", "%d");
    // long long
    replaceAll(display_string, "%lld", "%d");
    replaceAll(display_string, "%llu", "%d");
    replaceAll(display_string, "%llx", "%0x");
    // long
    replaceAll(display_string, "%ld", "%d");
    replaceAll(display_string, "%lu", "%d");
    replaceAll(display_string, "%lx", "%0x");
    // short
    replaceAll(display_string, "%hd", "%d");
    replaceAll(display_string, "%hu", "%d");
    replaceAll(display_string, "%hx", "%0x");
    // very short
    replaceAll(display_string, "%hhd", "%d");
    replaceAll(display_string, "%hhu", "%d");
    replaceAll(display_string, "%hhx", "%0x");

    // go back to escaped %
    replaceAll(display_string, "%?", "%%");

    // Print.
    Out << display_string;
}

std::vector<bool>
VerilogWriter::parseDisplayString(const std::string &display_string) {
    std::vector<bool> is_unsigned_vec;
    for (size_t pos = display_string.find('%');
         pos < display_string.length() && pos != string::npos;
         pos = display_string.find('%', pos + 2 /* skip the char after % */)) {
        assert(display_string.at(pos) == '%');
        // '%' sign cannot be the last char in the string, except "%%" case.
        assert(pos < display_string.length() - 1);
        // Ignore "%%" since it's escaped percentage sign.
        if (display_string.at(pos + 1) == '%')
            continue;
        // Check if the placeholder is '%u'
        is_unsigned_vec.push_back(display_string.at(pos + 1) == 'u');
    }
    return is_unsigned_vec;
}

void VerilogWriter::printDisplay(const RTLOp *op) {
    assert(op->getOpcode() == RTLOp::Write ||
           op->getOpcode() == RTLOp::Display);
    if (LEGUP_CONFIG->getParameterInt("PRINTF_CYCLES")) {
        Out << "$write(\"Cycle: %d Time: %d    \", ($time-50)/20, $time);\n";
    }
    if (op->getOpcode() == RTLOp::Write) {
        Out << "$write(";
    } else {
        Out << "$display(";
    }
    std::string display_string = op->getOperand(0)->getValue();
    // Parse the unsigned placeholders.
    auto is_unsigned_vec = parseDisplayString(display_string);
    assert(is_unsigned_vec.size() == op->getNumOperands() - 1);
    // Print the display string, and with placeholders replaced to Verilog.
    printDisplayString(display_string);

    // Print the variables.
    for (unsigned i = 1; i < op->getNumOperands(); i++) {
        Out << ", ";
        if (!is_unsigned_vec[i-1]) {
            Out << "$signed(";
        }
        if (op->getOpcode() == RTLOp::Write && i == 1) {
            printValue(op->getOperand(i),
                       op->getOperand(i)->getWidth().numNativeBits(rtl, alloc));
        } else {
            printValue(op->getOperand(i));
        }
        if (!is_unsigned_vec[i-1]) {
            Out << ")";
        }
    }
    Out << ")";

    if (op->getNumOperands() > 1 && !LEGUP_CONFIG->isXilinxBoard()) {
        Out << ";\n";
        Out << "\t\t// to fix quartus warning\n";
        // avoid getting a warning from quartus that we
        // never read the value being printed
        for (unsigned i = 1; i < op->getNumOperands(); i++) {
            // Out << "if (^reset !== 1'bX && ^(";
            Out << "\t\tif (reset == 1'b0 && ^(";
            printValue(op->getOperand(i));
            Out << ") === 1'bX) "
                << "finish <= 0";
            if (i != op->getNumOperands() - 1)
                Out << ";\n";
        }
    }
}

void VerilogWriter::printVerilogOperator(const RTLOp *op, unsigned w) {
    assert(op);

    if (op->getOpcode() == RTLOp::Finish) {
        Out << "$finish";
    } else if (op->getOpcode() == RTLOp::Write ||
               op->getOpcode() == RTLOp::Display) {
        printDisplay(op);
    } else {
        switch (op->getNumOperands()) {
        case 3:
            assert(op->getOpcode() == RTLOp::Sel);
            Out << "(";
            printValue(op->getOperand(0), w);
            Out << " ? ";
            printValue(op->getOperand(1), w);
            Out << " : ";
            printValue(op->getOperand(2), w);
            Out << ")";
            break;
        case 2:
            if (op->getOpcode() == RTLOp::Concat) {
                Out << "{";
                unsigned op0_width =
                    op->getOperand(0)->getWidth().numNativeBits(rtl, alloc);
                printValue(op->getOperand(0), op0_width, true);
                Out << ",";
                unsigned op1_width =
                    op->getOperand(1)->getWidth().numNativeBits(rtl, alloc);
                printValue(op->getOperand(1), op1_width, true);
                Out << "}";
            } else {
                Out << "(";
                printValue(op->getOperand(0), w);
                printVerilogOpcode(op->getOpcode());
                printValue(op->getOperand(1), w);
                Out << ")";
            }
            break;
        case 1:
            if (op->getOpcode() == RTLOp::Not) {
                Out << "~(";
                printValue(op->getOperand(0), w);
                Out << ")";
            } else if (op->getOpcode() == RTLOp::SExt) {
                Out << "$signed(";
                const RTLSignal *op0 = op->getOperand(0);
                // The left most bit of the signal will count as a sign bit.
                // If this is an unsigned signal, we need to pad it with a zero
                // Typically, when setting the zeroExtend flag to true, this
                // will happen
                // but with a signed comparison, since the output width is one,
                // printValue
                // won't know by how much to zeroExtend, so we have to set w
                // manually to be
                // one more than signal width
                bool op0IsSigned = op0->getWidth().getSigned();
                unsigned nativeBits = op0->getWidth().numNativeBits(rtl, alloc);
                unsigned bits = op0->getWidth().numBits(rtl, alloc);
                if (op0->getWidth().isNewStyleRTLWidth()) {
                    nativeBits = op0->getWidth().numNativeBits(rtl, alloc);
                }
                if (op0IsSigned && nativeBits > bits) {
                    printValue(op->getOperand(0), nativeBits, true);
                } else
                    printValue(op->getOperand(0), w);
                Out << ")";
            } else if (op->getOpcode() == RTLOp::ZExt) {
                printValue(op->getOperand(0), w);
            } else {
                assert(op->getOpcode() == RTLOp::Trunc);
                // ie. memory_controller_out[31:0]
                printValue(op->getOperand(0), w);
                // don't truncate a constant
                if (!op->getOperand(0)->isConst()) {
                    Out << op->getWidth().str();
                }
            }
            break;
        default:
            llvm_unreachable("Invalid RTLOp!");
        }
    }
}

void VerilogWriter::printDbgStateMuxer() {
    Function *fpMain = alloc->getModule()->getFunction("main");
    GenerateRTL *main = alloc->getGenerateRTL(fpMain);

    Out << "dbgStateMuxer dbgStateMuxer_inst (\n";

    vector<RTLDebugPort *> *ports = main->getDbgStatePorts();
    Out << "\t." << DEBUG_SIGNAL_NAME_ACTIVE_INST << "("
        << DEBUG_SIGNAL_NAME_ACTIVE_INST << "),\n";
    Out << "\t." << DEBUG_SIGNAL_NAME_CURRENT_STATE << "("
        << DEBUG_SIGNAL_NAME_CURRENT_STATE << ")";
    for (vector<RTLDebugPort *>::iterator port_it = ports->begin(),
                                          port_end = ports->end();
         port_it != port_end; ++port_it) {
        RTLDebugPort *port = *port_it;
        Out << ",\n\t." << port->getSignal()->getName() << "("
            << port->getSignal()->getName() << ")";
    }
    Out << "\n);\n\n";
}

void VerilogWriter::printTraceSchedulerInstance() {
    Function *fpMain = alloc->getModule()->getFunction("main");
    GenerateRTL *main = alloc->getGenerateRTL(fpMain);

    Out << "traceScheduler traceScheduler_inst (\n";
    Out << "\t." << DEBUG_SIGNAL_NAME_ACTIVE_INST << "("
        << DEBUG_SIGNAL_NAME_ACTIVE_INST << "_r)";

    if (alloc->getDbgInfo()->getOptionPreserveOneHot()) {
        vector<RTLDebugPort *> *ports = main->getDbgStatePorts();
        for (vector<RTLDebugPort *>::iterator port_it = ports->begin(),
                                              port_end = ports->end();
             port_it != port_end; ++port_it) {
            RTLDebugPort *port = *port_it;
            Out << ",\n\t." << port->getSignal()->getName() << "("
                << port->getSignal()->getName() << "_r)";
        }
    } else {
        Out << ",\n\t." << DEBUG_SIGNAL_NAME_CURRENT_STATE << "("
            << DEBUG_SIGNAL_NAME_CURRENT_STATE << ")";
    }

    Out << ",\n\t." << DEBUG_SIGNAL_NAME_TRACE_REGS << "("
        << DEBUG_SIGNAL_NAME_TRACE_REGS << "),\n";
    Out << "\t." << DEBUG_SIGNAL_NAME_TRACE_REGS_EN << "_a("
        << DEBUG_SIGNAL_NAME_TRACE_REGS_EN << "_a),\n";
    Out << "\t." << DEBUG_SIGNAL_NAME_TRACE_REGS_EN << "_b("
        << DEBUG_SIGNAL_NAME_TRACE_REGS_EN << "_b)";

    vector<RTLDebugPort *> *ports = main->getDbgTracePorts();
    for (vector<RTLDebugPort *>::iterator port_it = ports->begin(),
                                          port_end = ports->end();
         port_it != port_end; ++port_it) {
        RTLDebugPort *port = *port_it;
        Out << ",\n\t." << port->getSignal()->getName() << "("
            << port->getSignal()->getName() << ")";
    }

    Out << "\n);\n\n";
}

void VerilogWriter::printDebuggerInstance() {
    int tag_bits = requiredBits(alloc->getNumRAMs() + 1);

    int offset_bits = 0;
    for (Allocation::const_ram_iterator RI = alloc->ram_begin(),
                                        RE = alloc->ram_end();
         RI != RE; ++RI) {
        RAM *ram = *RI;

        int max_offset = requiredBits(ram->getElements() - 1);
        max_offset += (int)log2(ram->getDataWidth() / 8);

        offset_bits = max(offset_bits, max_offset);
    }

    Out << "hlsd\n";
    Out << "#(\n";
    if (LEGUP_CONFIG->isXilinxBoard())
        Out << ".FPGA_VENDOR(\"Xilinx\"),\n";
    else
        Out << ".FPGA_VENDOR(\"Altera\"),\n";

    if (alloc->getDbgInfo()->getOptionTraceRegs())
        Out << ".OPTION_TRACE_REGS(1),\n";
    else
        Out << ".OPTION_TRACE_REGS(0),\n";

    if (alloc->getDbgInfo()->getOptionRegBufferDualPorted())
        Out << ".OPTION_TRACE_REGS_DUAL_PORTED(1),\n";
    else
        Out << ".OPTION_TRACE_REGS_DUAL_PORTED(0),\n";

    if (alloc->getDbgInfo()->getOptionSupportsReadFromMem()) {
        Out << ".OPTION_MEMORY_READ_AT_TRIGGER(1),\n";
        Out << ".OPTION_MEMORY_READ_THEN_RESUME(1),\n";
    }

    Out << ".MEMORY_CONTROLLER_ADDR_SIZE(`MEMORY_CONTROLLER_ADDR_SIZE),\n";
    Out << ".MEMORY_CONTROLLER_DATA_SIZE(`MEMORY_CONTROLLER_DATA_SIZE),\n";
    Out << ".SYSTEM_ID("
        << "32'h" << std::hex << alloc->getDbgInfo()->getSystemID() << std::dec
        << "),\n";
    Out << ".MEM_TAG_BITS(" << tag_bits << "),\n";
    Out << ".MEM_OFFSET_BITS(" << offset_bits << "),\n";

    Out << ".CONTROL_BUFFER_DEPTH(" << alloc->getDbgInfo()->getTraceCtrlDepth()
        << "),\n";
    Out << ".DATA_BUFFER_DEPTH(" << alloc->getDbgInfo()->getTraceMemDepth()
        << "),\n";
    Out << ".REGS_BUFFER_DEPTH(" << alloc->getDbgInfo()->getTraceRegsDepth()
        << "),\n";

    int regsWidth = max(alloc->getDbgInfo()->getRegsTraceBits(), 1);
    Out << ".REGS_WIDTH(" << regsWidth << "),\n";
    Out << ".PC_MODULE_BITS(" << alloc->getDbgInfo()->getInstanceIdBits()
        << "),\n";
    Out << ".PC_STATE_BITS(" << alloc->getDbgInfo()->getStateBits() << ")\n";

    Out << ")\n";
    Out << "hlsd_inst\n";
    Out << "(\n";
    Out << ".clk(clk),\n";
    Out << ".reset(reset),\n";
    Out << ".start(debugger_start),\n";
    Out << ".finish(finish),\n";
    Out << ".program_clk(program_clk),\n";
    Out << ".program_reset(program_reset),\n";
    Out << ".pc_module(" << DEBUG_SIGNAL_NAME_ACTIVE_INST << "),\n";
    Out << ".pc_state(" << DEBUG_SIGNAL_NAME_CURRENT_STATE << "),\n";
    Out << ".memory_controller_enable_a(memory_controller_enable_a),\n";
    Out << ".memory_controller_enable_b(memory_controller_enable_b),\n";
    Out << ".memory_controller_address_a(memory_controller_address_a),\n";
    Out << ".memory_controller_address_b(memory_controller_address_b),\n";
    Out << ".memory_controller_write_enable_a(memory_controller_write_enable_a)"
           ",\n";
    Out << ".memory_controller_write_enable_b(memory_controller_write_enable_b)"
           ",\n";
    Out << ".memory_controller_in_a(memory_controller_in_a),\n";
    Out << ".memory_controller_in_b(memory_controller_in_b),\n";
    Out << ".memory_controller_out_a(memory_controller_out_a),\n";
    Out << ".memory_controller_out_b(memory_controller_out_b),\n";
    Out << ".memory_controller_size_a(memory_controller_size_a),\n";
    Out << ".memory_controller_size_b(memory_controller_size_b),\n";
    Out << ".main_enable_a(main_memory_controller_enable_a),\n";
    Out << ".main_enable_b(main_memory_controller_enable_b),\n";
    Out << ".main_address_a(main_memory_controller_address_a),\n";
    Out << ".main_address_b(main_memory_controller_address_b),\n";
    Out << ".main_write_enable_a(main_memory_controller_write_enable_a),\n";
    Out << ".main_write_enable_b(main_memory_controller_write_enable_b),\n";
    Out << ".main_in_a(main_memory_controller_in_a),\n";
    Out << ".main_in_b(main_memory_controller_in_b),\n";
    Out << ".main_out_a(main_memory_controller_out_a),\n";
    Out << ".main_out_b(main_memory_controller_out_b),\n";
    Out << ".main_size_a(main_memory_controller_size_a),\n";
    Out << ".main_size_b(main_memory_controller_size_b),\n";

    if (alloc->getDbgInfo()->getOptionTraceRegs()) {
        Out << ".regs_trace_wr_en_a(" << DEBUG_SIGNAL_NAME_TRACE_REGS_EN
            << "_a),\n";
        Out << ".regs_trace_wr_en_b(" << DEBUG_SIGNAL_NAME_TRACE_REGS_EN
            << "_b),\n";
        Out << ".regs_trace(" << DEBUG_SIGNAL_NAME_TRACE_REGS << "),\n";
    } else {
        Out << ".regs_trace_wr_en_a(1'b0),\n";
        Out << ".regs_trace_wr_en_b(1'b0),\n";
        Out << ".regs_trace(),\n";
    }

    Out << ".uart_rx(uart_rx),\n";
    Out << ".uart_tx(uart_tx),\n";
    Out << ".hlsd_state(hlsd_state),\n";
    Out << ".external_start(start)\n";
    Out << ");\n";
    Out << "\n";
}

} // End legup namespace
