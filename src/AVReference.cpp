//
//  AVReference.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/20/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "AVReference.h"

void AVReference::print(std::ostream& stream) const
{
    stream << "<AVRef uid=\"" << mUid << "\"/>";
}

AVReference::CEVConstPtr AVReference::evalCE(
    const MOContext& mo_context,
    const AVContext& av_context
) const
{
    return av_context.evalCE(mUid);
}
