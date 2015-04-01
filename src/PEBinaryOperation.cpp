//
//  PEBinaryOperation.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/20/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "PEBinaryOperation.h"

#include "CEValuesStored.h"

void PEBinaryOperation::print(std::ostream& stream) const
{
    stream << "<BinOp op=\"" << mOperator << "\">" << *mPE1 << *mPE2 << "</BinOp>";
}

PEBinaryOperation::CEVConstPtr PEBinaryOperation::evalCE(
    const MOContext& mo_context,
    const AVContext& av_context
) const
{
    CEVConstPtr arg1 = mPE1->evalCE(mo_context, av_context);
    CEVConstPtr arg2 = mPE2->evalCE(mo_context, av_context);
    boost::shared_ptr<CEValues> result(new CEValuesStored(arg1, arg2));
    result->evalBinaryOperation(mOperator, arg1, arg2);
    return result;
}

PEBinaryOperation::CEVConstPtr PEBinaryOperation::evalCE(
    const PDEModelPtr& model
) const
{
    CEVConstPtr arg1 = mPE1->evalCE(model);
    CEVConstPtr arg2 = mPE2->evalCE(model);
    boost::shared_ptr<CEValues> result(new CEValuesStored(arg1, arg2));
    result->evalBinaryOperation(mOperator, arg1, arg2);
    return result;
}