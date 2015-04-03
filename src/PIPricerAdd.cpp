//
//  PIPricerAdd.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/26/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "PIPricerAdd.h"

PIPricerAdd::PIPricerAdd(
    PricerUid         pricer_uid,
    const TLConstPtr& trade_leg_1,
    const TLConstPtr& trade_leg_2
)
:mPricerUid(pricer_uid)
,mTL1(trade_leg_1)
,mTL2(trade_leg_2)
{}

void PIPricerAdd::apply(const ModelPtr& model, const TradePtr& trade) const
{
    CEVConstPtr     leg1Val(mTL1->evalCE(model));
    CEVConstPtr     leg2Val(mTL2->evalCE(model));
    VarDependencies varDeps(leg1Val->getVarMemoryLayout(), leg2Val->getVarMemoryLayout());
    CEVPtr          pricer(model->addPricer(mPricerUid, varDeps));
    pricer->evalBinaryOperation(GenPDE::Add, leg1Val, leg2Val);
}

void PIPricerAdd::print(std::ostream& stream) const
{
    stream << "<PricerAdd uid=\"" << mPricerUid << "\">" << *mTL1 << *mTL2 << "</PricerAdd>";
}

std::ostream& operator<<(std::ostream& stream, const PIPricerAdd& pa)
{
    pa.print(stream);
    return stream;
}
