//===-- SchedulerDAG.cpp -----------------------------------*- C++ -*-===//
//
// This file is distributed under the LegUp license. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the data structures needed for scheduling.
//
//===----------------------------------------------------------------------===//

#include "FiniteStateMachine.h"
#include "SchedulerDAG.h"
#include "Scheduler.h"
#include "State.h"
#include "LegupPass.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/ErrorHandling.h"
#include "utils.h"
#include "LegupConfig.h"
#include "llvm/Pass.h"
#include "llvm/Analysis/AliasAnalysis.h"

#include "llvm/Analysis/CFG.h"
#include "llvm/ADT/SCCIterator.h"
#include "llvm/ADT/PostOrderIterator.h"
#include <algorithm>

using namespace llvm;
using namespace legup;

namespace legup {

// Add dependencies for all the operands of iNode
// ie. %3 = add %1, %2
// %3 is dependent on %1 and %2
// %1 is used by %3
// %2 is used by %3
void SchedulerDAG::regDataDeps(InstructionNode *iNode) {
    Instruction *inst = iNode->getInst();
    // these instructions are not scheduled
    if (isa<AllocaInst>(inst) || isa<PHINode>(inst))
        return;
    for (User::op_iterator i = inst->op_begin(), e = inst->op_end(); i != e;
         ++i) {
        // we only care about operands that are created by other instructions
        Instruction *dep = dyn_cast<Instruction>(*i);
        // also ignore if the dependency is an alloca
        if (!dep || isa<AllocaInst>(dep))
            continue;

        // ignore operands from other basic blocks, these are
        // guaranteed to be in another state
        if (dep->getParent() != inst->getParent())
            continue;

        InstructionNode *depNode = getInstructionNode(dep);
        iNode->addDepInst(depNode);
        depNode->addUseInst(iNode);
    }
}

// returns true if there is a dependency from I1 -> I2
// based on alias analysis
bool SchedulerDAG::hasDependencyAA(Instruction *I1, Instruction *I2) {
    AliasAnalysis::Location Loc1, Loc2;

    if (isa<CallInst>(I1)) {
        // assume that any loads/stores after a call must indeed execute
        // AFTER the call
        return true;
    }

    // bool store = false;
    if (LoadInst *lInst = dyn_cast<LoadInst>(I1)) {
        Loc1 = AliasA->getLocation(lInst);
    } else if (StoreInst *sInst = dyn_cast<StoreInst>(I1)) {
        Loc1 = AliasA->getLocation(sInst);
        // store = true;
    } else {
        assert(0 && "Unexpected input");
    }

    if (isa<StoreInst>(I1) && isa<LoadInst>(I2)) {
        // RAW dependency:
        // I1 is a store and I2 is a load from potentially the same address
        LoadInst *lInst = dyn_cast<LoadInst>(I2);
        Loc2 = AliasA->getLocation(lInst);
        if (!AliasA->isNoAlias(Loc1, Loc2)) {
            return true;
        }

    } else if (isa<StoreInst>(I2)) {
        // WAW or WAR dependency:
        // I1 is a store OR a load and I2 is a store to
        // potentially same address
        StoreInst *sInst = dyn_cast<StoreInst>(I2);
        Loc2 = AliasA->getLocation(sInst);
        if (!AliasA->isNoAlias(Loc1, Loc2)) {
            return true;
        }
    } else {
        // RAR: no dependency
        assert(isa<LoadInst>(I1) && isa<LoadInst>(I2));
    }

    return false;
}

// find all memory dependencies: I1 -> I2 (where I2 is given)
void SchedulerDAG::memDataDeps(InstructionNode *I2Node) {
    Instruction *I2 = I2Node->getInst();
    BasicBlock *bb = I2->getParent();

    // loop over all candidates for I1 in the BB for dependencies I1 -> I2
    for (BasicBlock::iterator dep = bb->begin(), ie = bb->end(); dep != ie;
         ++dep) {
        Instruction *I1 = dep;

        // If we reach I2 then there are no more candidates for I1
        if (I1 == I2)
            break;

        if (!isa<LoadInst>(I1) && !isa<StoreInst>(I1) && !isa<CallInst>(I1))
            continue;

        if (isaDummyCall(I1))
            continue;

        if (hasDependencyAA(I1, I2)) {
            // errs() << "memDataDeps: I1 -> I2\n" <<
            //    "I1: " << *I1 << "\n" <<
            //    "I2: " << *I2 << "\n";
            InstructionNode *I1Node = getInstructionNode(I1);
            I2Node->addMemDepInst(I1Node);
            I1Node->addMemUseInst(I2Node);
        }
    }
}

// find all memory dependencies: I1 -> I2 (where I2 is given)
void SchedulerDAG::callDataDeps(InstructionNode *I2Node) {
    Instruction *I2 = I2Node->getInst();
    BasicBlock *b = I2->getParent();

    if (isaDummyCall(I2) && !isaPrintCall(I2))
        return;

    RAM *localMem = NULL;
    if (LEGUP_CONFIG->getParameterInt("LOCAL_RAMS") &&
        (isa<LoadInst>(I2) || isa<StoreInst>(I2))) {
        assert(alloc);
        localMem = alloc->getLocalRamFromInst(I2);
    }

    // loop over all candidates for I1 in the BB for dependencies I1 -> I2
    for (BasicBlock::iterator dep = b->begin(), ie = b->end(); dep != ie;
         ++dep) {
        Instruction *I1 = dep;

        // If we reach I2 then there are no more candidates for I1
        if (I1 == I2)
            break;

        if (!isa<LoadInst>(I1) && !isa<StoreInst>(I1) && !isa<CallInst>(I1))
            continue;

        if (isaDummyCall(I1) && !isaPrintCall(I1))
            continue;

        if (isa<LoadInst>(I1) || isa<StoreInst>(I1)) {
            RAM *depMem = alloc->getLocalRamFromInst(I1);
            if (localMem && depMem) {
                // only make them dependent if they point to the same memory
                if (localMem == depMem) {
                    if (LEGUP_CONFIG->getParameterInt("ALIAS_ANALYSIS")) {
                        if (!hasDependencyAA(I1, I2))
                            continue;
                    }
                    InstructionNode *I1Node = getInstructionNode(I1);
                    I2Node->addMemDepInst(I1Node);
                    I1Node->addMemUseInst(I2Node);
                }
                continue;
            }
        }

        InstructionNode *I1Node = getInstructionNode(I1);
        I2Node->addMemDepInst(I1Node);
        I1Node->addMemUseInst(I2Node);
    }
}

SchedulerDAG::~SchedulerDAG() {
    for (DenseMap<Instruction *, InstructionNode *>::iterator
             i = nodeLookup.begin(),
             e = nodeLookup.end();
         i != e; ++i) {
        delete i->second;
    }
}

// hasNoDelay - detect instructions which have no delay. For example a shift by
// constant will just be turned into a wire by Quartus
bool hasNoDelay(Instruction *instr) {
    if (instr->isShift()) {
        return (isa<ConstantInt>(instr->getOperand(1)));
    }
    if (isa<GetElementPtrInst>(instr)) {
        if (LEGUP_CONFIG->getParameterInt("DONT_CHAIN_GET_ELEM_PTR")) {
            return false;
        }
        for (unsigned i = 1; i < instr->getNumOperands(); i++) {
            if (!isa<ConstantInt>(instr->getOperand(i))) {
                return false;
            }
        }
        return true;
    }

    switch (instr->getOpcode()) {
    case (Instruction::And):
    case (Instruction::Or):
        return (isa<ConstantInt>(instr->getOperand(1)) ||
                getBitWidth(instr->getType()) == 1);
    }
    return (instr->isCast() || isa<PHINode>(instr) || isa<AllocaInst>(instr) ||
            instr->isTerminator() || isa<LoadInst>(instr) ||
            isa<CallInst>(instr));
}

void SchedulerDAG::updateDAGwithInst(Instruction *instr) {
    InstructionNode *iNode = getInstructionNode(instr);

    // set delay
    std::string opName = LEGUP_CONFIG->getOpNameFromInst(instr, alloc);
    if (opName.empty() || isMem(instr)) {
        if (isa<GetElementPtrInst>(instr)) {
            if (LEGUP_CONFIG->getParameterInt("DONT_CHAIN_GET_ELEM_PTR")) {
                iNode->setAtMaxDelay();
            } else if (hasNoDelay(instr)) {
                iNode->setDelay(0);
            } else {
                iNode->setAtMaxDelay();
            }
        } else if (hasNoDelay(instr)) {
            iNode->setDelay(0);
        } else {
            // errs() << "Empty: " << *instr << "\n";
            // assert(hasNoDelay(instr));
            iNode->setAtMaxDelay();
        }
    } else {
        Operation *Op = LEGUP_CONFIG->getOperationRef(opName);
        assert(Op);
        float critDelay = Op->getCritDelay();

        if (critDelay > InstructionNode::getMaxDelay()) {
            // errs() << "Warning delay " << critDelay <<
            //    "ns is greater than clock constraint of " <<
            //    InstructionNode::getMaxDelay() << "ns for instruction: " <<
            //    *instr << "\n";
            iNode->setAtMaxDelay();
        } else if (isMul(instr) &&
                   LEGUP_CONFIG->getParameterInt("MULTIPLIER_NO_CHAIN")) {
            iNode->setAtMaxDelay();
        } else {
            iNode->setDelay(critDelay);
        }
    }
}

void SchedulerDAG::generateDependencies(Instruction *instr) {
    InstructionNode *iNode = getInstructionNode(instr);

    // generate dependencies
    regDataDeps(iNode);

    if (isa<LoadInst>(*instr) || isa<StoreInst>(*instr)) {
        if (LEGUP_CONFIG->getParameterInt("ALIAS_ANALYSIS")) {
            memDataDeps(
                iNode); // create dependencies based on LLVM alias analysis
        } else {
            callDataDeps(iNode); // create dependences in same order as in IR
                                 // [LegUp 1.0 & 2.0 functionality]
        }
    } else if (isa<CallInst>(instr)) {
        callDataDeps(iNode);
    }
}

bool SchedulerDAG::skipInst(const Instruction *I) {
	//if (isa<AllocaInst>(I))
	//	return true;
	if (isaDummyCall(I))
		return true;
	if (isa<ReturnInst>(I) || isa<StoreInst>(I))
		return false;

	//if (isa<BranchInst>(I) || isa<SwitchInst>(I))
	//	return false;

	// ignore instructions with no uses
	if (I->hasNUses(0))
		return true;

	return false;
}

IntGraph SchedulerDAG::makeDFG(Function &F) {
	int n = 0;
	instMap.clear();
	for (Function::iterator b = F.begin(); b!= F.end(); ++b) {
		for (BasicBlock::const_iterator i = (*b).begin(); i != (*b).end(); ++i) {
			if (skipInst(i))
				continue;

			instMap[i] = n++;
		}
	}

	IntGraph graph(n);
	for (MINST::iterator i = instMap.begin(); i != instMap.end(); ++i) {
		const Instruction *I = i->first;
		int idx = i->second;
		for (User::const_op_iterator op = I->op_begin(), e = I->op_end(); op != e; ++op) {
	    	const Instruction *dep = dyn_cast<Instruction>(*op);
	    	if (!dep)
	    	    continue;

			int d = instMap[dep];
			graph.addEdge(d, idx);
		}
	}

	return graph;
}

void SchedulerDAG::setAdjData(std::vector<VINT> &alist,
                              VINT scc,
                              IntGraph graph,
                              std::map<int, int> &intMap,
                              std::map<int, int> &intMap_R) {

	int idx=0;
	for (VINT::iterator v=scc.begin(); v!=scc.end(); ++v) {
		intMap_R[idx] = *v;
		intMap[*v] = idx++;
	}

	for (VINT::iterator v=scc.begin(); v!=scc.end(); ++v) {
		VINT currentAdjList;
		for (std::list<int>::iterator i = graph.adj[*v].begin(); i != graph.adj[*v].end(); ++i) {
			if (std::find(scc.begin(), scc.end(), *i)!=scc.end())
				currentAdjList.push_back(intMap[*i]);
		}
		alist.push_back(currentAdjList);
	}
}

const Instruction* SchedulerDAG::getInstNode(int idx) {
	for (MINST::iterator i = instMap.begin();
			i != instMap.end(); ++i) {
		if (i->second==idx)
			return i->first;
	}
	return NULL;
}

void SchedulerDAG::addSCC(VINST scc, const Instruction* use) {
	VINST found_scc;
	//if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=2)
	//	errs() << "     Find!!\n";
	for(VINST::const_iterator i = std::find(scc.begin(), scc.end(), use),
	                          e = scc.end(); i!=e; ++i) {
		found_scc.push_back(*i);
		//if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=2)
		//	errs() << "         - " << getValueStr(*i) << "\n";
	}
	addSCC(found_scc);
}

void SchedulerDAG::addSCC(VINST scc) {
	//FIXME - sorting may break the dependency order
	bool isSameList = false;
	std::sort(scc.begin(), scc.end());

	for (std::vector<VINST>::const_iterator i = InstCycles.begin(),
	                                        e = InstCycles.end(); i != e; ++i) {
		VINST currScc = *i;
		if (currScc.size() != scc.size())
			continue;

		if (scc == currScc) {
			isSameList = true;
			break;
		}
	}

	if (!isSameList) {
		InstCycles.push_back(scc);
	}
}

bool SchedulerDAG::findSCC(VINST scc, const Instruction *def, bool findWithinBB) {
   	for (User::const_op_iterator i = def->op_begin(), e = def->op_end(); i != e; ++i) {
		const Instruction *use = dyn_cast<Instruction>(*i);

		if (!use || isa<AllocaInst>(use))
			continue;

		if (isa<CallInst>(use))
			continue;

		if (findWithinBB && scc.front()->getParent() != use->getParent())
			continue;

		//if (isa<LoadInst>(use))
		//	continue;

		if (scc.front()==use) {
			addSCC(scc,use);
			return false;
		} else if (instMap[use]==0) {
			instMap[use] = 1;
			scc.push_back(use);
			//if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=2)
			//	errs() << "     <- (" << scc.size() << ")" << getValueStr(use) << "\n";
			if (!findSCC(scc, use, findWithinBB))
				return false;
			//if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=2)
			//	errs() << "     pop (" << scc.size() << ") : " << getValueStr(scc.back()) << "\n";
			scc.pop_back();
		}
	}
	return true;
}

void SchedulerDAG::clearInstMap() {
	for (MINST::iterator i = instMap.begin(); i != instMap.end(); ++i) {
		i->second = 0;
	}
}

void SchedulerDAG::findSCC(const BasicBlock* succ, bool findWithinBB) {
	for (BasicBlock::const_iterator si = succ->begin();
	                                si != succ->end(); si++) {
		const Instruction *def = si;
		clearInstMap();
		instMap[def] = 1;
		VINST scc;
		if (!isa<PHINode>(def))
			continue;

		//if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=2)
		//	errs() << "    INSTRUCTION:" << getValueStr(def) << "\n";
		scc.push_back(def);
		findSCC(scc, def, findWithinBB);
	}
}

void SchedulerDAG::insertSyncVoterDFS(Function &F) {
	instMap.clear();
	for (Function::iterator b = F.begin(); b != F.end(); ++b) {
		for (BasicBlock::const_iterator i = (*b).begin(); i != (*b).end(); ++i) {
			instMap[i] = 0;
		}
	}

	// add voter with SCC decomposition algorithm
	for (scc_iterator<Function *> I = scc_begin(&F); !I.isAtEnd(); ++I) {
		const std::vector<BasicBlock*> &SCCBBs = *I;
		//if (SCCBBs.size()==1)
		//	continue;

		// find SCC instructions
		if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=1)
			errs() << "  SCC(M):\n";
		for (std::vector<BasicBlock*>::const_iterator BBI = SCCBBs.begin(),
				                                      BBIE = SCCBBs.end();
		    	                                      BBI != BBIE; ++BBI) {
			if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=1)
				errs() << "    ->" << getLabel(*BBI) << "\n";
			findSCC(*BBI, SCCBBs.size()==1);
		}
	}
	//for (unsigned i = 0, e = BackEdges.size(); i != e; ++i) {
	//	const BasicBlock *succ = BackEdges[i].second;
	//	const BasicBlock *pred = BackEdges[i].first;
	//	if (pred == succ) {
	//		if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=1) {
	//			errs() << "  SCC(S):\n";
	//			errs() << "    ->" << getLabel(succ) << "\n";
	//		}
	//		bool findWithinBB = true;
	//		findSCC(succ, findWithinBB);
	//	}
	//}
}

void SchedulerDAG::insertSyncVoter(Function &F) {
	IntGraph graph = makeDFG(F);
	std::vector<std::vector<int> > isccs;
	graph.getSCCs(isccs);

	for (std::vector<VINT>::iterator s=isccs.begin(); s!=isccs.end(); ++s) {
		if ((*s).size()<=1)
			continue;

		vector<VINT> adjList;
		std::map<int, int> intMap, intMap_R;
		setAdjData(adjList, *s, graph, intMap, intMap_R);
		//for (unsigned i=0; i<adjList.size(); ++i) {
		//	cout << intMap_R[i] << ": ";
		//	for (VINT::iterator j=adjList[i].begin(); j!=adjList[i].end(); ++j)
		//		cout << intMap_R[*j] << " ";
		//	cout << "\n";
		//}

		std::vector<bool> blocked (adjList.size(), false);
		std::deque<int> stackLike;
		std::vector<VINT> cycles;

		std::vector<VINT> B_Fruitless;
		//B_Fruitless is the book keeping needed to avoid fruitless searches. It is
		//referred to as B in Johnson's original algorithm

		//Initialize
		for (unsigned i=0; i<adjList.size(); i++) {
			VINT k;
			B_Fruitless.push_back(k);
		}

		// loop to start new search from each node i
		for (unsigned i=0; i<adjList.size(); i++) {
			//clear all book keeping
			for (unsigned j=0; j<adjList.size(); j++) {
				blocked[j] = false;
				B_Fruitless[j].clear();
			}
			findCycles(i, i, adjList, blocked, stackLike, B_Fruitless, cycles);
		}
		//cout << "Cycles:\n";
		for (unsigned i=0; i<cycles.size(); ++i) {
			//cout << intMap_R[i] << ": ";
			VINST icycle;
			for (VINT::iterator j=cycles[i].begin(); j!=cycles[i].end(); ++j) {
				//cout << intMap_R[*j] << " ";
				const Instruction *I = getInstNode(intMap_R[*j]);
				icycle.push_back(I);
			}
			//cout << "\n";
			InstCycles.push_back(icycle);
		}
	}
}

unsigned SchedulerDAG::getBitWidth(Instruction *I) {
	unsigned width = (isa<BranchInst>(I) || isa<SwitchInst>(I))? 8
		: isa<AllocaInst>(I)? INF
		: isa<GetElementPtrInst>(I)? INF//alloc->getDataLayout()->getPointerSize() * 8
		: isa<ReturnInst>(I)? 32
		: isa<StoreInst>(I)? INF//I->getOperand(0)->getType()->getPrimitiveSizeInBits()
		: isa<LoadInst>(I)? INF
		: I->getType()->getPrimitiveSizeInBits();

	if (width==0) {
		//errs() << getValueStr(I) << "(" << I->getType()->getPrimitiveSizeInBits();
		//for (unsigned i=0; i<I->getNumOperands(); ++i)
		//	errs() << ", " << I->getOperand(i)->getType()->getPrimitiveSizeInBits();
		//errs() << ")\n";
		width = alloc->getDataLayout()->getPointerSize()*8;
	}

	return width;
}

void SchedulerDAG::connectDFGInst(int capacity[][MAX_NODE], Instruction *I, int t) {
	//bool cInst = (LEGUP_CONFIG->getParameterInt("PART_VOTER_MODE")==5
	//		&& (isa<BranchInst>(I) || isa<SwitchInst>(I)));
	int idx = instMap[I];
	capacity[idx+1][idx+2] = getBitWidth(I);
	capacity[idx][idx+1] = INF;
	capacity[idx+2][idx] = INF;

	//BasicBlock *B = I->getParent();
	//if (LEGUP_CONFIG->getParameterInt("PART_VOTER_MODE")==5
	//		&& !isa<BranchInst>(I) && !isa<SwitchInst>(I)) {
	//	int bbIdx = bbMap[B];
	//	capacity[bbIdx][idx] = INF;
	//}

	//if (LEGUP_CONFIG->getParameterInt("PART_VOTER_MODE")==5)
	//if (const PHINode *PN = dyn_cast<PHINode>(I)) {
	//	for (unsigned op = 0; op < PN->getNumIncomingValues(); ++op) {
	//		//BasicBlock *bb = PN->getIncomingBlock(op);
	//		TerminatorInst *tInst = PN->getIncomingBlock(op)->getTerminator();
	//		Instruction *dep = dyn_cast<Instruction>(tInst);
	//		//Value *IV = PN->getIncomingValueForBlock(bb);
	//		//errs() << getValueStr(I) << "<- [" << getValueStr(IV) << ", " << getLabel(bb) << "]\n";
	//		int d = instMap[dep];
	//		capacity[d+2][idx] = INF;
	//		capacity[idx][d+1] = INF;
	//	}
	//}

	for (User::op_iterator op = I->op_begin(), e = I->op_end(); op != e; ++op) {
	    // we only care about operands that are created by other instructions
	    Instruction *dep = dyn_cast<Instruction>(*op);

	    // also ignore if the dependency is an alloca
	    if (!dep)// || isa<AllocaInst>(dep))
	        continue;

		//if (instMap[dep]!=0 || instMap[I]!=t) {
			int d = instMap[dep];
			capacity[d+2][idx] = INF;
			capacity[idx][d+1] = INF;
			//errs() << getValueStr(I) << "<-" << getValueStr(dep) << "\n";
			//errs() << instMap[I] << "<-" << instMap[dep] << "\n";
		//}

		//update dependent edge weight to be connected tightly
		//these dependent edges are getelementptr calculation that they must be attached
		//to the load/store instructions
		//if (isa<LoadInst>(I) || isa<StoreInst>(I)) {
		//	capacity[d+1][d+2] = INF;
		//}
	}

	//if (cInst) {
	//	const TerminatorInst *tInst = I->getParent()->getTerminator();
	//	for (unsigned i=0, nSucc=tInst->getNumSuccessors(); i<nSucc; ++i) {
	//		BasicBlock *succ = tInst->getSuccessor(i);
	//		int bbIdx = bbMap[succ];
	//		capacity[idx+2][idx] = INF;
	//		capacity[bbIdx][idx+1] = INF;
	//	}
	//}

	//if (isa<LoadInst>(I)) {
	//	for (VINST::iterator d = storeInsts.begin();
	//	                     d != storeInsts.end(); ++d) {
	//		Instruction *dep = const_cast<Instruction *>(*d);
    //    	if (hasDependencyAA(dep, I) /*&& (instMap[dep]!=0 || instMap[I]!=t)*/) {
	//			int c = instMap[dep];
	//			capacity[c+1][idx] = INF;
	//			//errs() << getValueStr(I) << "<-" << getValueStr(dep) << "\n";
	//			//errs() << bbMap[dep->getParent()] << "(LD)->" << b << "\n";
	//		}
	//	}
	//}
}

unsigned SchedulerDAG::getVoterArea(unsigned numberOfCuts) {
	if (LEGUP_CONFIG->getParameterInt("NO_VOTER_AREA_ESTIMATE"))
		return 0;

	//FIXME - need to update with board characterization
	//unsigned voterArea = (numberOfCuts<100)? 0
	//	: (numberOfCuts<200)? numberOfCuts
	//	: (numberOfCuts<1000)? numberOfCuts*5
	//	: numberOfCuts*10;
	//return voterArea;
	return numberOfCuts*2;
}

int SchedulerDAG::getInstArea(PART_STATE s, unsigned v) {
	int area = getVoterArea(v);
	for (VINST::iterator i=insts.begin(); i!=insts.end(); ++i) {
		if (instPartState[*i] == s)
			area += getInstructionArea(const_cast<Instruction*>(*i));
	}
	return area;
}

int SchedulerDAG::getInstArea(VINST ilist, unsigned v) {
	int pArea = getVoterArea(v);
	for(VINST::iterator i=ilist.begin(); i!=ilist.end(); ++i)
		pArea += getInstructionArea(const_cast<Instruction*>(*i));

	return pArea;
}

unsigned SchedulerDAG::getInstructionArea(Instruction *instr) {
	//InstructionNode *iNode = getInstructionNode(instr);

	if (isa<BranchInst>(instr) || isa<SwitchInst>(instr)) {
		return 8;
	}

   	std::string opName = LEGUP_CONFIG->getOpNameFromInst(instr, alloc);
	if (isa<PHINode>(instr)) {
		std::string param[3];
		param[0] = "mux";
		int k = instr->getNumOperands();
		if (k<=2) k=2;
		else if(k<=4) k=4;
		else k=8;
		param[1] = utostr(k);
		int b = instr->getType()->getPrimitiveSizeInBits();
		if (b<=8) b=8;
		else if(b<=16) b=16;
		else if(b<=32) b=32;
		else b=64;
		param[2] = utostr(b);
		opName = param[0] + "_" + param[1] + "_" + param[2];
	} else if (isa<GetElementPtrInst>(instr)) {
		opName = "unsigned_add_8";//utostr(instr->getType()->getPrimitiveSizeInBits());
	} else if (isMem(instr)) {
		opName = "";//"unsigned_add_32";
	}

    if (instr->getOpcode() == Instruction::Add || instr->getOpcode() == Instruction::Sub) {
    	if (isa<ConstantInt>(instr->getOperand(1))) {
			opName = "signed_inc_" + utostr(instr->getType()->getPrimitiveSizeInBits());
		}
	}

	if (opName.empty()) {
		//errs() << getValueStr(instr) << "\n";
		return 0;
	}

	Operation *Op = LEGUP_CONFIG->getOperationRef(opName);
	assert(Op);

	//errs() << opName << "(" << Op->getLogicElements() << ") :" << getValueStr(instr) << "\n";

	//return Op->getLogicElements();
	return Op->getLUTs();
}

void SchedulerDAG::mergeNodes(bool frontMerge,
			const Instruction *boundaryInst, VINST p0, VINST p1,
			unsigned v0, unsigned v1) {
	int cArea = getcArea();

	// merge start and target nodes
	if (getInstArea(p0, v0)-getInstArea(PART_S_FINISH) < cArea) {
		for (VINST::iterator i=p0.begin(); i!=p0.end(); ++i)
			if (instPartState[*i] == PART_UNKNOWN)
				instPartState[*i] = PART_S;
	}
	if (getInstArea(p1, v1)-getInstArea(PART_T_FINISH) < cArea) {
		for (VINST::iterator i=p1.begin(); i!=p1.end(); ++i)
			if (instPartState[*i] == PART_UNKNOWN)
				instPartState[*i] = PART_T;
	}

	// boundary node merge
	int bArea = getInstructionArea(const_cast<Instruction*>(boundaryInst));
	if (frontMerge) {
		if (getInstArea(PART_S, v0)+bArea > cArea)
			pushNewInstPartition(PART_S);
		instPartState[boundaryInst] = PART_S;
	} else {
		if (getInstArea(PART_T, v1)+bArea > cArea)
			pushNewInstPartition(PART_T);
		instPartState[boundaryInst] = PART_T;
	}
}

void SchedulerDAG::pushNewInstPartition(PART_STATE s) {
	VINST pNodes;
	for (VINST::iterator i = insts.begin(); i != insts.end(); ++i) {
		if (instPartState[*i] == s) {
			pNodes.push_back(*i);
			instPartState[*i] = (s==PART_S)? PART_S_FINISH : PART_T_FINISH;
		}
	}
	errs() << "Push partitions " << (s==PART_S? "PART_S" : "PART_T") << "\n";
	InstPartitions.push_back(pNodes);
}

void SchedulerDAG::pushNewInstPartition(VINST v, PART_STATE s) {
	VINST pNodes;
	errs() << "Push partitions " << (s==PART_S_FINISH? "p0" : "p1") << "\n";
	for (VINST::iterator i = v.begin(); i != v.end(); ++i) {
		if (instPartState[*i] != s) {
			pNodes.push_back(*i);
			errs() << "\t(" << getInstructionArea(const_cast<Instruction*>(*i))
			       << ")" << getValueStr(*i) << "\n";
		}
	}
	InstPartitions.push_back(pNodes);
}

bool SchedulerDAG::isBalanced(VINST p0, VINST p1, unsigned v0, unsigned v1) {
	if (getInstArea(p0, v0)-getInstArea(PART_S_FINISH) < getcArea()
			&& getInstArea(p1, v1)-getInstArea(PART_T_FINISH) < getcArea())
		return true;

	return false;
}

const Instruction* SchedulerDAG::getBoundaryInst(int n, 
			int capacity[][MAX_NODE], int flow[][MAX_NODE],
			VINST &p0, VINST &p1, bool &frontMerge,
			unsigned &v0, unsigned &v1) {
	const Instruction* boundaryInst = NULL;

	bool visited[MAX_NODE];
	for(int i=0; i<MAX_NODE; i++)
		visited[i] = false;
	dfs(n-1, capacity, flow, 0, visited);

	p0.clear();
	p1.clear();
	for (VINST::iterator i = insts.begin(); i != insts.end(); ++i) {
		int idx = instMap[*i];
		if (visited[idx]) {
			p0.push_back(*i);
			assert(!isa<ReturnInst>(*i));
		} else
			p1.push_back(*i);
	}
	//for (VINST::iterator i = p0.begin(); i != p0.end(); ++i)
	//	errs() << "[0] " << getValueStr(*i) << "\n";
	//for (VINST::iterator i = p1.begin(); i != p1.end(); ++i)
	//	errs() << "[1] " << getValueStr(*i) << "\n";

	int minCut = 0;
	v0 = 0;
	v1 = 0;
	//VINST cutList;
	for(int i=0; i<n; i+=3) {
		if ((visited[i+1] && !visited[i+2]) && capacity[i+1][i+2]!=INF) {
			minCut++;
			//cutList.push_back(getInstNode(i));
			if (visited[i] == visited[i+1])
				v0 += capacity[i+1][i+2];
			else
				v1 += capacity[i+1][i+2];
		}
	}
	fprintf(stderr, "  %4d / %4d | %4d / %4d | %4d | %4d / %4d | mincut=%2d, v0=%2d, v1=%2d\n",
			getInstArea(PART_S_FINISH), getInstArea(PART_T_FINISH),
			getInstArea(PART_S), getInstArea(PART_T),
			getInstArea(PART_UNKNOWN), 
			getInstArea(p0), getInstArea(p1), minCut, v0, v1);
	//for (VINST::iterator i = cutList.begin(); i != cutList.end(); ++i)
	//	errs() << "  -" << getValueStr(*i) << "\n";

	//VINST blist;
	frontMerge = getInstArea(p0, v0) < getInstArea(p1, v1);
	for(int i=0; i<n; i+=3) {
		if (visited[i+1] && !visited[i+2]) {
			for (int j=3; j<n-1; j+=3) {
				if (capacity[j][i+1]) {
					const Instruction *d = frontMerge? getInstNode(j) : getInstNode(i);
					if (instPartState[d]==PART_UNKNOWN) {
						boundaryInst = d;
						break;
					}
				}
			}
			//for (int j=3; j<n-1; j++) {
			//	int f = frontMerge? flow[i+2][j] : flow[j][i];
			//	if (f>0) {
			//		const Instruction *d = getInstNode(j);
			//		if (instPartState[d]==PART_UNKNOWN) {
			//			//blist.push_back(d);
			//			boundaryInst = d;
			//			break;
			//		}
			//	}
			//}
		}
	}
	//unsigned minBit = INF;
	//for (VINST::iterator i = blist.begin(); i != blist.end(); ++i) {
	//	unsigned cbit = (*i)->getType()->getPrimitiveSizeInBits();
	//	if (cbit < minBit) {
	//		boundaryInst = *i;
	//		minBit = cbit;
	//	}
	//}

	//Select min-area boundaryBB from the out of min-cut list
	if (boundaryInst == NULL) {
		const Instruction *pred = NULL;
		const Instruction *succ = NULL;
		if (p0.size()>1) {
			for(VINST::iterator i = p0.begin(); i != p0.end(); ++i) {
				if (instPartState[*i] == PART_UNKNOWN && instMap[*i]!=0) {
					pred = *i;
				}
			}
		}
		if (p1.size()>1) {
			for (VINST::iterator i = p1.begin(); i != p1.end(); ++i) {
				if (instPartState[*i] == PART_UNKNOWN && instMap[*i]!=n-2) {
					succ = *i;
				}
			}
		}

		if (frontMerge && succ!=NULL) {
			boundaryInst = succ;
		} else if (!frontMerge && pred!=NULL) {
			boundaryInst = pred;
		} else if (succ) {
			frontMerge = true;
			boundaryInst = succ;
		} else if (pred) {
			frontMerge = false;
			boundaryInst = pred;
		}
	}

	//if (boundaryInst)
	//	errs() << getValueStr(boundaryInst) << "\n\n";
	return boundaryInst;
}

void SchedulerDAG::makeDFGInstGraph(int capacity[][MAX_NODE], int n) {
	for(int i=0; i<n; i++)
		for(int j=0; j<n; j++)
			capacity[i][j] = 0;

	// make INF path within merged nodes
	for (VINST::iterator i = insts.begin(); i != insts.end(); ++i) {
		int instIdx = instMap[*i];
		int state = instPartState[*i];
		if (state==PART_S || state==PART_S_FINISH) {
			capacity[0][instIdx] = INF;
			capacity[instIdx][0] = INF;
		} else if (state==PART_T || state==PART_T_FINISH) {
			capacity[instIdx][n-1] = INF;
			capacity[n-1][instIdx] = INF;
		}
	}

	//if (LEGUP_CONFIG->getParameterInt("PART_VOTER_MODE")==4) {
	//	// merge branch instructions
	//	for (VINST::iterator i=branchInsts.begin(); i!=branchInsts.end(); ++i) {
	//		for (VINST::iterator j=branchInsts.begin(); j!=branchInsts.end(); ++j) {
	//			int sidx = instMap[*i];
	//			int tidx = instMap[*j];
	//			capacity[sidx][tidx] = INF;
	//			capacity[tidx][sidx] = INF;
	//		}
	//	}
	//}

	// connectInsts
	for (VINST::iterator i = insts.begin(); i != insts.end(); ++i) {
		connectDFGInst(capacity, const_cast<Instruction*>(*i), n-1);
	}

	//dumpNF(capacity, n);
	assert(n<MAX_NODE);
}

int SchedulerDAG::getLimitAreaByPercentage(Function &F, unsigned areaMarginPercentage) {
	unsigned totalArea = 0;
	for (Function::iterator b = F.begin(); b != F.end(); ++b) {
        if (isEmptyFirstBB(b))
			continue;

		for (BasicBlock::iterator i = (*b).begin(); i != (*b).end(); ++i) {
			totalArea += getInstructionArea(i);
		}
	}

	unsigned pMode = LEGUP_CONFIG->getParameterInt("PART_VOTER_MODE");
	unsigned nPart = LEGUP_CONFIG->getParameterInt("NUMBER_OF_PARTITIONS");
	assert(nPart>0 || pMode==0);
	unsigned areaConstraint = totalArea/nPart;
	unsigned areaMargin = areaConstraint*areaMarginPercentage/100;

	if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=2) {
		errs() << "\n\n# DEBUG_TMR=2 - Partition Area Information\n";
		errs() << "Total Area = " << totalArea << "\n";
		errs() << "Area Limit = " << areaConstraint + areaMargin << "\n";
	}
	return areaConstraint + areaMargin;
}

int SchedulerDAG::initInstMap(Function &F) {
	int n=0;
	const Instruction *ret;
	for (Function::iterator b = F.begin(); b != F.end(); ++b) {
        if (isEmptyFirstBB(b))
			continue;

		//if (LEGUP_CONFIG->getParameterInt("PART_VOTER_MODE")==5) {
		//	bbMap[b] = n++;
		//	bbs.push_back(b);
		//}

		for (BasicBlock::iterator i = (*b).begin(); i != (*b).end(); ++i) {
   			//std::string opName = LEGUP_CONFIG->getOpNameFromInst(i, alloc);
			//errs() << opName << "(" << getInstructionArea(i) << ") " << getValueStr(i) << "\n";

			if (skipInst(i))
				continue;

			if (isa<ReturnInst>(i)) {
				ret = i;
				continue;
			}

			insts.push_back(i);

			instMap[i] = n;
			n+=3;
			if (isa<StoreInst>(i))
				storeInsts.push_back(i);
			if (isa<BranchInst>(i) || isa<SwitchInst>(i))
				branchInsts.push_back(i);
		}
	}
	// add a special node (node index = n) which has control dependency
	//if (LEGUP_CONFIG->getParameterInt("PART_VOTER_MODE")==5) {
	//	//const Instruction *fsm;
	//	//insts.push_back(fsm);
	//	//instMap[fsm] = n;
	//	n+=1;
	//}

	insts.push_back(ret);
	instMap[ret] = n++;
	return n;
}

void SchedulerDAG::networkFlowPartitionInsts(Function &F) {
	unsigned k = LEGUP_CONFIG->getParameterInt("NUMBER_OF_PARTITIONS");
	unsigned p = LEGUP_CONFIG->getParameterInt("PART_AREA_MARGIN_PERCENTAGE");

	int n = initInstMap(F);
	if (k<2) {
		InstPartitions.push_back(insts);
		return;
	}

	do {
		unsigned areaLimit = getLimitAreaByPercentage(F, p);
		networkFlowPartitionInsts(F, areaLimit, n);
		if (InstPartitions.size()>k) {
			p += 10;
			errs() << " - WARNING: Number of Partitions: " << InstPartitions.size()
			       << " (expected = " << k << ")"
			       << " / forced to increase area limit by 10%\n";
		} else if (InstPartitions.size()<k) {
			p -= 10;
			errs() << " - WARNING: Number of Partitions: " << InstPartitions.size()
			       << " (expected = " << k << ")"
			       << " / forced to decrease area limit by 10%\n";
		}
		// It may exceed 100 when voter area is relatively larger than module
		// area
		assert(p<=120);
	} while (InstPartitions.size()!=k);
}

void SchedulerDAG::networkFlowPartitionInsts(Function &F, unsigned areaLimit, int n) {
	int capacity[MAX_NODE][MAX_NODE];
	int flow[MAX_NODE][MAX_NODE];

	InstPartitions.clear();
	for (VINST::iterator i=insts.begin(); i!=insts.end(); ++i)
		instPartState[*i] = PART_UNKNOWN;
	//instMap.clear();
	//storeInsts.clear();

	setcArea(areaLimit);
	//instPartState[getInstNode(0)] = PART_S_FINISH;
	//instPartState[getInstNode(n-2)] = PART_T_FINISH;

	VINST p0;
	VINST p1;

	int itrCount = 0;
	const Instruction *boundaryInst = NULL;
	bool frontMerge;
	unsigned v0 = 0;
	unsigned v1 = 0;
	do {
		makeDFGInstGraph(capacity, n);
		maxFlow(n, 0, n-1, capacity, flow);
		boundaryInst = getBoundaryInst(n, capacity, flow, p0, p1, frontMerge, v0, v1);
		if (boundaryInst == NULL)
			errs() << "WARNING: not found boundaryInst\n";

		if (isBalanced(p0, p1, v0, v1) || boundaryInst == NULL) {
			pushNewInstPartition(p0, PART_S_FINISH);
			pushNewInstPartition(p1, PART_T_FINISH);
			break;
		}

		mergeNodes(frontMerge, boundaryInst, p0, p1, v0, v1);
	} while( itrCount++ < n );
}

bool SchedulerDAG::runOnFunction(Function &F, Allocation *_alloc) {
    assert(_alloc);
    alloc = _alloc;
    AliasAnalysis *AA = alloc->getAA();
    assert(AA);
    AliasA = AA;

    // generate Instruction to InstructionNode mapping
    for (Function::iterator b = F.begin(), be = F.end(); b != be; b++) {
        for (BasicBlock::iterator instr = b->begin(), ie = b->end();
                                  instr != ie; ++instr) {
    		InstructionNode *iNode = new InstructionNode(instr);
    		nodeLookup[instr] = iNode;
		}
	}

	//if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=2)
	//	errs() << "\n\n# DEBUG_TMR=2 ------ " << F.getName() << " ------\n";
	if (LEGUP_CONFIG->getParameterInt("TMR")) {
		if (LEGUP_CONFIG->getParameterInt("SYNC_VOTER_MODE")==1)
			insertSyncVoter(F);
		else if (LEGUP_CONFIG->getParameterInt("SYNC_VOTER_MODE")==2)
			insertSyncVoterDFS(F);
	}

	if (LEGUP_CONFIG->getParameterInt("PART_VOTER_MODE")==3) {
		//TODO - This mode is intened to partition at LLVM-IR level
		// but currently this mode is not implemented yet
		assert(0);
		networkFlowPartitionInsts(F);
	}


	// set delay
    for (Function::iterator b = F.begin(), be = F.end(); b != be; b++) {
        for (BasicBlock::iterator instr = b->begin(), ie = b->end();
             instr != ie; ++instr) {
            updateDAGwithInst(instr);
        }
    }

    for (Function::iterator b = F.begin(), be = F.end(); b != be; b++) {
        for (BasicBlock::iterator instr = b->begin(), ie = b->end();
             instr != ie; ++instr) {
            generateDependencies(instr);
        }
    }

    return false;
}

/// createStates - create new states for an FSM.
void createStates(unsigned begin, unsigned end,
                  std::map<unsigned, State *> &orderStates,
                  FiniteStateMachine *fsm) {
    assert(orderStates.find(begin - 1) != orderStates.end());
    for (; begin <= end; begin++) {
        orderStates[begin] = fsm->newState(orderStates[begin - 1]);
    }
}

/// setStateTransitions - determine the state transitions given a terminating
/// instruction
void setStateTransitions(State *lastState, const TerminatorInst *TI,
                         State *waitState,
                         std::map<BasicBlock *, State *> bbFirstState) {

    lastState->setTerminating(true);

    // unreachable could occur in a basic block like:
    // bb:                                               ; preds = %entry
    //   %2 = tail call i32 (i8*, ...)* @printf(i8* noalias getelementptr
    //    inbounds ([32 x i8]* @.str1, i32 0, i32 0)) nounwind ; <i32> [#uses=0]
    //   tail call void @exit(i32 1) noreturn nounwind
    //   unreachable
    if (isa<UnreachableInst>(TI) || TI->getOpcode() == Instruction::Ret) {
        lastState->setDefaultTransition(waitState);
        return;
    }

    lastState->setTransitionVariable(TI->getOperand(0));

    BasicBlock *Default;
    if (const SwitchInst *SI = dyn_cast<SwitchInst>(TI)) {

        for (unsigned i = 2, e = SI->getNumOperands(); i != e; i += 2) {
            Value *value = SI->getOperand(i);
            assert(value);
            BasicBlock *Succ = dyn_cast<BasicBlock>(SI->getOperand(i + 1));
            State *state = bbFirstState[Succ];

            lastState->addTransition(state, value);
        }

        Default = dyn_cast<BasicBlock>(SI->getDefaultDest());

    } else if (const BranchInst *B = dyn_cast<BranchInst>(TI)) {

        if (B->isConditional()) {
            Default = dyn_cast<BasicBlock>(TI->getSuccessor(1));

            BasicBlock *Succ = dyn_cast<BasicBlock>(B->getSuccessor(0));
            State *state = bbFirstState[Succ];

            lastState->addTransition(state);
        } else {
            Default = dyn_cast<BasicBlock>(TI->getSuccessor(0));
        }

    } else {
        llvm_unreachable(0);
    }

    State *state = bbFirstState[Default];
    lastState->setDefaultTransition(state);
}

/// createFSM - create a Finite State Machine object from the scheduler mapping.
FiniteStateMachine *SchedulerMapping::createFSM(Function *F,
                                                SchedulerDAG *dag) {
    // create FSM
    FiniteStateMachine *fsm = new FiniteStateMachine();

    // very first state
    State *waitState = fsm->newState();
    waitState->setDefaultTransition(waitState);

    // first state in each basic block
    std::map<BasicBlock *, State *> bbFirstState;
    // this map is used for labeling the states
    // each state name will have an index, from this map, as well as being
    // labeled with a function name, and a basic block name.
    std::map<BasicBlock *, unsigned> sCount;

    // create a FSM for the function where each basic block is assigned
    // an empty state as a placeholder
    unsigned bbNum = 0;
    for (Function::iterator b = F->begin(), be = F->end(); b != be; ++b) {
        if (isEmptyFirstBB(b))
            continue;

        State *s = fsm->newState();
        s->setBasicBlock(b);
        bbFirstState[b] = s;
        sCount[b] = 0;

        bbNum++;
    }

    // setup wait state transitions
    // setTransitionSignal(rtl->find("start"))
    // will be done later when we have access
    // to the RTL module
    Function::iterator firstBB = F->begin();
    if (BasicBlock *succ = isEmptyFirstBB(firstBB)) {
        // first BB was empty with just an unconditional branch,
        // so the waitstate should point to the next state, for instance:
        // BB_0:
        //   br label %BB_2
        // also need to set the basic block so phi's are still handled properly
        // for the empty BB
        waitState->setTerminating(true);
        waitState->setBasicBlock(firstBB);
        assert(bbFirstState.find(succ) != bbFirstState.end());
        waitState->addTransition(bbFirstState[succ]);
    } else {
        assert(bbFirstState.find(firstBB) != bbFirstState.end());
        waitState->addTransition(bbFirstState[firstBB]);
    }

    for (Function::iterator B = F->begin(), BE = F->end(); B != BE; ++B) {
        std::map<unsigned, State *> orderStates;
        if (isEmptyFirstBB(B))
            continue;

        orderStates[0] = bbFirstState[B];
        unsigned lastState = getNumStates(B);

        // int pipelined = getMetadataInt(B->getTerminator(),
        // "legup.pipelined");

        // errs() << "BB: " << getLabel(B) << " lastState: " << lastState <<
        // "\n";
        createStates(1, lastState, orderStates, fsm);

        for (BasicBlock::iterator instr = B->begin(), ie = B->end();
             instr != ie; ++instr) {
            Instruction *I = instr;
            unsigned order = getState(dag->getInstructionNode(I));

            orderStates[order]->push_back(I);

            // need to ensure multi-cycle instructions finish in the basic block
            unsigned delayState = Scheduler::getNumInstructionCycles(I);

            // Normally, loads take two cycles and the loaded values are stored
            // in shared memory controller output registers (port A or port B).
            //
            // In some flows however, we want each load to be stored in a
            // separate
            // register (e.g. to enable multi-cycle paths). But storing each
            // load
            // in a unique register and keeping the register on the output of
            // the
            // memory controller would make loads have a latency of 3, which is
            // not needed. Instead, when a separate register is created for each
            // load, make the FSM "think" that loads take 1 cycle (this is done
            // below). Then the second register is placed at the output of each
            // load (this is done in GenerateRTL.cpp, visitLoadInst);
            if (isa<LoadInst>(I) && LEGUP_CONFIG->duplicate_load_reg()) {
                delayState = 1; // Instead of normal 2 for loads
            }

            if (delayState == 0) {
                fsm->setEndState(I, orderStates[order]);
                continue;
            }

            delayState += order;
            if (delayState > lastState) {

                /*
                // assume iterative module scheduler has already handled
                // multi-cycle instructions
                // can't insert a new state - assume its ready in the first
                // state of next basic block
                if (pipelined) {
                    // this doesn't work for the kernel
                    assert(isa<LoadInst>(I));
                    //++B;
                    //assert(B != BE);
                    //fsm->setEndState(I, bbFirstState[B]);
                    //--B;

                    // all loads are assumed to be wires
                    fsm->setEndState(I, orderStates[order]);

                    continue;
                }
                */

                createStates(lastState + 1, delayState, orderStates, fsm);
                lastState = delayState;
            }

            fsm->setEndState(I, orderStates[delayState]);
        }

        setStateTransitions(orderStates[lastState], B->getTerminator(),
                            waitState, bbFirstState);
        orderStates[lastState]->setBasicBlock(B);

        for (unsigned i = 0; i < lastState; i++) {
            assert(orderStates.find(i) != orderStates.end());

            State *s = orderStates[i];
            s->setBasicBlock(B);
            s->setDefaultTransition(orderStates[i + 1]);
        }
    }

    FiniteStateMachine::iterator stateIter = fsm->begin();
    for (; stateIter != fsm->end(); stateIter++) {
        State *state = stateIter;
        if (!state->getBasicBlock()) {
            assert(state == waitState);
            continue;
        }

        sCount[state->getBasicBlock()] += 1;
        std::string newStateName = std::string("LEGUP_F_") +
                                   F->getName().str().data() + "_BB_" +
                                   getLabelStripped(state->getBasicBlock());

        stripInvalidCharacters(newStateName);
        state->setName(newStateName);
    }

    if (fsm->begin() != fsm->end())
        fsm->begin()->setName("LEGUP");

    return fsm;
}

void printNodeLabel(raw_ostream &out, InstructionNode *I) {
    out << *I->getInst();
}

// print a dot graph representing the dependency information (both normal and
// memory) for a basic block
void SchedulerDAG::printDFGDot(formatted_raw_ostream &out, BasicBlock *BB) {

    dotGraph<InstructionNode> graph(out, printNodeLabel);
    graph.setLabelLimit(40);

    bool ignoreDummyCalls =
        !LEGUP_CONFIG->getParameterInt("DFG_SHOW_DUMMY_CALLS");
    for (BasicBlock::iterator I = BB->begin(), ie = BB->end(); I != ie; ++I) {
        InstructionNode *op = getInstructionNode(I);
        if (ignoreDummyCalls && isaDummyCall(I))
            continue;

        std::string label = "label=\"D:" + ftostr(op->getDelay()) + "ns L:" +
                            utostr(Scheduler::getNumInstructionCycles(I)) +
                            "\",";
        for (Value::use_iterator use = I->use_begin(), e = I->use_end();
             use != e; ++use) {
            if (Instruction *child = dyn_cast<Instruction>(*use)) {
                if (ignoreDummyCalls && isaDummyCall(child))
                    continue;
                graph.connectDot(out, op, getInstructionNode(child),
                                 label + "color=blue");
            }
        }

        for (InstructionNode::iterator use = op->mem_use_begin(),
                                       e = op->mem_use_end();
             use != e; ++use) {
            if (ignoreDummyCalls && isaDummyCall((*use)->getInst()))
                continue;
            graph.connectDot(out, op, *use, label + "color=red");
        }
    }
}

} // End legup namespace
