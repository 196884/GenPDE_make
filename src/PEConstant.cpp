//
//  PEConstant.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/20/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "PEConstant.h"

void PEConstant::print(std::ostream& stream) const
{
    stream << "<Const value=\"" << mValue << "\"/>";
}

PEConstant::CEVConstPtr PEConstant::evalCE(
    const MOContext& mo_context,
    const AVContext& av_context
) const
{
    CEVConstPtr result(new CEValuesStored(mValue));
    return result;
}

PEConstant::CEVConstPtr PEConstant::evalCE(
    const PDEModelPtr& model
) const
{
    CEVConstPtr result(new CEValuesStored(mValue));
    return result;
}
