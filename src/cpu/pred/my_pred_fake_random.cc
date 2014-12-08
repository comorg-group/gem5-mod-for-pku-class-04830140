//
//  my_pred_fake_random.cc
//  gem5-mod-for-pku-class-04830140
//
//  Created by Leedy Liu on 2014-11-17.
//  Copyright (c) 2014 Leedy Liu. All rights reserved.
//

#include "base/intmath.hh"
#include "base/misc.hh"
#include "base/trace.hh"
#include "cpu/pred/my_pred_fake_random.hh"
#include "debug/Fetch.hh"
#include "debug/Branch.hh"

#include "arch/isa_traits.hh"
#include "arch/types.hh"
#include "arch/utility.hh"
#include "base/trace.hh"
#include "config/the_isa.hh"
#include "cpu/pred/bpred_unit.hh"
#include "debug/Branch.hh"

RandomBP::RandomBP(const Params *params) :
	BPredUnit(params)
{
}

void
RandomBP::btbUpdate(Addr branch_addr, void * &bp_history)
{
// Place holder for a function that is called to update predictor history when
// a BTB entry is invalid or not found.
}


bool
RandomBP::lookup(Addr branch_addr, void * &bp_history)
{
    bool pred = (branch_addr >> 4) & 1;
    return pred;
}

void
RandomBP::update(Addr branch_addr, bool taken, void *bp_history, bool squashed)
{
}

void
RandomBP::uncondBranch(void *&bp_history)
{
}
