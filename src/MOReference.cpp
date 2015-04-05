#include "MOReference.h"
#include "MOContext.h"
#include "PDEPricingModelInterface.h"
#include "CEValuesStored.h"
#include "AuxiliaryVariable.h"

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
    const MOFixingsIfc&       mo_fixings,
    const AuxiliaryVariables& av_defs,
    AVContext&                av_context // updated by the call
) const
{
    return mo_fixings.getFixing( mUid, date );
}
