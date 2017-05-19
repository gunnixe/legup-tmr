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
#include "Ram.h"
#include <algorithm>

using namespace llvm;
using namespace legup;

namespace legup {

bool SchedulerDAG::recursiveDFSToposort(const BasicBlock *BB, MBB &colorMap,
			VBB &sortedBBs) {
	colorMap[BB] = 1;
	// For demonstration, using the lowest-level APIs here. A BB's
	// successors are determined by looking at its terminator instruction.
	const TerminatorInst *TInst = BB->getTerminator();
	for (unsigned I = 0, NSucc = TInst->getNumSuccessors(); I < NSucc; ++I) {
		BasicBlock *Succ = TInst->getSuccessor(I);
		int succColor = colorMap[Succ];
		if (succColor == 0) {
			if (!recursiveDFSToposort(Succ, colorMap, sortedBBs))
				return false;
		//} else if (succColor == 1) {
		//	// This detects a cycle because grey vertices are all
		//	// ancestors of the currently explored vertex (in other words,
		//	// they're "on the stack").
		//	if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=2) {
		//		errs() << " Detected cycle: edge from " << getLabel(BB)
		//		       << " to " << getLabel(Succ) << "\n";
		//	}
		//	continue;
		//	//return false;
		}
	}
	// This BB is finished (fully explored), so we can add it to the
	// vector.
	colorMap[BB] = 2;
	sortedBBs.push_back(BB);
	return true;//(colorMap[target]==2);
}

void SchedulerDAG::runToposort(const Function &F) {
	// Color marks per vertex (BB).
	// Collects vertices (BBs) in "finish" order. The first finished vertex is
	// first, and so on.
	MBB colorMap;
	VBB sortedBBs;

	// Helper function to recursively run topological sorot from a given BB.
	// Returns true if the sort succeeded and false otherwise; topological
	// sort may fail if, for example, the graph is not a DAG (detected
	// a cycle).
	if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=2)
		errs() << "\n\n# DEBUG_TMR=2 - Topological sort of " << F.getName() << ":\n";
	// Initialize the color map by marking all the vertices white.
	for (Function::const_iterator I = F.begin(), IE = F.end(); I != IE; ++I) {
		colorMap[I] = 0;
	}

	// The BB graph has a single entry vertex from which the other BBs should
	// be discoverable - the function entry block.
	bool success = recursiveDFSToposort(&F.getEntryBlock(), colorMap, sortedBBs);
	if (success) {
		// Now we have all the BBs inside sortedBBs in reverse topological
		// order.
		for (VBB::const_reverse_iterator RI = sortedBBs.rbegin(),
		                                      RE = sortedBBs.rend();
		                                      RI != RE; ++RI) {
			if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=2)
				errs() << " " << getLabel(*RI) << "\n";
		}
	} else {
		errs() << " Sorting failed\n";
		assert(0);
	}

	DAGPaths.push(sortedBBs);
}

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
					localMem->setDependence(true);
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
	for (DenseMap<BasicBlock *, BasicBlockNode *>::iterator
			i = bbNodeLookup.begin(),
			e = bbNodeLookup.end();
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
	if (iNode->getPartition()
			&& LEGUP_CONFIG->getParameterInt("TMR")
			&& LEGUP_CONFIG->getParameterInt("PART_VOTER_MODE")!=0
			&& LEGUP_CONFIG->getParameterInt("SEQUENTIAL_PART_VOTER")) {
		iNode->setAtMaxDelay();
	} else if (opName.empty() || isMem(instr)) {
        if (isa<GetElementPtrInst>(instr)) {
            if (LEGUP_CONFIG->getParameterInt("DONT_CHAIN_GET_ELEM_PTR")) {
                iNode->setAtMaxDelay();
            } else if (hasNoDelay(instr)) {
                iNode->setDelay(0);
            } else {
                iNode->setAtMaxDelay();
            }
        } else if (hasNoDelay(instr)) {
			// TMR - add voter delay to backward edges
			//if (LEGUP_CONFIG->getParameterInt("TMR") &&
			//		LEGUP_CONFIG->getParameterInt("SYNC_VOTER_MODE")==4 &&
			//		isa<PHINode>(instr) && 
			//		iNode->getBackward()) {
			//	// FIXME - assume voter delay
			//	float syncVoterDelay = 0.5;
        	//	if (syncVoterDelay > InstructionNode::getMaxDelay())
			//		iNode->setAtMaxDelay();
			//	else
			//		iNode->setDelay(syncVoterDelay);
			//} else if (LEGUP_CONFIG->getParameterInt("TMR") &&
			//		LEGUP_CONFIG->getParameterInt("LOCAL_RAMS") &&
			//		isa<LoadInst>(instr)) {
			//	RAM *localRam = alloc->getLocalRamFromInst(instr);
			//	if (localRam && localRam->getScope() == RAM::LOCAL)
			//		iNode->setAtMaxDelay();
			//	else
            //		iNode->setDelay(0);
			//} else {
            	iNode->setDelay(0);
			//}
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

bool SchedulerDAG::foundBackwardDependency(const Instruction *use, const Instruction *def,
               const BasicBlock* predBB, const BasicBlock* succBB,
               unsigned siIdx, unsigned piIdx) {
	if (use == def) {
		if (predBB != succBB)
			return true;

		if (siIdx < piIdx)
			return true;
	}

	return false;
}

void SchedulerDAG::insertSyncVoterWithSCC() {
	for (std::vector<VINST>::const_iterator i = SCCs.begin(), e = SCCs.end(); i != e; ++i) {
		VINST scc = *i;
		for (VINST::const_iterator si = scc.begin(), se = scc.end(); si != se; ++si) {
			const Instruction* instr = *si;
			InstructionNode* iNode = getInstructionNode(const_cast<Instruction*>(instr));

			//FIXME
			// The first node in SCC is the backward edge found with DFS, If
			// we use SYNC_VOTER_MODE=2(or 3), it only finds subset of feedback edges
			// since it only check the instructions within feedback BBs,
			// and it does not find the instructions outside of feedback BBs.
			// In the following code, it finds every feedback edges for all SCC
			// instructions. And if we use condition, isa<PHINode>, it works as
			// SYNC_VOTER_MODE=2. Otherwise it works as SYNC_VOTER_MODE=3
			//if (isa<PHINode>(instr)) {
				iNode->setBackward(true);//, *scc); //use(phi)
				break;
			//}
		}
	}
} 

void SchedulerDAG::insertSyncVoter(const BasicBlock* pred, const BasicBlock* succ) {
	unsigned siIdx = 0;
	for (BasicBlock::const_iterator si = succ->begin();
	                                si != succ->end(); si++) {
		const Instruction *i2 = si;
		if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=3)
			errs() << "    INSTRUCTION:" << getValueStr(i2) << "\n";
   		for (User::const_op_iterator i = i2->op_begin(),
		                             e = i2->op_end(); i != e; ++i) {
			const Instruction *use = dyn_cast<Instruction>(*i);
			if (!use || isa<AllocaInst>(use))
				continue;

			unsigned piIdx = 0;
			for (BasicBlock::const_iterator pi = pred->begin();
			                                pi != pred->end(); pi++) {
				const Instruction *i1 = pi;
				if (foundBackwardDependency(use, i1, pred, succ, siIdx, piIdx)) {
					if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=3)
						errs() << "      <-" << getValueStr(i1) << "\n";
					//add backward information to InstructionNode
					InstructionNode* iNode = getInstructionNode(const_cast<Instruction*>(i2));
					InstructionNode* depNode = getInstructionNode(const_cast<Instruction*>(i1));
       				iNode->addBackDepInst(depNode);
       				depNode->addBackUseInst(iNode);

					if (LEGUP_CONFIG->getParameterInt("SYNC_VOTER_MODE")==3) {
						//FIXME - Now we do not support sync_voter_mode=3
						//errs() << "ERROR: 'SYNC_VOTER_MODE=3' is Not implemented yet!!!\n";
						//assert(0);
					    //LEGUP_CONFIG->getParameterInt("SYNC_VOTER_MODE")==5)
						depNode->setBackward(true); //def
					} else
						iNode->setBackward(true); //use(phi)
				}
				piIdx++;
			}
		}
		siIdx++;
	}
}

//template <class T>
//static bool compareVectors(const vector<T> &a, const vector<T> &b)
//{
//  const size_t n = a.size(); // make it const and unsigned!
//  std::vector<bool> free(n, true);
//  for ( size_t i = 0; i < n; ++i )
//  {
//      bool matchFound = false;
//      auto start = b.cbegin();
//      while ( true )
//      {
//          const auto position = std::find(start, b.cend(), a[i]);
//          if ( position == b.cend() )
//          {
//              break; // nothing found
//          }
//          const auto index = position - b.cbegin();
//          if ( free[index] )
//          {
//             // free pair found
//             free[index] = false;
//             matchFound = true;
//             break;
//          }
//          else
//          {
//             start = position + 1; // search in the rest
//          }
//      }
//      if ( !matchFound )
//      {
//         return false;
//      }
//   }
//   return true;
//}

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

	for (std::vector<VINST>::const_iterator i = SCCs.begin(), e = SCCs.end(); i != e; ++i) {
		VINST currScc = *i;
		if (currScc.size() != scc.size())
			continue;

		//Method 1
		//isSameList = compareVectors<const Instruction*>(currScc, scc);

		//FIXME - need to modify with faster matching algorithm
		//Method 2
		//unsigned matchCount = 0;
		//for (VINST::const_iterator ci = currScc.begin(),
		//                                                     ce = currScc.end(); ci != ce; ++ci) {
		//	for (VINST::const_iterator si = scc.begin(),
		//	                                                     se = scc.end(); si != se; ++si) {
		//		if (*ci == *si) {
		//			matchCount++;
		//			break;
		//		}
		//	}
		//}

		//if (matchCount == scc.size()) {
		//	isSameList = true;
		//	break;
		//}

		//Method 3
		if (scc == currScc) {
			isSameList = true;
			break;
		}
	}

	if (!isSameList) {
		SCCs.push_back(scc);
	}
}

bool SchedulerDAG::isSCCInst(const Instruction *def) {
	for (std::vector<VINST>::const_iterator i = SCCs.begin(),
	                                        e = SCCs.end(); i != e; ++i) {
		VINST scc = *i;
		if (scc.front()==def)
			return true;
	}
	return false;
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

//bool SchedulerDAG::findSCC(VINST scc, const Instruction *def) {
//   	for (User::const_op_iterator i = def->op_begin(), e = def->op_end(); i != e; ++i) {
//		const Instruction *use = dyn_cast<Instruction>(*i);
//		if (!use || isa<AllocaInst>(use))
//			continue;
//
//		// module output (return) is always voted upon.
//		if (isa<CallInst>(use))
//			continue;
//
//		// skip if 'use instruction' is already idendtified as feedback edges
//		if (LEGUP_CONFIG->getParameterInt("SYNC_VOTER_MODE")==5
//				&& isSCCInst(use))
//			continue;
//
//		if (scc.front() == use) {
//			addSCC(scc);
//			if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=2)
//				errs() << "     Find!!\n";
//			for (VINST::const_iterator si = scc.begin(),
//			                                                     ei = scc.end(); si != ei; ++si) {
//				if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=2)
//					errs() << "         - " << getValueStr(*si) << "\n";
//			}
//			if (LEGUP_CONFIG->getParameterInt("SYNC_VOTER_MODE")==5)
//				return false;
//			else
//				continue;
//		}
//
//		// push 'use instruction' only if it is not in the previous scc list
//		if (std::find(scc.begin(), scc.end(), use) == scc.end()) {
//			scc.push_back(use);
//			//if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=2)
//			//	errs() << "     <- " << getValueStr(use) << "\n";
//			if (!findSCC(scc, use))
//				return false;
//			//if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=2)
//			//	errs() << "     pop : " << getValueStr(scc.back()) << "\n";
//			scc.pop_back();
//		}
//	}
//	return true;
//}

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

void SchedulerDAG::syncVoterScc(Function &F) {
	// add voter with SCC decomposition algorithm
	for (scc_iterator<Function *> I = scc_begin(&F); !I.isAtEnd(); ++I) {
		const std::vector<BasicBlock*> &SCCBBs = *I;
		if (SCCBBs.size()==1)
			continue;

		// find SCC instructions
		if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=1)
			errs() << "  SCC(M):\n";
		for (std::vector<BasicBlock*>::const_iterator BBI = SCCBBs.begin(),
				                                      BBIE = SCCBBs.end();
		    	                                      BBI != BBIE; ++BBI) {
			if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=1)
				errs() << "    ->" << getLabel(*BBI) << "\n";
			findSCC(*BBI);
		}
	}
	for (unsigned i = 0, e = BackEdges.size(); i != e; ++i) {
		const BasicBlock *succ = BackEdges[i].second;
		const BasicBlock *pred = BackEdges[i].first;
		if (pred == succ) {
			if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=1) {
				errs() << "  SCC(S):\n";
				errs() << "    ->" << getLabel(succ) << "\n";
			}
			bool findWithinBB = true;
			findSCC(succ, findWithinBB);
		}
	}

	// When (SYNC_VOTER_MODE==6 || 7 || 8) setBackward() is performed after
	// generateRTL phase, since all FSM state should be determined to
	// calculate minimum latency between SCC Registers.
	if (LEGUP_CONFIG->getParameterInt("SYNC_VOTER_MODE")==5) {
		insertSyncVoterWithSCC();
	//} else if (LEGUP_CONFIG->getParameterInt("SYNC_VOTER_MODE")==7) {
	//	insertSyncVoterOnMaxFanIn();
	//} else if (LEGUP_CONFIG->getParameterInt("SYNC_VOTER_MODE")==8) {
	//	insertSyncVoterOnMaxFanOut(F);
	}

	// FIXME
	//if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=1)
	//	errs() << "\n\n# DEBUG_TMR=1 - SCC Instructions\n";
	//unsigned cnt = 0;
	//for (std::vector<VINST>::const_iterator i = SCCs.begin(),
	//                                        e = SCCs.end(); i != e; ++i) {
	//	VINST scc = *i;
	//	errs() << "  SCCs[" << cnt++ << "]\n";
	//	for (VINST::const_iterator ssi = scc.begin(), sse = scc.end(); ssi != sse; ++ssi) {
	//		errs() << "  " << getValueStr(*ssi) << "\n";
	//	}
	//}
}

void SchedulerDAG::insertSyncVoterOnMaxFanIn() {
	for (std::vector<VINST>::const_iterator i = SCCs.begin(); i != SCCs.end(); ++i) {
		VINST scc = *i;
		const Instruction *maxFanInInst = NULL;
		int maxFanInCnt = 0;
		for (VINST::const_iterator si = scc.begin(); si != scc.end(); ++si) {
			int fanInCnt = 0;
			for(const Use &U : (*si)->operands()) {
				Value *v = U.get();
				if (!isa<Constant>(v))
					fanInCnt++;
			}
			//errs() << getValueStr(*si) << "(fanInCnt=" << fanInCnt << ")\n";
			if (maxFanInCnt < fanInCnt) {
				maxFanInCnt = fanInCnt;
				maxFanInInst = *si;
			}
		}
		//errs() << "MaxFanInInst = " << getValueStr(maxFanInInst) << "\n";
		InstructionNode* iNode = getInstructionNode(const_cast<Instruction*>(maxFanInInst));
		iNode->setBackward(true);//, *scc); //use(phi)
	}	
}

void SchedulerDAG::insertSyncVoterOnMaxFanOut(Function &F) {
	for (std::vector<VINST>::const_iterator i = SCCs.begin(); i != SCCs.end(); ++i) {
		VINST scc = *i;
		const Instruction *maxFanOutInst = NULL;
		unsigned maxFanOutCnt = 0;
		for (VINST::const_iterator si = scc.begin(); si != scc.end(); ++si) {
			unsigned fanOutCnt = (*si)->getNumUses();
			//errs() << "def: " << getValueStr(*si) << "\n";
			//for (const User *U : (*si)->users()) {
			//	fanOutCnt++;
			//	//if (const Instruction *Inst = dyn_cast<const Instruction>(U)) {
			//	//	errs() << " -" << *Inst << "\n";
			//	//}
			//}
			if (maxFanOutCnt < fanOutCnt) {
				maxFanOutCnt = fanOutCnt;
				maxFanOutInst = *si;
			}
		}
		//errs() << "MaxFanOutInst = " << getValueStr(maxFanOutInst) << "\n";
		InstructionNode* iNode = getInstructionNode(const_cast<Instruction*>(maxFanOutInst));
		iNode->setBackward(true);//, *scc); //use(phi)
	}
}

void SchedulerDAG::clearInstMap() {
	for (MINST::iterator i = instMap.begin(); i != instMap.end(); ++i) {
		i->second = 0;
	}
}

void SchedulerDAG::insertSyncVoter(Function &F) {
	instMap.clear();
	for (Function::iterator b = F.begin(); b != F.end(); ++b) {
		for (BasicBlock::const_iterator i = (*b).begin(); i != (*b).end(); ++i) {
			instMap[i] = 0;
		}
	}

	int syncMode = LEGUP_CONFIG->getParameterInt("SYNC_VOTER_MODE");
	if (syncMode==8 || syncMode==7 || syncMode==6 || syncMode==5) {
		syncVoterScc(F);
	} else { //syncMode==4,3,2,1
		// add voter to the instruction which has backward edges
		if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=3)
			errs() << "\n\n# DEBUG_TMR=3 - Backward Edges\n";

		for (unsigned i = 0, e = BackEdges.size(); i != e; ++i) {
			const BasicBlock *succ = BackEdges[i].second;
			const BasicBlock *pred = BackEdges[i].first;

			if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=3)
				errs() << "  [" << i << "] " << getLabel(succ) << "<-" << getLabel(pred) << "\n";
			findSCC(succ);
			insertSyncVoter(pred, succ);
		}
	}
}

void SchedulerDAG::findSCCBBs(Function &F) {
	if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=2)
		errs() << "\n\n# DEBUG_TMR=2 - SCC\n";
	errs() << "SCCs for " << F.getName() << " in post-order:\n";
	//for (scc_iterator<Function *> I = scc_begin(&F), IE = scc_end(&F); I != IE; ++I) {
	for (scc_iterator<Function *> I = scc_begin(&F); !I.isAtEnd(); ++I) {
		const std::vector<BasicBlock*> &SCCBBs = *I;
		if (SCCBBs.size()==1)
			continue;
		errs() << "  SCC:\n";
		for (std::vector<BasicBlock*>::const_iterator BBI = SCCBBs.begin(),
				BBIE = SCCBBs.end(); BBI != BBIE; ++BBI) {
			errs() << "    ->" << getLabel(*BBI) << "\n";
		}
	}
}

unsigned SchedulerDAG::getInstructionArea(Instruction *instr) {
	//InstructionNode *iNode = getInstructionNode(instr);
   	std::string opName = LEGUP_CONFIG->getOpNameFromInst(instr, alloc);
	if (opName.empty() || isMem(instr))
		return 0;

	Operation *Op = LEGUP_CONFIG->getOperationRef(opName);
	assert(Op);

	return Op->getLUTs();
}

bool SchedulerDAG::isBackwardEdge(std::pair<const BasicBlock*, const BasicBlock*> edge) {
	for (unsigned i = 0, e = BackEdges.size(); i != e; ++i) {
		if (edge == BackEdges[i])
			return true;
	}

	return false;
}

void SchedulerDAG::dfs(int n, int capacity[][MAX_NODE], int flow[][MAX_NODE], int s, bool visited[]) {
	visited[s] = true;
	for (int i=0; i<n; i++) {
		if (capacity[s][i]-flow[s][i]>0 && !visited[i])
			dfs(n, capacity, flow, i, visited);
	}
}

bool SchedulerDAG::bfs(int n, int start, int target,
			int capacity[][MAX_NODE], int flow[][MAX_NODE], int pred[]) {
	bool visited[MAX_NODE];
	for(int i=0; i<MAX_NODE; i++)
		visited[i] = false;
	std::queue<int> que;

	que.push(start);
	visited[start] = true;
	pred[start] = -1;
	while (!que.empty()) {
		int u = que.front();
		que.pop();
		for (int v=0; v<n; v++) {
			if(!visited[v] && capacity[u][v]-flow[u][v]>0) {
				que.push(v);
				visited[v] = true;
				pred[v] = u;
			}
		}
	}
	//fprintf(stderr, "---------------------------------------\n");
	//fprintf(stderr, "         ");
	//for(int i=start; i<=target; i++)
	//	fprintf(stderr, "%4d", i);
	//fprintf(stderr, "\n");
	//fprintf(stderr, "visited: ");
	//for(int i=start; i<=target; i++)
	//	fprintf(stderr, "%4d", visited[i]);
	//fprintf(stderr, "\n");
	//fprintf(stderr, "pred   : ");
	//for(int i=start; i<=target; i++)
	//	fprintf(stderr, "%4d", pred[i]);
	//fprintf(stderr, "\n");
	return (visited[target]==true);
}

void SchedulerDAG::connectDFGInst(int capacity[][MAX_NODE], Instruction *I, int t) {
	int idx = instMap[I];
	capacity[idx][idx+1] = 1;//isa<LoadInst>(I)? INF : 1;
	
	for (User::op_iterator op = I->op_begin(), e = I->op_end(); op != e; ++op) {
	    // we only care about operands that are created by other instructions
	    Instruction *dep = dyn_cast<Instruction>(*op);

	    // also ignore if the dependency is an alloca
	    if (!dep)// || isa<AllocaInst>(dep))
	        continue;

		//if (instMap[dep]!=0 || instMap[I]!=t) {
			int d = instMap[dep];
			capacity[d+1][idx] = INF;
			//errs() << getValueStr(dep) << "->" << getValueStr(I) << "\n";
			//errs() << instMap[dep] << "->" << instMap[I] << "\n";
		//}
	}

	if (isa<LoadInst>(I)) {
		for (VINST::iterator d = storeInsts.begin();
		                     d != storeInsts.end(); ++d) {
			Instruction *dep = const_cast<Instruction *>(*d);
        	if (hasDependencyAA(dep, I) /*&& (instMap[dep]!=0 || instMap[I]!=t)*/) {
				int c = instMap[dep];
				capacity[c+1][idx] = INF;
				//errs() << getValueStr(I) << "<-" << getValueStr(dep) << "\n";
				//errs() << bbMap[dep->getParent()] << "(LD)->" << b << "\n";
			}
		}
	}
}

void SchedulerDAG::connectDFGBB(int capacity[][MAX_NODE], const Instruction *Inst,
			int s, int t) {
	Instruction *I = const_cast<Instruction *>(Inst);
	if (skipInst(I))
		return;

	const BasicBlock *BB = I->getParent();
	int b = bbMap[BB];
	int i = instMap[I];

	capacity[b][i] = INF;
	capacity[i][i+1] = 1;//isa<LoadInst>(I)? INF : 1;
	
	for (User::op_iterator op = I->op_begin(), e = I->op_end(); op != e; ++op) {
	    // we only care about operands that are created by other instructions
	    Instruction *dep = dyn_cast<Instruction>(*op);

	    // also ignore if the dependency is an alloca
	    if (!dep)// || isa<AllocaInst>(dep))
	        continue;

		if (BB != dep->getParent() &&
				(bbMap[dep->getParent()]!=s || bbMap[BB]!=t)) {
			int c = instMap[dep];
			capacity[c+1][b] = INF;
			//errs() << getValueStr(dep) << "->" << getLabel(I->getParent()) << "\n";
			//errs() << bbMap[dep->getParent()] << "->" << b << "\n";
		}
	}

	if (isa<LoadInst>(I)) {
		for (VINST::iterator d = storeInsts.begin();
		                     d != storeInsts.end(); ++d) {
			Instruction *dep = const_cast<Instruction *>(*d);
        	if (hasDependencyAA(dep, I) &&
				(bbMap[dep->getParent()]!=s || bbMap[BB]!=t)) {
				int c = instMap[dep];
				capacity[c+1][b] = INF;
				//errs() << getValueStr(I) << "<-" << getValueStr(dep) << "\n";
				//errs() << bbMap[dep->getParent()] << "(LD)->" << b << "\n";
			}
		}
	}
}

bool SchedulerDAG::skipInst(const Instruction *I) {
	//if (isa<AllocaInst>(I))
	//	return true;
	if (isaDummyCall(I))
		return true;

	// ignore instructions with no uses
	if (I->hasNUses(0) && !isa<ReturnInst>(I) && !isa<StoreInst>(I))
		return true;

	return false;
}

void SchedulerDAG::makeDFGInstGraph(int capacity[][MAX_NODE], int n) {
	for(int i=0; i<n; i++)
		for(int j=0; j<n; j++)
			capacity[i][j] = 0;

	// make INF path within merged nodes
	for (VINST::iterator i = insts.begin(); i != insts.end(); ++i) {
		int instIdx = instMap[*i];
		int state = instPartState[*i];
		if (state==PART_S || state==PART_S_FINISH)
			capacity[0][instIdx] = INF;
		else if (state==PART_T || state==PART_T_FINISH)
			capacity[instIdx][n-1] = INF;
	}

	// connectInsts
	for (VINST::iterator i = insts.begin(); i != insts.end(); ++i) {
		connectDFGInst(capacity, const_cast<Instruction*>(*i), n-1);
	}

	//dumpNF(capacity, n);
	assert(n<MAX_NODE);
}

int SchedulerDAG::makeDFGBBGraph(int capacity[][MAX_NODE], int entryBB) {
	//make graph
	//int s, t;
	int n = entryBB;//+1;
	bbMap.clear();
	//int lastBB = 0;
	//for (Function::iterator b = F.begin(); b != F.end(); ++b) {
    //    if (isEmptyFirstBB(b))
	//		continue;
	//	if (boundaryBB != NULL && getLabel(b) == getLabel(boundaryBB))
	//		continue;

	//	bbMap[b] = n;
	//	lastBB = n;
	//	n++;

	//	for (BasicBlock::iterator i = (*b).begin(); i != (*b).end(); ++i) {
	//		instMap[i] = n;
	//		n+=2;
	//		if (isa<StoreInst>(i))
	//			storeInsts.push_back(i);
	//	}
	//	//lastInst = n;
	//}
	//if(boundaryBB) {
	//	bbMap[boundaryBB] = frontMerge? 0 : lastBB;
	//	for (BasicBlock::iterator i = boundaryBB->begin(); i != boundaryBB->end(); ++i) {
	//		instMap[i] = n;
	//		n+=2;
	//	}
	//}

	for (VBB::iterator b = bbs.begin(); b != bbs.end(); ++b) {
		bbMap[*b] = n++;
	}
	//for (VBB::iterator b = bbs.begin(); b != bbs.end(); ++b) {
	//	if (bbPartState[*b] == PART_UNKNOWN)
	//		bbMap[*b] = n++;
	//}
	//bool lastExist = false;
	//for (VBB::iterator b = bbs.begin(); b != bbs.end(); ++b) {
	//	if (bbPartState[*b] == PART_S) {
	//		bbMap[*b] = entryBB; 
	//	} else if (bbPartState[*b] == PART_T) {
	//		lastExist = true;
	//		bbMap[*b] = n;
	//	}
	//}
	//if (lastExist)
	//	n++;

	for(int i=0; i<n; i++)
		for(int j=0; j<n; j++)
			capacity[i][j] = 0;

	// make INF path within merged nodes
	for (VBB::iterator b = bbs.begin(); b != bbs.end(); ++b) {
		int bbIdx = bbMap[*b];
		int state = bbPartState[*b];
		if (state==PART_S || state==PART_S_FINISH)
			capacity[entryBB][bbIdx] = INF;
		else if (state==PART_T || state==PART_T_FINISH)
			capacity[bbIdx][n-1] = INF;
	}

	for (VBB::iterator b = bbs.begin(); b != bbs.end(); ++b) {
		//int state = bbPartState[*b];
		//if (state==PART_S_FINISH || state==PART_T_FINISH)
		//	continue;
		for (BasicBlock::const_iterator i = (*b)->begin(); i != (*b)->end(); ++i)
			connectDFGBB(capacity, i, entryBB, n-1);
	}

	//dumpNF(capacity, n);
	assert(n<MAX_NODE);

	return n;
}

void SchedulerDAG::dumpFlow(int flow[][MAX_NODE], int s, int t, int max_flow) {
	int resi[MAX_NODE][MAX_NODE];
	for (int i=0; i<=t; i++) {
		for (int j=0; j<=t; j++) {
			resi[i][j] = flow[i][j];
		}
	}

	for (int i=0; i<max_flow; i++) {
		fprintf(stderr, "max_flow[%d]\n", i);
		std::queue<int> q;
		int k = s;
		q.push(k);
		while (k != t) {
			for (int j=0; j<=t; j++) {
				if (resi[k][j]>0) {
					resi[k][j]--;
					k = j;
					q.push(k);
				}
			}
		}

		while (!q.empty()) {
			int k = q.front();
			const BasicBlock *b = isBBNode(k);
			if (b) {
				errs() << " - [" << bbMap[b] << "] " << getLabel(b) << "\n";
			}
			q.pop();
		}
	}
}

void SchedulerDAG::dumpNF(int capacity[][MAX_NODE], int n) {
	errs() << "\n# basicblock\n";
	for (VBB::iterator b = bbs.begin(); b != bbs.end(); ++b) {
		fprintf(stderr, "[%2d]  ", bbMap[*b]);
		errs() << getLabel(*b);
		fprintf(stderr, " (%d)\n", bbArea[*b]);
	}

	//errs() << "\n# instruction\n";
	//for (VBB::iterator b = bbs.begin(); b != bbs.end(); ++b) {
	//	for (BasicBlock::const_iterator i = (*b)->begin(); i != (*b)->end(); ++i) {
	//		if (skipInst(i))
	//			continue;
	//		fprintf(stderr, "[%2d]", instMap[i]);
	//		errs() << getValueStr(i) <<"\n";
	//	}
	//}

	//errs() << "\n# capacity[n][n]\n";
	//fprintf(stderr, "     ");
	//for(int i=0; i<n; i++)
	//	fprintf(stderr, "%d ", i%10);
	//fprintf(stderr, "\n");
	//for(int i=0; i<n; i++) {
	//	fprintf(stderr, "[%2d] ", i);
	//	for(int j=0; j<n; j++) {
	//		if(capacity[i][j]==0) fprintf(stderr, "  ");
	//		else if(capacity[i][j]==INF) fprintf(stderr, "x ");
	//		else fprintf(stderr, "%d ", capacity[i][j]);
	//	}
	//	errs() << "\n";
	//}
}

int SchedulerDAG::initBBArea(Function &F) {
	bbArea.clear();
	unsigned totalArea = 0;
	int maxArea = 0;

	errs() << "\n# BB area\n";
	for (Function::iterator b = F.begin(); b != F.end(); ++b) {
        if (isEmptyFirstBB(b))
			continue;

		int pArea = 0;
		for (BasicBlock::iterator i = (*b).begin(); i != (*b).end(); ++i) {
			pArea += getInstructionArea(i);
		}
		bbArea[b] = pArea;
		if (maxArea < pArea)
			maxArea = pArea;
		
		errs() << "    " << getLabel(b) << "(" << bbArea[b] << ")\n";
		
		totalArea += pArea;
	}
	setcArea(maxArea);
	errs() << "Area total= " << totalArea << "\n";

	return totalArea;
}

int SchedulerDAG::getInstArea(VINST ilist) {
	int pArea = 0;
	for(VINST::iterator i=ilist.begin(); i!=ilist.end(); ++i)
		pArea += getInstructionArea(const_cast<Instruction*>(*i));

	return pArea;
}

int SchedulerDAG::getBBArea(VBB blist) {
	int pArea = 0;
	for(VBB::iterator b=blist.begin(); b!=blist.end(); ++b)
		pArea += bbArea[*b];

	return pArea;
}

bool SchedulerDAG::isBalanced(VINST p0, VINST p1) {
	if (getInstArea(p0)-getInstArea(PART_S_FINISH) < getcArea()
			&& getInstArea(p1)-getInstArea(PART_T_FINISH) < getcArea())
		return true;

	return false;
}

bool SchedulerDAG::isBalanced(int totalArea, VBB p0, VBB p1) {
	int cArea = getcArea();
	int p0Area = getBBArea(p0);
	int p1Area = getBBArea(p1);
	//errs() << "p0Area= " << p0Area << "\n";
	//errs() << "p1Area= " << p1Area << "\n";

	//if (totalArea/2 + margin < p0Area)
	//	return false;
	//if (totalArea/2 + margin < p1Area)
	//	return false;
	//return true;

	if (p0Area<cArea && p1Area<cArea)
		return true;

	return false;
}

int SchedulerDAG::getMinGraphSize(Function &F) {
	int instCount = 0;
	for (Function::iterator b = F.begin(); b != F.end(); ++b) {
        if (isEmptyFirstBB(b))
			continue;

		for (BasicBlock::iterator i = (*b).begin(); i != (*b).end(); ++i) {
			instCount++;
		}
	}
	return instCount*2 + 2;
}

const BasicBlock *SchedulerDAG::getBoundaryBB(int boundaryEdge,
				int flow[][MAX_NODE], bool &frontMerge, int s, int n) {
	if (boundaryEdge==-1 || n==0)
		return NULL;

	const Instruction *inst = isInstNode(boundaryEdge);

	const BasicBlock *succ = NULL;
	for(int j=s; j<n; j++) {
		if(flow[boundaryEdge+1][j]>0) {
			succ = isBBNode(j);
			if (bbPartState[succ]==PART_UNKNOWN) {
				break;
			}
		}
	}

	const BasicBlock *pred = inst->getParent();

	//errs() << "predBB: " << getLabel(pred) << "\n";
	//errs() << "boundaryEdge: " << getValueStr(inst) << "\n";
	//errs() << "succBB: " << getLabel(succ) << "\n";

	if (frontMerge && bbPartState[succ]==PART_UNKNOWN) {
		return succ;
	} else if (bbPartState[pred]==PART_UNKNOWN) {
		frontMerge = false;
		return pred;
	}

	return NULL;
}

void SchedulerDAG::dumpbbPartState(std::string str) {
	int sArea = 0;
	int tArea = 0;
	errs() << "\n#" << str << "\n";
	for (VBB::iterator b = bbs.begin(); b != bbs.end(); ++b) {
		errs() << "[" << bbMap[*b] << "] = " << bbPartState[*b]
		       << " " << getLabel(*b) << "\n";
		if (bbPartState[*b]==PART_S)
			sArea += bbArea[*b];
		else if (bbPartState[*b]==PART_T)
			tArea += bbArea[*b];
	}
	errs() << "sArea=" << sArea << ", tArea=" << tArea << "\n";
}

int SchedulerDAG::initInstMap(Function &F) {
	int n=0;
	for (Function::iterator b = F.begin(); b != F.end(); ++b) {
        if (isEmptyFirstBB(b))
			continue;

		for (BasicBlock::iterator i = (*b).begin(); i != (*b).end(); ++i) {
			if (skipInst(i))
				continue;

			insts.push_back(i);

			instMap[i] = n;
			instPartState[i] = PART_UNKNOWN;

			n+=2;
			if (isa<StoreInst>(i))
				storeInsts.push_back(i);
		}
	}
	assert(isa<ReturnInst>(isInstNode(n-2)));
	return n;
}

int SchedulerDAG::initMap(Function &F) {
	int n=0;
	for (Function::iterator b = F.begin(); b != F.end(); ++b) {
        if (isEmptyFirstBB(b))
			continue;

		bbs.push_back(b);
		//if (n==0) {
		//	bbPartState[b] = PART_S;
		//} else {
			bbPartState[b] = PART_UNKNOWN;
		//}

		for (BasicBlock::iterator i = (*b).begin(); i != (*b).end(); ++i) {
			if (skipInst(i))
				continue;

			instMap[i] = n;
			n+=2;
			if (isa<StoreInst>(i))
				storeInsts.push_back(i);
		}
	}
	return n;
}

int SchedulerDAG::getInstArea(PART_STATE s) {
	int area = 0;
	for (VINST::iterator i=insts.begin(); i!=insts.end(); ++i) {
		if (instPartState[*i] == s)
			area += getInstructionArea(const_cast<Instruction*>(*i));
	}
	return area;
}

int SchedulerDAG::getBBArea(PART_STATE s) {
	int area = 0;
	for (VBB::iterator b=bbs.begin(); b!=bbs.end(); ++b) {
		if (bbPartState[*b] == s)
			area += bbArea[*b];
	}
	return area;
}

void SchedulerDAG::mergeNodes(bool frontMerge,
			const Instruction *boundaryInst, VINST p0, VINST p1) {
	int cArea = getcArea();

	// merge start and target nodes
	if (getInstArea(p0)-getInstArea(PART_S_FINISH) < cArea) {
		for (VINST::iterator i=p0.begin(); i!=p0.end(); ++i)
			if (instPartState[*i] == PART_UNKNOWN)
				instPartState[*i] = PART_S;
	}
	if (getInstArea(p1)-getInstArea(PART_T_FINISH) < cArea) {
		for (VINST::iterator i=p1.begin(); i!=p1.end(); ++i)
			if (instPartState[*i] == PART_UNKNOWN)
				instPartState[*i] = PART_T;
	}

	// boundary node merge
	int bArea = getInstructionArea(const_cast<Instruction*>(boundaryInst));
	if (frontMerge) {
		if (getInstArea(PART_S)+bArea > cArea)
			pushNewInstPartition(PART_S);
		instPartState[boundaryInst] = PART_S;
	} else {
		if (getInstArea(PART_T)+bArea > cArea)
			pushNewInstPartition(PART_T);
		instPartState[boundaryInst] = PART_T;
	}
}

bool SchedulerDAG::checkAreaConstraint(bool frontMerge,
			const BasicBlock *boundaryBB, VBB p0, VBB p1) {
	assert(boundaryBB);
	int cArea = getcArea();
	bool ret = false;

	// merge start and target nodes
	if (getBBArea(p0)-getBBArea(PART_S_FINISH) < cArea) {
		for (VBB::iterator b=p0.begin(); b!=p0.end(); ++b)
			if (bbPartState[*b] == PART_UNKNOWN)
				bbPartState[*b] = PART_S;
	} else if (getBBArea(p1)-getBBArea(PART_T_FINISH) < cArea) {
		for (VBB::iterator b=p1.begin(); b!=p1.end(); ++b)
			if (bbPartState[*b] == PART_UNKNOWN)
				bbPartState[*b] = PART_T;
	}

	//if (boundaryBB == NULL) {
	//	if (getBBArea(PART_S) < getBBArea(PART_T) && getBBArea(PART_T)>0) {
	//		pushNewPartition(PART_T);
	//		ret = true;
	//	} else if (getBBArea(PART_S)>0) {
	//		pushNewPartition(PART_S);
	//		ret = true;
	//	}
	//} else {
		if (frontMerge) {
			if (getBBArea(PART_S)+bbArea[boundaryBB] > cArea) {
				pushNewPartition(PART_S);
				ret = true;
			}
			bbPartState[boundaryBB] = PART_S;
		} else {
			if (getBBArea(PART_T)+bbArea[boundaryBB] > cArea) {
				pushNewPartition(PART_T);
				ret = true;
			}
			bbPartState[boundaryBB] = PART_T;
		}
	//}
	
	return ret;
}

void SchedulerDAG::dumpVBB(VBB blist, std::string str) {
	int area = 0;
	errs() << str << ": ";
	for (VBB::iterator b=blist.begin(); b!=blist.end(); ++b) {
		errs() << getLabel(*b) << " - ";
		area += bbArea[*b];
	}
	errs() << "(" << area << ")\n";
}

void SchedulerDAG::networkFlowPartitionInsts(Function &F) {
	int n = initInstMap(F);

	unsigned k = LEGUP_CONFIG->getParameterInt("NUMBER_OF_PARTITIONS");
	unsigned p = LEGUP_CONFIG->getParameterInt("PART_AREA_MARGIN_PERCENTAGE");
	do {
		unsigned areaLimit = getLimitAreaByPercentage(F, p);
		networkFlowPartitionInsts(F, areaLimit, n);
		if (InstPartitions.size()>k) {
			p += 5;
			errs() << " - WARNING: Number of Partitions: " << InstPartitions.size()
			       << " (expected = " << k << ")"
			       << " / forced to increase area limit by 5%\n";
		} else if (InstPartitions.size()<k) {
			p -= 5;
			errs() << " - WARNING: Number of Partitions: " << InstPartitions.size()
			       << " (expected = " << k << ")"
			       << " / forced to decrease area limit by 5%\n";
		}
		assert(p<=100);
	} while (InstPartitions.size()!=k);
}

void SchedulerDAG::networkFlowPartitionInsts(Function &F, unsigned areaLimit, int n) {
	int capacity[MAX_NODE][MAX_NODE];
	int flow[MAX_NODE][MAX_NODE];

	InstPartitions.clear();
	instPartState.clear();
	//instMap.clear();
	//storeInsts.clear();

	setcArea(areaLimit);
	//instPartState[isInstNode(0)] = PART_S_FINISH;
	//instPartState[isInstNode(n-2)] = PART_T_FINISH;

	VINST p0;
	VINST p1;

	int itrCount = 0;
	const Instruction *boundaryInst = NULL;
	bool frontMerge;
	do {
		makeDFGInstGraph(capacity, n);
		maxFlow(n, 0, n-1, capacity, flow);
		boundaryInst = getBoundaryInst(n, capacity, flow, p0, p1, frontMerge);
		if (boundaryInst)
			errs() << "boundaryInst = " << getValueStr(boundaryInst) << "\n";

		if (isBalanced(p0, p1) || boundaryInst == NULL) {
			pushNewInstPartition(p0, PART_S_FINISH);
			pushNewInstPartition(p1, PART_T_FINISH);
			break;
		}

		mergeNodes(frontMerge, boundaryInst, p0, p1);
	} while( itrCount++ < n );
}

void SchedulerDAG::networkFlowPartitionBBs(Function &F) {
	//Ford-Fulkerson Algorithm source from
	//http://aduni.org/courses/algorithms/courseware/handouts/Reciation_09.html

	int capacity[MAX_NODE][MAX_NODE];
	int flow[MAX_NODE][MAX_NODE];
	int n = 0;

	bbPartState.clear();
	//bbs.clear();
	//startNodes.clear();
	//targetNodes.clear();
	storeInsts.clear();
	bbMap.clear();
	instMap.clear();

	int totalArea = initBBArea(F);
	// 2-way partition
	unsigned pArea = getLimitAreaByPercentage(F);
	unsigned cArea = getcArea();
	errs() << "Area constraint (user)= " << pArea << "\n";
	errs() << "Area constraint (max bb)= " << cArea << "\n";
	if (pArea>cArea)
		setcArea(pArea);

	//int minGraphSize = getMinGraphSize(F);
	//int boundaryEdge = -1;
	VBB p0;
	VBB p1;
	//int start = 0;
	//int target = 0;
	int entryBB = 0;

	entryBB = initMap(F);
	int nodeSize = bbPartState.size();
	if (nodeSize==1) {
		return;
	}

	int itrCount = 0;
	const BasicBlock *boundaryBB = NULL;
	bool frontMerge;
	do {
		n = makeDFGBBGraph(capacity, entryBB);

		//errs() << "LastInst = " << target << "\n";
		//if (n<=minGraphSize) {
		//	errs() << "Error: Fail to find balanced partitioning\n";
		//	break;
		//}
		maxFlow(n, entryBB, n-1, capacity, flow);
		boundaryBB = getBoundaryBB(n, entryBB, n-1, capacity, flow, 
				p0, p1, frontMerge);
		if (boundaryBB) {
			errs() << "*boundaryBB= " << getLabel(boundaryBB) << "\n";
			errs() << "*frontMerge= " << frontMerge << "\n";
		}

		if (isBalanced(totalArea, p0, p1)) {
			Partitions.push_back(p0);
			Partitions.push_back(p1);
			break;
		}
		if (boundaryBB == NULL) {
			//assert(isEmptyCand());
			pushNewPartition(PART_S);
			pushNewPartition(PART_T);
			break;
		}
		//if (bbMap[boundaryBB]==entryBB) {
		//	errs() << "Error: Reach entryBB without balanced partitioning\n";
		//	break;
		//}
		//if (bbMap[boundaryBB]==n-1) {
		//	errs() << "Error: Reach exitBB without balanced partitioning\n";
		//	break;
		//}

		checkAreaConstraint(frontMerge, boundaryBB, p0, p1);
		//setPartitions(boundaryBB, frontMerge);
	} while( itrCount++ < nodeSize );
}

bool SchedulerDAG::isEmptyCand() {
	for (VBB::iterator b = bbs.begin(); b != bbs.end(); ++b) {
		if (bbPartState[*b] == PART_UNKNOWN) {
			return false;
		}
	}
	return true;
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
	for (VINST::iterator i = v.begin(); i != v.end(); ++i) {
		if (instPartState[*i] != s) {
			pNodes.push_back(*i);
		}
	}
	errs() << "Push partitions " << (s==PART_S_FINISH? "p0" : "p1") << "\n";
	InstPartitions.push_back(pNodes);
}

void SchedulerDAG::pushNewPartition(PART_STATE s) {
	VBB pNodes;
	for (VBB::iterator b = bbs.begin(); b != bbs.end(); ++b) {
		if (bbPartState[*b] == s) {
			pNodes.push_back(*b);
			bbPartState[*b] = (s==PART_S)? PART_S_FINISH : PART_T_FINISH;
		}
	}
	dumpVBB(pNodes, "#PushPartitions");
	Partitions.push_back(pNodes);
}

void SchedulerDAG::pushPlist(VBB &plist, VBB pNodes) {
	for (VBB::iterator b=pNodes.begin(); b!=pNodes.end(); ++b) {
		plist.push_back(*b);
	}
}

//void SchedulerDAG::setPartitions(const BasicBlock *boundaryBB, bool frontMerge) {
//	if (boundaryBB==NULL)
//		return;
//
//	bbs.erase(std::find(bbs.begin(), bbs.end(), boundaryBB));
//	if (frontMerge) {
//		startNodes.push_back(boundaryBB);
//	} else {
//		targetNodes.push_back(boundaryBB);
//	}
//}

const Instruction* SchedulerDAG::getBoundaryInst(int n, 
			int capacity[][MAX_NODE], int flow[][MAX_NODE],
			VINST &p0, VINST &p1, bool &frontMerge) {
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

	int minCut = 0;
	for(int i=0; i<n; i+=2) {
		if ((visited[i] && !visited[i+1]) && capacity[i][i+1]==1) {
			minCut++;
		}
	}
	fprintf(stderr, "  %4d / %4d | %4d / %4d | %4d | %4d / %4d | minCut= %2d\n",
			getInstArea(PART_S_FINISH), getInstArea(PART_T_FINISH),
			getInstArea(PART_S), getInstArea(PART_T),
			getInstArea(PART_UNKNOWN), 
			getInstArea(p0), getInstArea(p1), minCut);

	frontMerge = getInstArea(p0) < getInstArea(p1);
	for(int i=0; i<n; i+=2) {
		if ((visited[i] && !visited[i+1]) && capacity[i][i+1]==1) {
			for (int j=2; j<n-2; j++) {
				int f = frontMerge? flow[i+1][j] : flow[j][i];
				if (f) {
					const Instruction *d = isInstNode(j);
					if (instPartState[d]==PART_UNKNOWN) {
						boundaryInst = d;
						break;
					}
				}
			}
		}
	}

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

	return boundaryInst;
}

int SchedulerDAG::maxFlow(int n, int start, int target,
			int capacity[][MAX_NODE], int flow[][MAX_NODE]) {
	int pred[MAX_NODE];

	int max_flow = 0;
	for(int i=0; i<n; i++)
		for(int j=0; j<n; j++)
			flow[i][j] = 0;
	while (bfs(n, start, target, capacity, flow, pred)) {
		//Determin the amount by which we can increment the flow
		int increment = INF;
		for(int u=target; pred[u]>=0; u=pred[u]) {
			int f = capacity[pred[u]][u] - flow[pred[u]][u];
			if (increment > f) {
				increment = f;
			}
		}
		//Now increment the flow
		for(int u=target; pred[u]>=0; u=pred[u]) {
			flow[pred[u]][u] += increment;
			flow[u][pred[u]] -= increment;
		}
		max_flow += increment;
	}
	//No aumenting path anymore. We are done.

	//dumpbbPartState("bb states");
	//fprintf(stderr, "\n# flow(%d->%d) (maxflow=%d)\n", start, target, max_flow);
	//dumpFlow(flow, start, target, max_flow);
	//fprintf(stderr, "     ");
	//for(int i=0; i<n; i++)
	//	fprintf(stderr, "%d ", i%10);
	//fprintf(stderr, "\n");
	//for(int i=0; i<n; i++) {
	//	fprintf(stderr, "[%2d] ", i);
	//	for(int j=0; j<n; j++) {
	//		if(flow[i][j]<=0) fprintf(stderr, "  ");
	//		else if(flow[i][j]==INF) fprintf(stderr, "x ");
	//		else fprintf(stderr, "%d ", flow[i][j]);
	//	}
	//	fprintf(stderr, "\n");
	//}

	return max_flow;
}

const BasicBlock* SchedulerDAG::getBoundaryBB(int n, int start, int target, 
			int capacity[][MAX_NODE], int flow[][MAX_NODE],
			VBB &p0, VBB &p1, bool &frontMerge) {
	const BasicBlock* boundaryBB = NULL;

	bool visited[MAX_NODE];
	for(int i=0; i<MAX_NODE; i++)
		visited[i] = false;
	dfs(target, capacity, flow, start, visited);

	//fprintf(stderr, "---------------------------------------\n");
	//fprintf(stderr, "         ");
	//for(int i=start; i<=target; i++)
	//	fprintf(stderr, "%4d", i);
	//fprintf(stderr, "\n");
	//fprintf(stderr, "visited: ");
	//for(int i=start; i<=target; i++)
	//	fprintf(stderr, "%4d", visited[i]);
	//fprintf(stderr, "\n");

	p0.clear();
	p1.clear();
	for (VBB::iterator b = bbs.begin(); b != bbs.end(); ++b) {
		int idx = bbMap[*b];
		if (visited[idx])
			p0.push_back(*b);
		else
			p1.push_back(*b);
	}
	//for(VBB::iterator b=startNodes.begin();
	//                                             b!=startNodes.end(); ++b) {
	//	errs() << "s: " << getLabel(*b) << "\n";
	//	if(std::find(p0.begin(), p0.end(), *b)==p0.end())
	//		p0.push_back(*b);
	//}
	//for(VBB::iterator b=targetNodes.begin();
	//                                             b!=targetNodes.end(); ++b) {
	//	errs() << "t: " << getLabel(*b) << "\n";
	//	if(std::find(p1.begin(), p1.end(), *b)==p1.end())
	//		p1.push_back(*b);
	//}

	errs() << "----part 1---- " << getBBArea(p0) << "/" << getcArea() << "\n";
	for(VBB::iterator b = p0.begin(); b != p0.end(); ++b)
		errs() << "  - " << getLabel(*b) << "\n";

	errs() << "----part 2---- " << getBBArea(p1) << "/" << getcArea() << "\n";
	for(VBB::iterator b = p1.begin(); b != p1.end(); ++b)
		errs() << "  - " << getLabel(*b) << "\n";

	frontMerge = getBBArea(p0) < getBBArea(p1);
	for(int i=0; i<start; i+=2) {
		//const Instruction *I = isInstNode(i);
		//if (I!=NULL && isa<StoreInst>(I))
		//	continue;
		if ((visited[i] && !visited[i+1]) && capacity[i][i+1]==1 /*&& (i!=start || j!=target)*/) {
			boundaryBB = getBoundaryBB(i, flow, frontMerge, start, n);
			if (boundaryBB) {
				//const Instruction *I = isInstNode(i);
				//errs() << "cand_boundaryBB= " << getLabel(boundaryBB) << getValueStr(I) << "\n";
				//if ((frontMerge && bbMap[boundaryBB]!=n-1) ||
				//		(!frontMerge && bbMap[boundaryBB]!=start))
					break;
			}
		}
	}

	//Select min-area boundaryBB from the out of min-cut list
	if (boundaryBB == NULL && p0.size()>1) {
		unsigned minArea = -1;
		for(VBB::iterator b = p0.begin(); b != p0.end(); ++b) {
			if (bbPartState[*b] == PART_UNKNOWN) {
				if ((unsigned)bbArea[*b]<minArea) {
					minArea = bbArea[*b];
					frontMerge = true;
					boundaryBB = *b;
				}
			}
		}
	}
	if (boundaryBB == NULL && p1.size()>1) {
		unsigned minArea = -1;
		for(VBB::iterator b = p1.begin(); b != p1.end(); ++b) {
			if (bbPartState[*b] == PART_UNKNOWN) {
				if ((unsigned)bbArea[*b]<minArea) {
					minArea = bbArea[*b];
					frontMerge = false;
					boundaryBB = *b;
				}
			}
		}
	}

	//for(int i=1; i<n; i++) {
	//	const Instruction* inst = isInstNode(i);
	//	if(!inst)
	//		continue;

	//	int f = 0;
	//	for(int j=0; j<n; j++)
	//		if(flow[i][j]>0)
	//			f += flow[i][j];
	//	if(f==max_flow) {
	//		boundaryEdge = i;
	//		errs() << "boundaryEdge = " << boundaryEdge << "\n";
	//		break;
	//	}
	//}

	return boundaryBB;
}

const Instruction* SchedulerDAG::isInstNode(int idx) {
	for (MINST::iterator i = instMap.begin();
			i != instMap.end(); ++i) {
		if (i->second==idx)
			return i->first;
	}
	return NULL;
}

const BasicBlock* SchedulerDAG::isBBNode(int i) {
	for (MBB::iterator b = bbMap.begin(); b != bbMap.end(); ++b) {
		if (b->second==i)
			return b->first;
	}
	return NULL;
}

void SchedulerDAG::findAllPaths(VBB path, const BasicBlock *current) {
	path.push_back(current);

	const TerminatorInst *tInst = current->getTerminator();
	if (tInst->getNumSuccessors()==0) {
		DAGPaths.push(path);
	}

	for (unsigned i = 0, nSucc = tInst->getNumSuccessors(); i < nSucc; ++i) {
		BasicBlock *succ = tInst->getSuccessor(i);
		std::pair<const BasicBlock*, const BasicBlock*> edge;
		edge = std::make_pair(current, succ);
		if (!isBackwardEdge(edge))
			findAllPaths(path, succ);
	}
}

void SchedulerDAG::findAllPaths(Function &F) {
	VBB path;
	findAllPaths(path, &F.getEntryBlock());
}

void SchedulerDAG::findTopologicalBBList(Function &F) {
	// Topologically sorted list - NOT consider backward edges
	//if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=2)
	//	errs() << "\n\n# DEBUG_TMR=2 - Find all paths\n";
	//errs() << "Basic blocks of " << F.getName() << " in post-order:\n";
	//for (po_iterator<BasicBlock *> I = po_begin(&F.getEntryBlock()),
	//                               IE = po_end(&F.getEntryBlock());
	//                               I != IE; ++I) {
	//	errs() << " " << getLabel(*I) << "\n";
	//}

	if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=2)
		errs() << "\n\n# DEBUG_TMR=2 - Topological Instruction List\n";

	// Topologically sorted list - consider backward edges
	//runToposort(F);

	// Topologically sorted list - skip backward edges
	VBB path;
	errs() << "Basic blocks of " << F.getName() << " in post-order:\n";
	for (po_iterator<BasicBlock *> I = po_begin(&F.getEntryBlock()),
	                               IE = po_end(&F.getEntryBlock());
	                               I != IE; ++I) {
		errs() << " " << getLabel(*I) << "\n";
		path.push_back(*I);
	}
	DAGPaths.push(path);
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
	if (areaMarginPercentage == 0)
		areaMarginPercentage = LEGUP_CONFIG->getParameterInt("PART_AREA_MARGIN_PERCENTAGE");
	unsigned areaConstraint = totalArea/nPart;
	unsigned areaMargin = areaConstraint*areaMarginPercentage/100;

	if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=2) {
		errs() << "\n\n# DEBUG_TMR=2 - Partition Area Information\n";
		errs() << "Total Area = " << totalArea << "\n";
		errs() << "Area Limit = " << areaConstraint + areaMargin << "\n";
	}
	return areaConstraint + areaMargin;
}

void SchedulerDAG::bfsPartitionInsts(Function &F) {
	unsigned k = LEGUP_CONFIG->getParameterInt("NUMBER_OF_PARTITIONS");
	unsigned p = LEGUP_CONFIG->getParameterInt("PART_AREA_MARGIN_PERCENTAGE");
	do {
		unsigned areaLimit = getLimitAreaByPercentage(F, p);
		bfsPartitionInsts(F, areaLimit);
		if (InstPartitions.size()>k) {
			p += 5;
			errs() << " - WARNING: Number of Partitions: " << InstPartitions.size()
			       << " (expected = " << k << ")"
			       << " / forced to increase area limit by 5%\n";
		} else if (InstPartitions.size()<k) {
			p -= 5;
			errs() << " - WARNING: Number of Partitions: " << InstPartitions.size()
			       << " (expected = " << k << ")"
			       << " / forced to decrease area limit by 5%\n";
		}
		assert(p<=100);
	} while (InstPartitions.size()!=k);
}

void SchedulerDAG::bfsPartitionInsts(Function &F, unsigned areaLimit) {
	InstPartitions.clear();

	// there is only one DAG path in bfs partition (topologicallly sorted)
	VBB path = DAGPaths.front();
	unsigned areaRecommand = 0;
	unsigned areaLimitViolation = false;
	unsigned areaTotal = 0;

	VINST partitionPath;
	for (VBB::const_reverse_iterator b = path.rbegin(),
	                                 be = path.rend();
	                                 b != be; ++b) {
		const BasicBlock *BB = *b;
		for (BasicBlock::const_iterator instr = BB->begin(), ie = BB->end();
		                                instr != ie; ++instr) {
			const Instruction *I = instr;
			//InstructionNode *iNode = getInstructionNode(const_cast<Instruction *>(I));
			unsigned area = getInstructionArea(const_cast<Instruction *>(I));
			if ((areaTotal+area > areaLimit) && (areaLimit!=0)) {
				if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=3)
					errs() << "  ---- Insert Part. Voter ----\n";
				if (!partitionPath.empty()) {
					//iNode->setPartition(true);
					InstPartitions.push_back(partitionPath);
					partitionPath.clear();
					areaTotal = 0;
				} else {
					areaLimitViolation = true;
					areaLimit = areaTotal + area + 1;
				}
			}
			areaTotal += area;
			if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=3) {
				errs() << "  Area=(" << areaTotal << ") "
				          << getLabel(*b) << ":" << getValueStr(instr) << "\n";
			}
			partitionPath.push_back(I);
		}
		if (areaRecommand < areaTotal)
			areaRecommand = areaTotal;
	}
	if (!partitionPath.empty()) {
		InstPartitions.push_back(partitionPath);
	}

	if (areaLimitViolation) {
		unsigned areaLimit = LEGUP_CONFIG->getParameterInt("PARTITION_AREA_LIMIT");
		errs() << "Error: PARTITION_AREA_LIMIT=" << areaLimit << " is too small\n";
		errs() << "       Recommanded area = (larger than) " << (areaRecommand) << "\n";
		//assert(0);
	}

}

bool SchedulerDAG::isPredPartition(VBB path, const BasicBlock *useBB) {
	for (VBB::const_iterator b = path.begin(),
	                                                     be = path.end();
	                                                     b != be; ++b) {
		if (*b == useBB)
			return true;
	}
	return false;
}

void SchedulerDAG::findInstPartitionSignals() {
	if (InstPartitions.empty())
		return;

	MINST pMap;
	int pIdx = 0;
	for (std::vector<VINST>::const_iterator p = InstPartitions.begin(),
	                                        pe = InstPartitions.end(); p != pe; ++p) {
		for (VINST::const_iterator i = (*p).begin(),
		                           ie = (*p).end(); i != ie; ++i) {
			pMap[*i] = pIdx;
		}
		pIdx++;
	}

	for (std::vector<VINST>::const_iterator p = InstPartitions.begin(),
	                                        pe = InstPartitions.end(); p != pe; ++p) {
		VINST path = *p;
		for (VINST::const_iterator i = path.begin(),
		                           ie = path.end(); i != ie; ++i) {
			Instruction *I = const_cast<Instruction*>(*i);
    		if (isa<AllocaInst>(I))// || isa<PHINode>(I))
    		    continue;

			if (isa<ReturnInst>(I)) {
   	    		InstructionNode *iNode = getInstructionNode(I);
				iNode->setPartition(true);
				continue;
			}

    		for (User::op_iterator op = I->op_begin(); op != I->op_end(); ++op) {
    		    // we only care about operands that are created by other instructions
    		    Instruction *dep = dyn_cast<Instruction>(*op);

    		    // also ignore if the dependency is an alloca
    		    if (!dep || isa<AllocaInst>(dep))
    		        continue;

				if (pMap[dep] < pMap[I]) {
    	    		InstructionNode *depNode = getInstructionNode(dep);
					depNode->setPartition(true);
				}
    		}
		}
	}
}

void SchedulerDAG::findPartitionSignals() {
	if (Partitions.empty())
		return;

	for (std::vector<VBB>::const_iterator p = Partitions.begin(),
	                                      pe = Partitions.end(); p != pe; ++p) {
		VBB path = *p;
		for (VBB::const_iterator b = path.begin(),
		                         be = path.end(); b != be; ++b) {
			BasicBlock *BB = const_cast<BasicBlock *>(*b);
    		for (BasicBlock::iterator I = BB->begin(), ie = BB->end(); I != ie; ++I) {
    			// these instructions are not scheduled
    			if (isa<AllocaInst>(I))// || isa<PHINode>(I))
    			    continue;

    			for (User::op_iterator i = I->op_begin(), e = I->op_end(); i != e; ++i) {
    			    // we only care about operands that are created by other instructions
    			    Instruction *dep = dyn_cast<Instruction>(*i);

    			    // also ignore if the dependency is an alloca
    			    if (!dep || isa<AllocaInst>(dep))
    			        continue;

					if (dep->getParent() != BB || isa<ReturnInst>(I)) {
    		    		InstructionNode *depNode = getInstructionNode(dep);
						depNode->setPartition(true);
					}
    			}
    		}
		}
	}

	//VBB predPartBBs;
	//VBB path = Partitions.front();
	//for (VBB::const_iterator b = path.begin(),
	//                         be = path.end(); b != be; ++b) {
	//	predPartBBs.push_back(*b);
	//}
	//for (std::vector<VBB>::const_iterator p = Partitions.begin()+1,
	//                                      pe = Partitions.end(); p != pe; ++p) {
	//	VBB path = *p;
	//	for (VBB::const_iterator b = path.begin(),
	//	                         be = path.end(); b != be; ++b) {
	//		BasicBlock *BB = const_cast<BasicBlock *>(*b);
    //		for (BasicBlock::iterator I = BB->begin(), ie = BB->end(); I != ie; ++I) {
    //			// these instructions are not scheduled
    //			if (isa<AllocaInst>(I))// || isa<PHINode>(I))
    //			    continue;

	//			//if (isa<ReturnInst>(I)) {
	//			//	InstructionNode *INode = getInstructionNode(I);
	//			//	INode->setPartition(true);
	//			//	continue;
	//			//}

    //			for (User::op_iterator i = I->op_begin(), e = I->op_end(); i != e; ++i) {
    //			    // we only care about operands that are created by other instructions
    //			    Instruction *dep = dyn_cast<Instruction>(*i);

    //			    // also ignore if the dependency is an alloca
    //			    if (!dep || isa<AllocaInst>(dep))
    //			        continue;

    //			    // ignore operands from other basic blocks, these are
    //			    // guaranteed to be in another state
    //		    	InstructionNode *depNode = getInstructionNode(dep);
    //			    if (isPredPartition(predPartBBs, dep->getParent()))
	//					//if (!depNode->getBackward())
	//						depNode->setPartition(true);
    //			}
    //		}
	//	}
	//	for (VBB::const_iterator b = path.begin(),
	//	                         be = path.end(); b != be; ++b) {
	//		predPartBBs.push_back(*b);
	//	}
	//}
}

void SchedulerDAG::bfsPartitionBBs(Function &F) {
	unsigned pArea = getLimitAreaByPercentage(F);
	unsigned cArea = getcArea();
	errs() << "Area constraint (user)= " << pArea << "\n";
	errs() << "Area constraint (max bb)= " << cArea << "\n";
	unsigned areaLimit = pArea<cArea? cArea : pArea;
	unsigned areaTotal = 0;

	VBB path = DAGPaths.front();
	VBB partitionPath;
	for (VBB::const_reverse_iterator b = path.rbegin(),
	                                 be = path.rend();
	                                 b != be; ++b) {
        if (bbArea.find(*b) == bbArea.end())
            continue;
		if (areaTotal+bbArea[*b] > areaLimit) {
			if (!partitionPath.empty()) {
				Partitions.push_back(partitionPath);
				partitionPath.clear();
				areaTotal = 0;
			} else {
				assert(0);
			}
		}
		areaTotal += bbArea[*b];
		partitionPath.push_back(*b);
	}
	if (!partitionPath.empty()) {
		Partitions.push_back(partitionPath);
	}
}

void SchedulerDAG::revisitPartitions() {
	for (std::vector<VBB>::iterator p = Partitions.begin(),
	                                pe = Partitions.end(); p != pe; ++p) {
		VBB path = *p;
		int areaTotal = 0;
		for (VBB::const_iterator b = path.begin(),
		                         be = path.end(); b != be; ++b) {
			areaTotal += bbArea[*b];
		}
		if (areaTotal==0) {
			Partitions.erase(p);
		}
	}
}

bool SchedulerDAG::runOnFunction(Function &F, Allocation *_alloc) {
    assert(_alloc);
    alloc = _alloc;
    AliasAnalysis *AA = alloc->getAA();
    assert(AA);
    AliasA = AA;

	if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=2)
		errs() << "\n\n# DEBUG_TMR=2 ------ " << F.getName() << " ------\n";

    // generate Instruction to InstructionNode mapping
    for (Function::iterator b = F.begin(), be = F.end(); b != be; b++) {
		BasicBlockNode *bNode = new BasicBlockNode(b);
    	bbNodeLookup[b] = bNode;
        for (BasicBlock::iterator instr = b->begin(), ie = b->end();
             instr != ie; ++instr) {
    		InstructionNode *iNode = new InstructionNode(instr);
    		nodeLookup[instr] = iNode;
		}
	}

	// TMR - detect backward edges and mark on them (setBackward())
	if (LEGUP_CONFIG->getParameterInt("TMR")) {
		if (LEGUP_CONFIG->getParameterInt("SYNC_VOTER_MODE")!=0) {
			FindFunctionBackedges(F, BackEdges);
			insertSyncVoter(F);
		}

		//findSCCBBs(F);
		if (LEGUP_CONFIG->getParameterInt("PART_VOTER_MODE")==1) {
			initBBArea(F);
			findTopologicalBBList(F);
			bfsPartitionBBs(F);
			revisitPartitions();
		} else if (LEGUP_CONFIG->getParameterInt("PART_VOTER_MODE")==2) {
			//findAllPaths(F);
			networkFlowPartitionBBs(F);
			revisitPartitions();
		} else if (LEGUP_CONFIG->getParameterInt("PART_VOTER_MODE")==3) {
			findTopologicalBBList(F);
			bfsPartitionInsts(F);
		} else if (LEGUP_CONFIG->getParameterInt("PART_VOTER_MODE")==4) {
			networkFlowPartitionInsts(F);
		}
		if (LEGUP_CONFIG->getParameterInt("DEBUG_TMR")>=2) {
			if (LEGUP_CONFIG->getParameterInt("PART_VOTER_MODE")<=2) {
				errs() << "\n\n# DEBUG_TMR=2 - Partition List (n="
				       << Partitions.size() << ")\n";
				int cnt = 0;
				for (std::vector<VBB>::const_iterator p = Partitions.begin(),
				                                      pe = Partitions.end();
				                                      p != pe; ++p) {
					VBB path = *p;
					errs() << " [" << cnt++ << "] ";
					for (VBB::const_iterator b = path.begin(),
					                         be = path.end();
					                         b != be; ++b) {
						errs() << getLabel(*b) << " - ";
					}
					errs() << " (" << getBBArea(path) << ")\n";
				}
			} else {
				errs() << "\n\n# DEBUG_TMR=2 - Partition List (n="
				       << InstPartitions.size() << ")\n";
				int cnt = 0;
				for (std::vector<VINST>::const_iterator p = InstPartitions.begin();
				                                        p != InstPartitions.end(); ++p) {
					VINST path = *p;
					for (VINST::const_iterator i = path.begin();
					                           i != path.end(); ++i) {
						errs() << " [" << cnt << "] " << getValueStr(*i) << "\n";
					}
					errs() << " ---- Total area = " << getInstArea(path) << " ----\n";
					cnt++;
				}
			}
		}
	}

	// setPartition Bounday
	if (LEGUP_CONFIG->getParameterInt("PART_VOTER_MODE")<=2)
		findPartitionSignals();
	else
		findInstPartitionSignals();


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

	// make BB input & output
	//errs() << "---- BasicBlock inout analysis ----\n";
    //for (Function::iterator b = F.begin(), be = F.end(); b != be; b++) {
	//	//errs() << "---- BB_" << getLabel(b) << "\n";
	//	BasicBlockNode *bbNode = getBasicBlockNode(b);
    //    for (BasicBlock::iterator instr = b->begin(), ie = b->end();
    //         instr != ie; ++instr) {
	//		Instruction *I = instr;
	//		//InstructionNode *iNode = getInstructionNode(I);

	//		if (I->getType()->getTypeID() == Type::VoidTyID)
	//			continue;
	//		if (isa<AllocaInst>(I))
	//			continue;
	//		if (isaDummyCall(I))
	//			continue;
    //    	if (I->hasNUses(0))
    //    	    continue; // ignore instructions with no uses

	//		bbNode->addOutput(I);
	//		//errs() << "  output " << getValueStr(I) << ",\n";
    //		for (User::op_iterator i = I->op_begin(), e = I->op_end(); i != e; ++i) {
    //		    // we only care about operands that are created by other instructions
    //		    Instruction *dep = dyn_cast<Instruction>(*i);

   	//		    // also ignore if the dependency is an alloca
   	//		    if (!dep)// || isa<AllocaInst>(dep))
   	//		        continue;

	//			InstructionNode *depNode = getInstructionNode(dep);
	//			if (I->getParent() != dep->getParent()) {
	//				bbNode->addInput(dep);
	//				//errs() << "    input " << getValueStr(dep) << ",\n";
	//			}
	//			if (I->getParent()==dep->getParent() && (depNode->getBackward())) {
	//					//|| LEGUP_CONFIG->getParameterInt("SYNC_VOTER_MODE")==1))
	//				bbNode->addFeedbackInput(dep);
	//				//errs() << "    finput " << getValueStr(dep) << ",\n";
	//			}
   	//		}
    //    }
    //}

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

			//FIXME -- added for TMR
			if (LEGUP_CONFIG->getParameterInt("TMR") &&
					LEGUP_CONFIG->getParameterInt("SYNC_VOTER_MODE")==4) {
    			InstructionNode *iNode = dag->getInstructionNode(I);
				if (iNode->getBackward() && isa<PHINode>(I))
					delayState = 1;
				//if (LEGUP_CONFIG->getParameterInt("LOCAL_RAMS") &&
				//		LEGUP_CONFIG->getParameterInt("USE_REG_VOTER_FOR_LOCAL_RAMS") &&
				//		isa<LoadInst>(I)) {
				//	Allocation *alloc = dag->getAlloc();
				//	RAM *localRam = alloc->getLocalRamFromInst(I);
				//	if (localRam) {
				//		if (localRam->getScope() == RAM::LOCAL)
				//			delayState++;
				//	}
				//}
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
