//
//  MOReference.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/20/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "MOReference.h"

void MOReference::print(std::ostream& stream) const
{
    stream << "<MORef uid=\"" << mUid << "\"/>";
}

MOReference::CEVConstPtr MOReference::evalCE(
    const MOContext& mo_context,
    const AVContext& av_context
) const
{
    return mo_context.evalCE(mUid);
}

