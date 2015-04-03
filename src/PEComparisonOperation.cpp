//
//  PEComparisonOperation.cpp
//  GenPDE
//
//  Created by Regis Dupont on 9/21/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "PEComparisonOperation.h"

#include "CEValuesStored.h"

void PEComparisonOperation::print(std::ostream& stream) const
{
    stream << "<CmpOp op=\"" << mOperator << "\">" << *mPE1 << *mPE2 << "</CmpOp>";
}

PEComparisonOperation::CEVConstPtr PEComparisonOperation::evalCE(
    const MOContext& mo_context,
    const AVContext& av_context
) const
{
    CEVConstPtr arg1 = mPE1->evalCE(mo_context, av_context);
    CEVConstPtr arg2 = mPE2->evalCE(mo_context, av_context);
    boost::shared_ptr<CEValues> result(new CEValuesStored(arg1, arg2));
    result->evalComparisonOperation(mOperator, arg1, arg2);
    return result;
}

PEComparisonOperation::CEVConstPtr PEComparisonOperation::evalCE(
    const PDEModelPtr& model
) const
{
    CEVConstPtr arg1 = mPE1->evalCE(model);
    CEVConstPtr arg2 = mPE2->evalCE(model);
    boost::shared_ptr<CEValues> result(new CEValuesStored(arg1, arg2));
    result->evalComparisonOperation(mOperator, arg1, arg2);
    return result;
}

PEComparisonOperation::CEVConstPtr PEComparisonOperation::evalFromFixings(
    const GenPDE::Date&       date,
    const TradeFixings&       fixings,
    const AuxiliaryVariables& av_defs,
    AVContext&                av_context // updated by the call
) const
{
    CEVConstPtr arg1 = mPE1->evalFromFixings(date, fixings, av_defs, av_context);
    if( !arg1 )
        return arg1;
    CEVConstPtr arg2 = mPE2->evalFromFixings(date, fixings, av_defs, av_context);
    if( !arg2 )
        return arg2;
    boost::shared_ptr<CEValues> result(new CEValuesStored(arg1, arg2));
    result->evalComparisonOperation(mOperator, arg1, arg2);
    return result;
}

