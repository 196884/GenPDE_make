//
//  TLPricerRef.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/26/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "TLPricerRef.h"

TLPricerRef::TLPricerRef(PricerUid uid)
:mUid(uid)
{}

void TLPricerRef::print(std::ostream &stream) const
{
    stream << "<PricerRef uid=\"" << mUid << "\"/>";
}

std::ostream& operator<<(std::ostream& stream, const TLPricerRef& pricer)
{
    pricer.print(stream);
    return stream;
}