//
//  my_pred_always_true.cc
//  gem5-mod-for-pku-class-04830140
//
//  Created by Jackie Yang on 2014-11-17.
//  Copyright (c) 2014 Jackie Yang. All rights reserved.
//

#include "base/intmath.hh"
#include "base/misc.hh"
#include "base/trace.hh"
#include "cpu/pred/my_pred_always_true.hh"
#include "debug/Fetch.hh"
#include "debug/Branch.hh"

#include "arch/isa_traits.hh"
#include "arch/types.hh"
#include "arch/utility.hh"
#include "base/trace.hh"
#include "config/the_isa.hh"
#include "cpu/pred/bpred_unit.hh"
#include "debug/Branch.hh"

AlwaysBP::AlwaysBP(const Params *params) :
	BPredUnit(params)
{

}

void
AlwaysBP::reset()
{
}

void
AlwaysBP::btbUpdate(Addr branch_addr, void * &bp_history)
{
// Place holder for a function that is called to update predictor history when
// a BTB entry is invalid or not found.
}


bool
AlwaysBP::lookup(Addr branch_addr, void * &bp_history)
{
    bool taken = true;
    DPRINTF(Branch, "AlwaysBP always choose TAKEN!\n");
    return taken;
}

void
AlwaysBP::update(Addr branch_addr, bool taken, void *bp_history, bool squashed)
{

}

void
AlwaysBP::uncondBranch(void *&bp_history)
{
}