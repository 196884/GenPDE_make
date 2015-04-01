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
    boost::shared_ptr<const AuxiliaryVariable> avDef = av_context.getAuxiliaryVariable(mUid);
    const std::vector<double>& avValues = avDef->getDiscretizationValues();
    VarDependencies varDeps(GenPDE::VT_AuxiliaryVariable, mUid, avValues.size());
    boost::shared_ptr<CEValues> result(new CEValuesStored(varDeps));
    double* ptr = result->getDataPtr();
    std::copy(avValues.begin(), avValues.end(), ptr);
    return result;
}
