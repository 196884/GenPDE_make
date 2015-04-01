//
//  PEConditional.cpp
//  GenPDE
//
//  Created by Regis Dupont on 9/21/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "PEConditional.h"

#include "CEValuesStored.h"

void PEConditional::print(std::ostream& stream) const
{
    stream << "<Conditional>" << *mCond << *mPE1 << *mPE2 << "</Conditional>";
}

PEConditional::CEVConstPtr PEConditional::evalCE(
    const MOContext& mo_context,
    const AVContext& av_context
) const
{
    CEVConstPtr cond = mCond->evalCE(mo_context, av_context);
    CEVConstPtr arg1 = mPE1->evalCE(mo_context, av_context);
    CEVConstPtr arg2 = mPE2->evalCE(mo_context, av_context);
    boost::shared_ptr<CEValues> result(new CEValuesStored(cond, arg1, arg2));
    result->evalConditional(cond, arg1, arg2);
    return result;
}

PEConditional::CEVConstPtr PEConditional::evalCE(
    const PDEModelPtr& model
) const
{
    CEVConstPtr cond = mCond->evalCE(model);
    CEVConstPtr arg1 = mPE1->evalCE(model);
    CEVConstPtr arg2 = mPE2->evalCE(model);
    boost::shared_ptr<CEValues> result(new CEValuesStored(cond, arg1, arg2));
    result->evalConditional(cond, arg1, arg2);
    return result;
}
