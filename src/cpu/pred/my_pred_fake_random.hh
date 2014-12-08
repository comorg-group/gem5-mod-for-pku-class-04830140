//
//  my_pred_fake_random.hh
//  gem5-mod-for-pku-class-04830140
//
//  Created by Leedy Liu on 2014-11-17.
//  Copyright (c) 2014 Leedy Liu. All rights reserved.
//

#ifndef __CPU_PRED_MY_PRED_FAKE_RANDOM_HH__
#define __CPU_PRED_MY_PRED_FAKE_RANDOM_HH__

#include <vector>

#include "base/types.hh"
#include "cpu/pred/bpred_unit.hh"
#include "cpu/pred/sat_counter.hh"

/**
 * Implements a simple predictor that always choose to jump.
 */
class RandomBP : public BPredUnit
{
  public:
    /**
     * Default branch predictor constructor.
     */
    RandomBP(const Params *params);

    virtual void uncondBranch(void * &bp_history);

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

  private:
};

#endif // __CPU_PRED_MY_PRED_FAKE_RANDOM_HH__
