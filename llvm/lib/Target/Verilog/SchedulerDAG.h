//===-- SchedulerDAG.h -------------------------------------*- C++ -*-===//
//
// This file is distributed under the LegUp license. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the data structures needed for scheduling.
//
//===----------------------------------------------------------------------===//

#ifndef LEGUP_SCHEDULERDAG_H
#define LEGUP_SCHEDULERDAG_H

#include "Allocation.h"
#include "LegupPass.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Analysis/AliasAnalysis.h"

using namespace llvm;

namespace legup {
class FiniteStateMachine;
class LegupConfig;

class BasicBlockNode {
public:
	BasicBlockNode (BasicBlock *B) : bb(B) {}

	typedef std::vector<Instruction *>::iterator iterator; 
	typedef std::vector<Instruction *>::const_iterator const_iterator; 

	iterator       finput_begin()       { return finput.begin(); }
	const_iterator finput_begin() const { return finput.begin(); }
	iterator       finput_end()         { return finput.end(); }
	const_iterator finput_end()   const { return finput.end(); }
	iterator       input_begin()        { return input.begin(); }
	const_iterator input_begin()  const { return input.begin(); }
	iterator       input_end()          { return input.end(); }
	const_iterator input_end()    const { return input.end(); }
	iterator       output_begin()       { return output.begin(); }
	const_iterator output_begin() const { return output.begin(); }
	iterator       output_end()         { return output.end(); }
	const_iterator output_end()   const { return output.end(); }

	bool addInput(Instruction *I) {
		if (std::find(input.begin(), input.end(), I) == input.end()) {
			input.push_back(I);
			return true;
		}
		return false;
	}
	bool addFeedbackInput(Instruction *I) {
		if (std::find(finput.begin(), finput.end(), I) == finput.end()) {
			finput.push_back(I);
			return true;
		}
		return false;
	}
	void addOutput(Instruction *I) { output.push_back(I); }
	BasicBlock* getBasicBlock() { return bb; }

private:
	BasicBlock *bb;
	std::vector<Instruction *> input;
	std::vector<Instruction *> finput;
	std::vector<Instruction *> output;
};

/// InstructionNode - Container for Instructions to contain schedule-specific
/// information. Contains memory dependencies.
/// TODO: include pseudo-instructions, etc...
/// @brief Legup Instruction container class
class InstructionNode {
public:
    InstructionNode (Instruction *I) : inst(I),
	    backwardEdge(false), partitionEdge(false) {}

    typedef SmallVector<InstructionNode*, 3> depType;
    typedef depType::iterator iterator;
    typedef depType::const_iterator const_iterator;

    // true data dependencies, for example:
    //    %1 = add %2, %3
    //    %4 = add %1, %5
    // %1 depends on %2 and %3
    // %4 depends on %1 and %5
    // another example:
    //    %a = load
    //    store %a
    // Here you would have to delay the store by two (if loads have a
    // 2 cycle latency)
    iterator       dep_begin()       { return depInsts.begin(); }
    const_iterator dep_begin() const { return depInsts.begin(); }
    iterator       dep_end()         { return depInsts.end(); }
    const_iterator dep_end() const   { return depInsts.end(); }
    iterator       back_dep_begin()       { return backDepInsts.begin(); }
    const_iterator back_dep_begin() const { return backDepInsts.begin(); }
    iterator       back_dep_end()         { return backDepInsts.end(); }
    const_iterator back_dep_end() const   { return backDepInsts.end(); }

    // in the example above:
    // %2 and %3 are used by %1
    // %1 and %5 are used by %4
    iterator       use_begin()       { return useInsts.begin(); }
    const_iterator use_begin() const { return useInsts.begin(); }
    iterator       use_end()         { return useInsts.end(); }
    const_iterator use_end() const   { return useInsts.end(); }
    iterator       back_use_begin()       { return backUseInsts.begin(); }
    const_iterator back_use_begin() const { return backUseInsts.begin(); }
    iterator       back_use_end()         { return backUseInsts.end(); }
    const_iterator back_use_end() const   { return backUseInsts.end(); }

    // dependencies due to alias analysis
    // these could be RAW, WAR, or WAR dependencies, for example:
    //    store %1
    //    store %2
    // would be a WAW dependency if pointers %1 and %2 alias
    // another example:
    //    %a = load
    //    store %b
    // could be a WAR dependency if %a and %b alias. In this
    // case you should delay the store by ONE cycle (not two as in the case
    // above) because you don't need to wait for the load to finish
    // For example:
    //    %... = load
    //    store %...
    // Calling mem_dep_begin() on the store would return the load since
    // there is a WAR dependency from load -> store. The store 'depends'
    // on the load instruction.
    // iterate over all instructions that "depend" on this memory instruction
    iterator       mem_dep_begin()       { return bbMemDepInsts.begin(); }
    const_iterator mem_dep_begin() const { return bbMemDepInsts.begin(); }
    iterator       mem_dep_end()         { return bbMemDepInsts.end(); }
    const_iterator mem_dep_end() const   { return bbMemDepInsts.end(); }

    // For example:
    //    %... = load
    //    store %...
    // Calling mem_use_begin() on the load would return the store since
    // there is a WAR dependency from load -> store. The load is 'used' by
    // the store instruction.
    // iterate over all instructions that "use" the result of this memory
    // instruction
    iterator       mem_use_begin()       { return bbMemUseInsts.begin(); }
    const_iterator mem_use_begin() const { return bbMemUseInsts.begin(); }
    iterator       mem_use_end()         { return bbMemUseInsts.end(); }
    const_iterator mem_use_end() const   { return bbMemUseInsts.end(); }

    void addBackDepInst(InstructionNode* in) { backDepInsts.push_back(in); }
    void addBackUseInst(InstructionNode* in) { backUseInsts.push_back(in); }
    void addDepInst(InstructionNode* in) { depInsts.push_back(in); }
    void addUseInst(InstructionNode* in) { useInsts.push_back(in); }
    void addMemDepInst(InstructionNode* in) { bbMemDepInsts.push_back(in); }
    void addMemUseInst(InstructionNode* in) { bbMemUseInsts.push_back(in); }

    void setDelay (float _delay) {
        delay = _delay;
        assert(delay <= getMaxDelay());
    }
    float getDelay () { return delay; }
    void setAtMaxDelay () { delay = getMaxDelay(); }
    static float getMaxDelay () {
        // 10 ns ie. 100 Mhz
        // 5 ns ie. 200 Mhz
        //return 5
        return LEGUP_CONFIG->getParameterInt("CLOCK_PERIOD");
    }

  // the ASAP methods were relevant with the LegUp 1.0 release
  // deprecated with the SDC scheduler in the LegUp 1.1 release
    void setAsapDelay (int _delay) { asapPropDelay = _delay; }
    float getAsapDelay () { return asapPropDelay; }

    Instruction* getInst() { return inst; }

	// voter
	void setBackward(bool val) { backwardEdge = val; }
	bool getBackward() { return backwardEdge; }
	void setPartition(bool val) { partitionEdge = val; }
	bool getPartition() { return partitionEdge; }

private:
    depType backDepInsts;
    depType backUseInsts;
    depType depInsts;
    depType useInsts;
    depType bbMemUseInsts;
    depType bbMemDepInsts;

    Instruction* inst;

    // propogation delays, used for packing (will change when scheduled)
    // this is the state delay after the instruction executes
    float asapPropDelay;

    // intrinsic delay, calculated by constructor
    float delay;

	// voter
	bool backwardEdge;
	bool partitionEdge;
};

/// LegupSchedulerPass - Builds memory dependencies and Instruction to
/// InstructionNode lookup.
/// @brief Legup Scheduler Memory Dependency Pass
class SchedulerDAG {
public:
	typedef std::vector<const BasicBlock*> VBB;
	typedef std::vector<const Instruction*> VINST;
	typedef DenseMap<const BasicBlock*, int> MBB;
	typedef DenseMap<const Instruction*, int> MINST;

	#define INF 1000000000
	#define MAX_NODE 8192

	enum PART_STATE { PART_UNKNOWN=0, PART_S, PART_T,
			PART_S_FINISH, PART_T_FINISH };

    SchedulerDAG () {}
    ~SchedulerDAG();
    bool runOnFunction(Function &F, Allocation *_alloc);

    InstructionNode* getInstructionNode(Instruction *inst) {
        assert (nodeLookup.find(inst) != nodeLookup.end());
        return nodeLookup[inst];
    }
	BasicBlockNode* getBasicBlockNode(BasicBlock *bb) {
		assert (bbNodeLookup.find(bb) != bbNodeLookup.end());
		return bbNodeLookup[bb];
	}

    // print a .dot file for the dependency DFG of basic block BB
    void printDFGDot(formatted_raw_ostream &out, BasicBlock *BB);

	// voter
	void syncVoterScc(Function &F);
	void addSCC(VINST scc);
	void addSCC(VINST scc, const Instruction *use);
	bool isSCCInst(const Instruction *def);
	void findSCC(const BasicBlock* succ, bool findWithinBB = false);
	bool findSCC(VINST scc, const Instruction *i2, bool findWithinBB = false);
	void insertSyncVoterWithSCC();
	void insertSyncVoterOnMaxFanIn();
	void insertSyncVoterOnMaxFanOut(Function &F);
	void insertSyncVoter(Function &F);
	void insertSyncVoter(const BasicBlock* pred, const BasicBlock* succ);
	bool foundBackwardDependency(const Instruction *use, const Instruction *def,
               const BasicBlock* predBB, const BasicBlock* succBB,
               unsigned siIdx, unsigned piIdx);
	void findSCCBBs(Function &F);
	void findTopologicalBBList(Function &F);
	unsigned getInstructionArea(Instruction *instr);
	void findAllPaths(Function &F);
	void findAllPaths(std::vector<const BasicBlock *> path, const BasicBlock *current);
	bool isBackwardEdge(std::pair<const BasicBlock*, const BasicBlock*> edge);
	void insertPartitionVoter(Function &F);
	void runToposort(const Function &F);
	bool recursiveDFSToposort(const BasicBlock *BB, MBB &colorMap,
               std::vector<const BasicBlock *> &sortedBBs);
	bool isPredPartition(std::vector<const BasicBlock *> path, const BasicBlock *useBB);
	void bfsPartitionBBs(Function &F);
	void bfsPartitionInsts(Function &F);
	void bfsPartitionInsts(Function &F, unsigned areaLimit);

	// NetworkFlow Partitioning
	void networkFlowPartitionBBs(Function &F);
	void networkFlowPartitionInsts(Function &F);
	void networkFlowPartitionInsts(Function &F, unsigned areaLimit, int n);
	bool bfs(int n, int start, int target, int capacity[][MAX_NODE], int flow[][MAX_NODE], int pred[]);
	void dfs(int n, int capacity[][MAX_NODE], int flow[][MAX_NODE], int s, bool visited[]);
	const BasicBlock* isBBNode(int idx);
	const Instruction* isInstNode(int idx);
	int makeDFGBBGraph(int capacity[][MAX_NODE], int entryBB);
	void makeDFGInstGraph(int capacity[][MAX_NODE], int n);
	int maxFlow(int n, int s, int t, int capacity[][MAX_NODE], int flow[][MAX_NODE]);
	int initBBArea(Function &F);
	int getBBArea(VBB blist);
	int getBBArea(PART_STATE s);
	int getInstArea(VINST ilist);
	int getInstArea(PART_STATE s);
	int getMinGraphSize(Function &F);
	bool isBalanced(int totalArea, VBB p0, VBB p1);
	bool isBalanced(VINST p0, VINST p1);
	const BasicBlock *getBoundaryBB(int boundaryEdge, int flow[][MAX_NODE],
				bool &frontMerge, int s, int n);
	const BasicBlock* getBoundaryBB(int n, int start, int target, 
				int capacity[][MAX_NODE], int flow[][MAX_NODE],
				VBB &p0, VBB &p1, bool &frontMerge);
	const Instruction* getBoundaryInst(int n, 
			int capacity[][MAX_NODE], int flow[][MAX_NODE],
			VINST &p0, VINST &p1, bool &frontMerge);
	void setPartitions(const BasicBlock *boundaryBB, bool frontMerge);
	void connectDFGBB(int capacity[][MAX_NODE], const Instruction *I, int s, int t);
	void connectDFGInst(int capacity[][MAX_NODE], const Instruction *I, int t);
	int initMap(Function &F);
	bool skipInst(const Instruction *I);
	int getLimitAreaByPercentage(Function &F, unsigned areaMarginPercentage=0);
	void findPartitionSignals();
	void findInstPartitionSignals();
	bool isEmptyCand();
	void revisitPartitions();
	bool checkAreaConstraint(bool frontMerge, const BasicBlock *boundaryBB,
				VBB p0, VBB p1);
	void pushPlist(VBB &plist, VBB pNodes);
	void pushNewPartition(PART_STATE s);
	void pushNewInstPartition(PART_STATE s);
	void dumpNF(int capacity[][MAX_NODE], int n);
	void dumpVBB(VBB blist, std::string str);
	void dumpbbPartState(std::string str);
	void dumpFlow(int flow[][MAX_NODE], int s, int t, int max_flow);
	int initInstMap(Function &F);
	void clearInstMap();
	void mergeNodes(bool frontMerge, const Instruction *boundaryInst, VINST p0, VINST p1);
	void connectDFGInst(int capacity[][MAX_NODE], Instruction *I, int t);

	int getcArea() { return cArea; }
	void setcArea(int a) { cArea = a; }

	// TMR
	SmallVector<std::pair<const BasicBlock*, const BasicBlock*>, 32> BackEdges;
	std::queue<VBB> DAGPaths;
	std::vector<VBB> Partitions;
	std::vector<VINST> InstPartitions;
	Allocation *getAlloc() { return alloc; }
	std::vector<VINST> SCCs;

private:
    void regDataDeps(InstructionNode *iNode);
    void memDataDeps(InstructionNode *iNode);
    bool hasDependencyAA(Instruction *inst, Instruction *dep);
    void callDataDeps(InstructionNode *iNode);
    void updateDAGwithInst(Instruction *instr);
    void generateDependencies(Instruction *instr);

    AliasAnalysis            *AliasA;

    DenseMap<Instruction *, InstructionNode*> nodeLookup;
    DenseMap<BasicBlock *, BasicBlockNode*> bbNodeLookup;
    Allocation *alloc;

	// TMR
	MBB bbArea;
	MBB bbMap;
	MINST instMap;
	VINST storeInsts;

	MBB bbPartState;
	VBB bbs;

	//VBB startNodes;
	//VBB targetNodes;
	int cArea;

	MINST instPartState;
	VINST insts;
};

/// SchedulerMapping - Holds InstructionNode to state # mapping.
/// Usage: Pass this mapping between schedulers to effectively chain them.
/// @brief Legup Scheduler data structure for Instruction to state # lookup.
class SchedulerMapping {
public:
    unsigned getState(InstructionNode *i) { return map[i]; }
    void setState(InstructionNode *i, unsigned u) { map[i] = u; }
    unsigned getNumStates(BasicBlock *bb) { return stateNum[bb]; }
    void setNumStates(BasicBlock *bb, unsigned u) { stateNum[bb] = u; }
    FiniteStateMachine *createFSM(Function *, SchedulerDAG *);

private:
    DenseMap<InstructionNode*, unsigned> map;
    DenseMap<BasicBlock *, unsigned> stateNum;
};

} // End legup namespace

#endif
