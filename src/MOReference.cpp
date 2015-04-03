//
//  MOReference.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/20/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "MOReference.h"
#include "MOContext.h"
#include "PDEPricingModelInterface.h"

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

MOReference::CEVConstPtr MOReference::evalCE(
    const PDEModelPtr& model
) const
{
    return model->marketObservableCE(mUid);
}

MOReference::CEVConstPtr MOReference::evalFromFixings(
    const GenPDE::Date&       date,
    const TradeFixings&       fixings,
    const AuxiliaryVariables& av_defs,
    AVContext&                av_context // updated by the call
) const
{
    boost::optional<double> fixing = fixings.getMOFixing(mUid, date);
    if( !fixing )
        return CEVConstPtr();
    return boost::shared_ptr<CEValues>(new CEValuesStored(*fixing));
}
