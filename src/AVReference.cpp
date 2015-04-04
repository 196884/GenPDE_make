#include "AVReference.h"
#include "AVContext.h"
#include "PDEPricingModelInterface.h"
#include "CEValues.h"

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

AVReference::CEVConstPtr AVReference::evalCE(
    const PDEModelPtr& model
) const
{
    return model->auxiliaryVariableCE(mUid);
}

AVReference::CEVConstPtr AVReference::evalFromFixings(
    const GenPDE::Date&       date,
    const TradeFixings&       fixings,
    const AuxiliaryVariables& av_defs,
    AVContext&                av_context // updated by the call
) const
{
    CEVConstPtr result = av_context.evalCE(mUid);
    if( result )
        return result;

    result = av_defs.getAuxiliaryVariable(mUid)->getDefinition()->evalFromFixings(date, fixings, av_defs, av_context);
    if( result )
    {
        std::vector<double> value(1, *result->getDataPtr());
        av_context.setAVDiscretizationValues(mUid, value);
    }
    return result;
}
