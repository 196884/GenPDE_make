//
//  PIPricerInit.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/26/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "PIPricerInit.h"

PIPricerInit::PIPricerInit(
    PricerUid         pricer_uid,
    const TLConstPtr& trade_leg
)
:mPricerUid(pricer_uid)
,mTL(trade_leg)
{}

void PIPricerInit::apply(const ModelPtr& model) const
{
    CEVConstPtr tlValue(mTL->evalCE(model));
    CEVPtr      pricer(model->addPricer(mPricerUid, tlValue->getVarMemoryLayout()));
    pricer->evalUnaryOperation(GenPDE::Id, tlValue);
}

void PIPricerInit::print(std::ostream& stream) const
{
    stream << "<PricerInit uid=\"" << mPricerUid << "\">" << *mTL << "</PricerInit>";
}

std::ostream& operator<<(std::ostream& stream, const PIPricerInit& pi)
{
    pi.print(stream);
    return stream;
}
