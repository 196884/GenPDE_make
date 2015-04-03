//
//  PIPricerMult.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/26/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "PIPricerMult.h"

PIPricerMult::PIPricerMult(
    PricerUid         pricer_uid,
    const PEConstPtr& payout_expression,
    const TLConstPtr& trade_leg
)
:mPricerUid(pricer_uid)
,mPE(payout_expression)
,mTL(trade_leg)
{}

void PIPricerMult::apply(const ModelPtr& model, const TradePtr& trade) const
{
    CEVConstPtr     peVal(mPE->evalCE(model));
    CEVConstPtr     legVal(mTL->evalCE(model));
    VarDependencies avDeps(legVal->getVarMemoryLayout(), peVal->getVarMemoryLayout());
    CEVPtr          pricer(model->addPricer(mPricerUid, avDeps));
    pricer->evalBinaryOperation(GenPDE::Mul, legVal, peVal);
}

void PIPricerMult::print(std::ostream& stream) const
{
    stream << "<PricerMult uid=\"" << mPricerUid << "\">" << *mPE << *mTL << "</PricerMult>";
}

std::ostream& operator<<(std::ostream& stream, const PIPricerMult& pm)
{
    pm.print(stream);
    return stream;
}
