//
//  MOContext1DFixed.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/25/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "MOContext1DFixed.h"

MOContext1DFixed::MOContext1DFixed(MOUid uid, const std::vector<double>& values)
:mUid(uid)
{
    VarDependencies svDep(GenPDE::VT_StateVariable, 0, values.size());
    mValues = boost::shared_ptr<CEValues>(new CEValuesStored(svDep));
    std::copy(values.begin(), values.end(), mValues->getDataPtr());
}