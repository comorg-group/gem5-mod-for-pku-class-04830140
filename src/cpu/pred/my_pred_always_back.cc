//
//  my_pred_always_back.cc
//  gem5-mod-for-pku-class-04830140
//
//  Created by Jackie Yang on 2014-12-03.
//  Copyright (c) 2014 Jackie Yang. All rights reserved.
//

#include "base/intmath.hh"
#include "base/misc.hh"
#include "base/trace.hh"
#include "cpu/pred/my_pred_always_back.hh"
#include "debug/Fetch.hh"
#include "debug/Branch.hh"

#include "arch/isa_traits.hh"
#include "arch/types.hh"
#include "arch/utility.hh"
#include "base/trace.hh"
#include "config/the_isa.hh"
#include "cpu/pred/bpred_unit.hh"
#include "debug/Branch.hh"

BackBP::BackBP(const Params *params) :
	BPredUnit(params)
{

}

void
BackBP::reset()
{
}

void
BackBP::btbUpdate(Addr branch_addr, void * &bp_history)
{
// Place holder for a function that is called to update predictor history when
// a BTB entry is invalid or not found.
}

bool
BackBP::predictInOrder(StaticInstPtr &inst, const InstSeqNum &seqNum,
                          int asid, TheISA::PCState &instPC,
                          TheISA::PCState &predPC, ThreadID tid)
{
    // See if branch predictor predicts taken.
    // If so, get its target addr either from the BTB or the RAS.
    // Save off record of branch stuff so the RAS can be fixed
    // up once it's done.

	DPRINTF(Branch, "override function reached!");
    using TheISA::MachInst;

    bool pred_taken = false;
    TheISA::PCState target;

    ++lookups;
    DPRINTF(Branch, "[tid:%i] [sn:%i] %s ... PC %s doing branch "
            "prediction\n", tid, seqNum,
            inst->disassemble(instPC.instAddr()), instPC);

    void *bp_history = NULL;

    if (inst->isUncondCtrl()) {
        DPRINTF(Branch, "[tid:%i] Unconditional control.\n", tid);
        pred_taken = true;
        // Tell the BP there was an unconditional branch.
        uncondBranch(bp_history);

        if (inst->isReturn() && RAS[tid].empty()) {
            DPRINTF(Branch, "[tid:%i] RAS is empty, predicting "
                    "false.\n", tid);
            pred_taken = false;
        }
    } else {
        ++condPredicted;

        pred_taken = lookup(predPC.instAddr(), bp_history, asid);
    }

    PredictorHistory predict_record(seqNum, predPC.instAddr(), pred_taken,
                                    bp_history, tid);

    // Now lookup in the BTB or RAS.
    if (pred_taken) {
        if (inst->isReturn()) {
            ++usedRAS;

            // If it's a function return call, then look up the address
            // in the RAS.
            TheISA::PCState rasTop = RAS[tid].top();
            target = TheISA::buildRetPC(instPC, rasTop);

            // Record the top entry of the RAS, and its index.
            predict_record.usedRAS = true;
            predict_record.RASIndex = RAS[tid].topIdx();
            predict_record.RASTarget = rasTop;

            assert(predict_record.RASIndex < 16);

            RAS[tid].pop();

            DPRINTF(Branch, "[tid:%i]: Instruction %s is a return, "
                    "RAS predicted target: %s, RAS index: %i.\n",
                    tid, instPC, target,
                    predict_record.RASIndex);
        } else {
            ++BTBLookups;

            if (inst->isCall()) {

                RAS[tid].push(instPC);
                predict_record.pushedRAS = true;

                // Record that it was a call so that the top RAS entry can
                // be popped off if the speculation is incorrect.
                predict_record.wasCall = true;

                DPRINTF(Branch, "[tid:%i]: Instruction %s was a call"
                        ", adding %s to the RAS index: %i.\n",
                        tid, instPC, predPC,
                        RAS[tid].topIdx());
            }

            if (inst->isCall() &&
                inst->isUncondCtrl() &&
                inst->isDirectCtrl()) {
                target = inst->branchTarget(instPC);
            } else if (BTB.valid(predPC.instAddr(), asid)) {
                ++BTBHits;

                // If it's not a return, use the BTB to get the target addr.
                target = BTB.lookup(predPC.instAddr(), asid);

                DPRINTF(Branch, "[tid:%i]: [asid:%i] Instruction %s "
                        "predicted target is %s.\n",
                        tid, asid, instPC, target);
            } else {
                DPRINTF(Branch, "[tid:%i]: BTB doesn't have a "
                        "valid entry, predicting false.\n",tid);
                pred_taken = false;
            }
        }
    }

    if (pred_taken) {
        // Set the PC and the instruction's predicted target.
        predPC = target;
    }
    DPRINTF(Branch, "[tid:%i]: [sn:%i]: Setting Predicted PC to %s.\n",
            tid, seqNum, predPC);

    predHist[tid].push_front(predict_record);

    DPRINTF(Branch, "[tid:%i] [sn:%i] pushed onto front of predHist "
            "...predHist.size(): %i\n",
            tid, seqNum, predHist[tid].size());

    return pred_taken;
}

bool
BackBP::lookup(Addr branch_addr, void * &bp_history, ThreadID tid)
{
    bool taken = true;
    if(BTB.valid(branch_addr, tid)) {
    	Addr target = BTB.lookup(branch_addr,tid).instAddr();;
    	if(target<branch_addr) {
    		taken = true;
    	}
    	else {
    		taken = false;
    	}
    }
    else {
    	taken = false;
    	DPRINTF(Branch, "This case should never be reached?");
    }
    return taken;
}

bool
BackBP::lookup(Addr branch_addr, void * &bp_history)
{
    DPRINTF(Branch, "This should never be reached!");
    return false;
}

void
BackBP::update(Addr branch_addr, bool taken, void *bp_history, bool squashed)
{

}

void
BackBP::uncondBranch(void *&bp_history)
{
}