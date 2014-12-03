//
//  my_pred_always_back.hh
//  gem5-mod-for-pku-class-04830140
//
//  Created by Jackie Yang on 2014-12-03.
//  Copyright (c) 2014 Jackie Yang. All rights reserved.
//

#ifndef __CPU_PRED_MY_PRED_ALWAYS_BACK_HH__
#define __CPU_PRED_MY_PRED_ALWAYS_BACK_HH__

#include <vector>

#include "base/types.hh"
#include "cpu/pred/bpred_unit.hh"
#include "cpu/pred/sat_counter.hh"

/**
 * Implements a simple predictor that always choose to jump back.
 */
class BackBP : public BPredUnit
{
  public:
    /**
     * Default branch predictor constructor.
     */
    BackBP(const Params *params);

    virtual void uncondBranch(void * &bp_history);

    /**
     * Predicts whether or not the instruction is a taken branch, and the
     * target of the branch if it is taken. (This override the same function in the base class)
     * @param inst The branch instruction.
     * @param PC The predicted PC is passed back through this parameter.
     * @param tid The thread id.
     * @return Returns if the branch is taken or not.
     */
    bool predictInOrder(StaticInstPtr &inst, const InstSeqNum &seqNum,
                          int asid, TheISA::PCState &instPC,
                          TheISA::PCState &predPC, ThreadID tid);

    /**
     * Looks up the given address in the branch predictor and returns
     * a true/false value as to whether it is taken.
     * @param branch_addr The address of the branch to look up.
     * @param bp_history Pointer to any bp history state.
     * @param tid The thread id.
     * @return Whether or not the branch is taken.
     */
    bool lookup(Addr branch_addr, void * &bp_history, ThreadID tid);

    /**
     * Looks up the given address in the branch predictor and returns
     * a true/false value as to whether it is taken.
     * @param branch_addr The address of the branch to look up.
     * @param bp_history Pointer to any bp history state.
     * @return Whether or not the branch is taken.
     */
    bool lookup(Addr branch_addr, void * &bp_history);

    /**
     * Updates the branch predictor to Not Taken if a BTB entry is
     * invalid or not found.
     * @param branch_addr The address of the branch to look up.
     * @param bp_history Pointer to any bp history state.
     * @return Whether or not the branch is taken.
     */
    void btbUpdate(Addr branch_addr, void * &bp_history);

    /**
     * Updates the branch predictor with the actual result of a branch.
     * @param branch_addr The address of the branch to update.
     * @param taken Whether or not the branch was taken.
     */
    void update(Addr branch_addr, bool taken, void *bp_history, bool squashed);

    void squash(void *bp_history)
    { assert(bp_history == NULL); }

    void reset();

  private:
};

#endif // __CPU_PRED_MY_PRED_ALWAYS_BACK_HH__
