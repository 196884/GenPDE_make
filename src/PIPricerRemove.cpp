//
//  PIPricerRemove.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/26/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "PIPricerRemove.h"

PIPricerRemove::PIPricerRemove(
    PricerUid         pricer_uid
)
:mPricerUid(pricer_uid)
{}

void PIPricerRemove::apply(const ModelPtr& model) const
{
    model->removePricer(mPricerUid);
}

void PIPricerRemove::print(std::ostream& stream) const
{
    stream << "<PricerRm uid=\"" << mPricerUid << "\"/>";
}

std::ostream& operator<<(std::ostream& stream, const PIPricerRemove& pi)
{
    pi.print(stream);
    return stream;
}
