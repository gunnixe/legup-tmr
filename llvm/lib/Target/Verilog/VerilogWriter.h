//===-- VerilogWriter.h -----------------------------------------*- C++ -*-===//
//
// This file is distributed under the LegUp license. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// VerilogWriter takes an RTLModule and prints out the corresponding
// Verilog along with necessary memory controllers and avalon signals.
//
//===----------------------------------------------------------------------===//

#ifndef LEGUP_VERILOG_WRITER_H
#define LEGUP_VERILOG_WRITER_H

#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"
#include "llvm/ADT/DenseMap.h"
#include "RTL.h"
#include <set>

#include <sstream>

using namespace llvm;

namespace legup {

class HwModule;
class Allocation;
class RAM;

/// VerilogWriter - Prints the Verilog for a RTLModule.
/// Also handles printing memory controller, test suite, and avalon interface
/// @brief VerilogWriter Class
class VerilogWriter {
public:
	typedef std::vector<const RTLSignal*> VSIG;
	typedef DenseMap<const RTLSignal*, int> MSIG;
	typedef DenseMap<int, const RTLSignal*> MSIG_R;

	enum PART_STATE { PART_UNKNOWN=0, PART_S, PART_T,
			PART_S_FINISH, PART_T_FINISH };

    //NC changes...
    VerilogWriter(raw_ostream &StreamOut, Allocation *alloc, std::set<const
		  Function*> AcceleratedFcts) : StreamOut(StreamOut), alloc(alloc),
						AcceleratedFcts(AcceleratedFcts), usesSynchronization(false) {}
    VerilogWriter(raw_ostream &StreamOut, Allocation *alloc) : StreamOut(StreamOut),
							 alloc(alloc), usesSynchronization(false) {}
    void print();
    
    void printRTL(const RTLModule *rtl);
    
    //NC changes...    
    void setRTL(const RTLModule* rtl) { this->rtl = rtl;}
    
    void printSignal(const RTLSignal *signal);
    void printRamInstance(RAM *R, bool memCtrlInstance=false);
    void printSignalConditionForInstruction(const RTLSignal* signal, const Instruction* I);
    void clearStringStreamBuffer() { this->Out.str(""); }
    std::string getStringStreamOut() { return this->Out.str(); }
    //

	//TMR
	unsigned ConstrainedFrames;

private:
    void printValue(const RTLSignal *sig,unsigned w=0, bool zeroExtend=false);
    void printValueMinBW(const RTLSignal *sig, unsigned w, bool zeroExtend);

    bool stripRAM(RAM *R);

    void printCaseFSM(const RTLSignal *signal, std::string assignOp);
    void caseConditions(const RTLSignal *condition, 
			std::vector<const RTLSignal *> &clausesToKeep);
    void getStateName(const RTLSignal *condition, std::string &param);

    void printComments(const Instruction *I, std::string prefix="");
    //NC changes.. 
    //void printSignal(const RTLSignal *signal);
    void printIndividualCondition(const RTLSignal* signal, int conditionNum, std::string assignOp, bool printCmnts);
    
    void printConditions(const RTLSignal *signal, std::string assignOp);
    std::string bitsForSignalAndModuleName(const RTLSignal *sig, std::string name);
    void printModuleInstance(std::stringstream &Out, const RTLModule *mod, std::string postfix="");
    void printMemCtrlModuleHeader();
    void printMemCtrlVariablesSignals(std::string postfix);
    void printMemCtrlVariables();
    void printMemCtrlRAMs();
    void printAddrAlignmentCheck(std::string postfix);
    void printPrevAddr(RAM *R, std::string postfix, std::string name);
    void printAlwaysTrigger(const RTLSignal *signal, const RTLSignal *driver);
    bool isConst(const RTLSignal *sig);
    
    void printTop(const Function * F);
    void printRAMModule();
    void printInferredRAMModule(bool readonly);
    void printAltSyncRAMModule(bool readonly);
    void printPLLModule();
    void printClkFollowerModule();
    void printMultipumpModule();
    void printDebugModule();
    void printDebugModuleInstance(std::string postfix);
    void printDebugModuleSignals(std::string postfix);
    void printArbiterModule();
    void printPriorityArbiterModule();
    void printMemoryControllerSignals(std::string postfix);
    void printMemoryController();
    void printMIFFiles();
    void printSimpleMemoryController();
    bool currentBoardHasPin(std::string pinName);
    void printBoardPortList();
    void printBoardSignalDeclarations();
    void printBoardLogic();
    void printBoardTopSignals();
    void printVerilogAtSpaceSeparatedPaths(char *);
    void printDebuggerInstance();
    void printTraceSchedulerInstance();
    void printDbgStateMuxer();
    void printBoardUtils();
    void printBoardTops();
    void printDE2();
    void printDE4();
    void printHex();
    void printVerilog(std::stringstream &Out, const HwModule *module);
    void printModuleDeclaration(const RTLModule *rtl);
    void printVerilogTestbench();
    void printMemoryVariablesSignals(std::string busName, std::string inputPrefix,
				     std::string outputPrefix, std::string postfix);
    void printMemoryVariables(bool top);
    void printBlankDefaultCase(std::string indent);
    void printRAMTag(RAM *R, std::string postfix);
    void printRAMSignals(std::string postfix);
    void printRAMSignalsHelper(std::string postfix, std::string
			       name, unsigned datawidth, std::string tag, bool isStruct);
    void printPrevTagCase(std::string postfix);
    void printRAMTagCase(std::string postfix);

    void printModuleHeader();
    void printVerilogOperator(const RTLOp *op, unsigned w=0);
    // Detects all placeholders in the string and returns a vector of booleans
    // indicating whether the placeholder is unsigned.
    // Currently only "%u" is considered as unsigned.
    std::vector<bool> parseDisplayString(const std::string &display_string);
    // Replaces C-style placeholder to Verilog placeholder and prints it out.
    void printDisplayString(std::string display_string);
    void printDisplay(const RTLOp *op);
    void printVerilogOpcode(RTLOp::Opcode opcode);

    void printTopHybrid(const Function * F, unsigned dataSize);
    void printMemTag(unsigned dataSize);
    void printGlobalTags(RAM *R);
    void printLocalTags(RAM *R, unsigned dataSize, unsigned &tagIndex);
    void printAvalonInterface(std::string ModuleName);
    int getAvalonBusWidth(const Function * F);
    void printIODeclarations(int AddressBusWidth);
    bool printSignalDeclarations(const Function * F, std::vector<int> &arg_bitwidth);
    void printArgumentSignalDeclarations(const Function * F, std::vector<int> &arg_bitwidth);
    void printAssignStatements(int NumParams, bool return64, bool isParallel);
    void printACCELassignStatements(bool isParallel);
    void printMemoryAssignStatements(int NumParams, bool return64, bool isParallel);
    void printArgsReceivers(int NumParams, const std::vector<int>
			    &arg_bitwidth);
    void printStartDoneSignals();
    void printMemorySignals();
    void printMemoryRegisters(std::string postfix);
    bool printReturnValSignals(bool return64, const Function * F, bool isParallel);
    void printMemoryControllerInstance(std::string postfix="");
    void printAccelInstance(const Function * F, int NumParams, bool voidtype);
    std::string searchMIPSAddress(RAM *R);
    //std::string parseMIPSdisassembly(const char * disassembly, std::string varName);
    void parseMIPSdisassembly();
    void printModelsimSignals(bool voidtype);
    void printDeclaration(const RTLSignal *signal, bool testBench=false);
    void printVerilogBitwidthPrefix(const RTLSignal *sig);
    //NC changes...
    //void printRamInstance(RAM *R);
    void printMainInstance(const bool usesPthreads, std::string postfix="");
    void printLockInstance(int lockIndex);
    void printLockModule();
    void printBarrierInstance(int barrierIndex);
    void printBarrierModule();
    void printMemoryDataReadySignals();
    void printMemoryDataReceivers();
    void printMemoryShiftRegisters(std::string postfix);
    void printMemoryControllerAssignStatements(std::string postfix);
    void printMemoryDataSignals();
    void printMemoryStateMachine();
    void printMemoryDataReceivers(std::string postfix);
    void printMemorySignalDeclarations(std::string postfix);
    void printoffChipMemoryFlags(std::string postfix);
    
    void printSynchronizationControllerVariables();
    void printSynchronizationControllerInstance();
    void printSynchronizationController();
    void printSyncCtrlModuleHeader();
    void printSyncCtrlVariables();
    void printSyncCtrlCoresVariables();
    void printSyncCtrlSignals();
    void printSyncCtrlCoreInstances();
    void printSyncTagCase(std::string syncType);
    void printSyncTag(const std::string syncType, const int index);
    void printSynchronizationControllerShiftReg(std::string postfix);
    void printSynchronizationControllerVariableDeclarations();
    void printSynchronizationControllerMuxLogic();
    std::string getMemoryOutputRegisters();

	// TMR
	bool noSharedMemoryController();
	//IntGraph* makeFunctionCallGraph();
	IntGraph* makeFunctionCallGraphFromModuleInstances();
	unsigned getFunctionArea(std::string name);
	unsigned getPartVoterArea(int instanceId);
	unsigned getSyncVoterArea(int instanceId);
	unsigned calculateFrames(std::map<std::string, unsigned> areas);
	float calculateRawFrames(std::map<std::string, unsigned> areas);
	void resetResourceConstraints(std::map<std::string, unsigned> &areas);
	void setResourceConstraints(std::map<std::string, unsigned> &constraints,
			                    std::map<std::string, unsigned> areas,
	                            unsigned areaMax, int pFrame, int pResouce);
	void setResourceConstraints(std::map<std::string, unsigned> &areas, std::string name);
	void addResourceConstraints(std::map<std::string, unsigned> &areas, std::string name);
	int checkResourceConstraints(std::map<std::string, unsigned> areas,
			                     std::map<std::string, unsigned> constraints);
	int checkResourceConstraints(std::map<std::string, unsigned> constraints);
	unsigned getInstructionArea(Instruction *instr);
	void makeUserReportCallGraph();
	void makeUserReportPartition();
	void doPartitions();
	unsigned calculatePartID(int node);
	void initPartitions();
	int getCaller(int node);
	bool getHierarchyName(std::string &name, int node, int callerId=0);
	void getHierarchyName(std::string &name, RTLModule *mod, RTLModule *caller=NULL);
	bool isPartModuleCommonSignal(const RTLSignal *sig);
	void propagateVoteSignalToMain(RTLSignal *sig, int caller);
	bool isFLPartVoteSignal(const RTLSignal *sig);
	bool isGlobalMemorySignal(const RTLSignal *sig);
	void printVoter(const RTLSignal *signal, bool isRegVoter=false);
	void printVoter(std::string name, std::string lo, std::string hi, bool isRegVoter);
	void printVoterInstance(RTLModule *mod, std::string postfix="");
	void printVoterBody(const RTLModule *mod);
	void printMemCtrlVoterBody();
	void printMemCtrlVoterInstance(std::string postfix);
	void flattenHierarchy();
	RTLModule* createMainInstance(RTLInstance *inst);
	void printFlattenedModuleInstances();
	RTLModule* createFlattenedInstance(RTLModule *m);
	bool isPrimitiveFunctionModule(RTLModule *m);
	bool isTmrVoterSignal(const RTLSignal* sig);
	void findInterconnection();

	// dfs
	void dfsPartition(std::map<std::string, unsigned> areasTotal, unsigned areaMax);
	void dfsPartition(int s, IntGraph* CallGraph, std::map<int, bool> &visit, 
		              std::map<std::string, unsigned> &areas, 
		              std::map<std::string, unsigned> &constraints, 
	                  int &partId, unsigned &maxArea);

	// misc
	bool isMemSig(const RTLSignal *sig);
	bool isMemInputSig(const RTLSignal *sig);
	bool isMemOutputSig(const RTLSignal *sig);
	bool isGlobalMemSig(const RTLSignal *sig);
	bool isGlobalMemInputSig(const RTLSignal *sig);
	bool isGlobalMemOutputSig(const RTLSignal *sig);
	bool isLocalMemSig(const RTLSignal *signal);
	bool isLocalMemOutputSig(const RTLSignal *signal);
	bool isLocalMemInputSig(const RTLSignal *signal);

	bool isPartModuleSig(const RTLSignal *sig);
	bool isPartModuleOutputSig(RTLPartModule *pm, RTLSignal *sig);
	bool isPortSig(const RTLSignal *sig);
	bool skipSig(const RTLSignal *signal);
	bool isFeedbackBetweenModules(int s, int t, RTLSignal *sig);

	void addOpToInput(VSIG &V, const RTLOp *op);
	void addSensitiveListToInput(VSIG &V, const RTLSignal *sig);
	void getSensitiveList(VSIG &V);
	void getSensitiveList(VSIG &V, const RTLSignal *sig);
	void getSensitiveList(VSIG &V, const RTLModule *mod);
	void getSensitiveList(VSIG &V, RAM *R);
	void getSensitiveList(VSIG &V, RTLPartModule *pm);
	RTLPartModule* getPartModule(const Instruction *instr);
	void initPartModules();
	RTLPartModule* findFirstUsePartModule();
	RTLPartModule* findFirstUsePartModule(RAM *R);
	RTLPartModule* findFirstUsePartModule(const RTLSignal *sig);
	RTLPartModule* findFirstUsePartModule(const RTLModule *mod);

	void printPipelinedVoterStatusSignal(RTLPartModule *pm);
	void printPipelinedVoterStatusSignal(std::vector<RTLSignal*> partVoters);
	void printTmrWires();
	void printTmrWires(std::string name, std::string width, std::string postfix);
    void printPartModuleRTL(RTLModule *rtl);
	void printPartModuleInstance(RTLPartModule *pm);
	void printPartModuleBody(RTLPartModule *pm);
	void printErrorBitSignal(RTLPartModule *pm, std::string postfix="");
	void printErrorBitBody(RTLPartModule *pm);
	void printErrorBitInstance(RTLPartModule *pm, std::string postfix);

	//network flow
	void networkFlowPartition(std::map<std::string, unsigned> areasTotal, unsigned areaMax);
	int networkFlowPartitionMain(std::map<std::string, unsigned> constraints);
	bool pushNewPartition(std::set<int> v, PART_STATE s);
	bool pushNewPartition(PART_STATE s);
	bool pushNewPartition();
	bool mergeNodes(int t,
					int capacity[][MAX_NODE],
	                bool frontMerge,
			        std::list<std::pair<int, int> > boundaryNodes,
                    std::set<int> p0, 
                    std::set<int> p1,
			        unsigned v0, 
                    unsigned v1,
	                std::map<std::string, unsigned> constraints);
	void makeDFGGraph(int capacity[][MAX_NODE], int t);
	void connectDFG(int capacity[][MAX_NODE]);
	int checkResourceConstraints(std::set<int> slist,
			                     std::map<std::string, unsigned> constraints);
	unsigned calculateVoterArea(unsigned v);
	void getAreas(std::map<std::string, unsigned> &areas, std::set<int> slist);
	void getAreas(std::map<std::string, unsigned> &areas, PART_STATE s);
	unsigned getArea(std::set<int> slist, unsigned v=0);
	unsigned getArea(PART_STATE s, unsigned v=0);
	void getBoundaryNodes(int n, int t,
                          std::list<std::pair<int, int> > &boundaries,
			              int capacity[][MAX_NODE], 
                          bool visited[],
                          bool &frontMerge);
	void getBoundaryNodes(int n, int t,
                          std::list<std::pair<int, int> > &boundaries,
                          int capacity[][MAX_NODE],
                          int flow[][MAX_NODE],
                          std::set<int> &p0,
                          std::set<int> &p1,
                          bool &frontMerge,
                          unsigned &v0,
                          unsigned &v1);
	int findTargetNode();
	bool isMemorySignal(const RTLSignal *sig);

	// TMR variables
	DenseMap<int, Function*> FunctionMap;
	DenseMap<int, RTLModule*> ModuleMap;
	DenseMap<int, RTLInstance*> InstanceMap;
	std::list<std::set<int> > Partitions;
	IntGraph *CallGraph;
	IntGraph *CallGraphBackward;
	std::list<RTLSignal*> FLPVoteSignals;
	std::set<RTLSignal*> topWires;
	DenseMap<int, int> partState;

	raw_fd_ostream &PartFile();
 
	//
    std::string indent, indent0;
    raw_ostream &StreamOut;
    std::stringstream Out;
    Allocation *alloc;
    const RTLModule *rtl;
    const std::set<const Function*> AcceleratedFcts;
    
    //std::map<std::string, std::set<std::string> > syncMap;
    std::map<std::string, int> syncMap;
    std::map<std::string, std::string> globalAddresses;
    bool usesSynchronization;

	//IL partitioning
	void makePartModuleWithSigPartitions(RTLModule *rtl);
	void updatePartVoterSignals(RTLModule *rtl);
	bool networkFlowPartitionSigs(RTLModule *rtl);
	bool networkFlowPartitionSigs(unsigned areaLimit, int n);
	unsigned initSigMap(RTLModule *rtl);
	unsigned getLimitAreaByPercentage(unsigned areaMarginPercentage);
	unsigned getSigArea(PART_STATE s, unsigned v=0);
	unsigned getSigArea(VSIG slist, unsigned v=0);
	unsigned getSigArea(const RTLSignal *signal);
	unsigned getFsmArea(const RTLSignal *signal);
	unsigned getVoterArea(unsigned numberOfCuts);
	bool isDivOutputSignal(const RTLSignal *signal);
	unsigned getInstructionAreaFromSignal(const RTLSignal *signal);
	unsigned getValueArea(const RTLSignal *sig, unsigned w=0);
	unsigned getOperatorArea(const RTLSignal *sig, unsigned w=0);
	unsigned getIndividualConditionArea(const RTLSignal *signal, int conditionNum);
	void makeDFGSigGraph(int capacity[][MAX_NODE], int n);
	const RTLSignal* getBoundarySig(int n, 
			                        int capacity[][MAX_NODE], 
                                    bool visited[],
                                    bool &frontMerge,
	                           	    unsigned areaLimit);
	const RTLSignal* getBoundarySig(int n, 
			                        int capacity[][MAX_NODE], 
                                    int flow[][MAX_NODE],
			                        VSIG &p0, 
                                    VSIG &p1, 
                                    bool &frontMerge,
			                        unsigned &v0, 
                                    unsigned &v1,
                                    unsigned areaLimit);
	void pushNewSigPartition(VSIG v, PART_STATE s);
	void pushNewSigPartition(PART_STATE s);
	void mergeSigNodes(int capacity[][MAX_NODE],
	                   int n,
	                   bool frontMerge,
			           const RTLSignal *boundarySig, 
                       VSIG p0, 
                       VSIG p1,
			           unsigned v0, 
                       unsigned v1,
                       unsigned areaLimit);
	unsigned getMemoryControllerPartID();
	unsigned getRound(unsigned w);
	std::string getOpName(const RTLOp *op, bool isSigned, unsigned w);
	void connectDFGSig(int capacity[][MAX_NODE], const RTLSignal *sig, int t);
	const RTLModule *getInstanceModuleFromOutputSignal(const RTLSignal *signal);
	RAM *getRamFromOutputSignal(const RTLSignal *signal);
	bool isBalanced(VSIG p0, VSIG p1, unsigned v0, unsigned v1);
	bool isOversized(VSIG p0, VSIG p1, unsigned v0, unsigned v1);

	MSIG sigMap;
	MSIG_R sigMap_R;
	VSIG sigs;
	MSIG sigPartState;
	std::vector<VSIG> SigPartitions;
	DenseMap<const RTLSignal*, const RTLSignal*> memorySignalPair;
	unsigned cArea;
	std::set<const RTLSignal*> Interconnections[64][64];

	unsigned getcArea() { return cArea; }
	void setcArea(unsigned a) { cArea = a; }

};

} // End legup namespace

#endif
