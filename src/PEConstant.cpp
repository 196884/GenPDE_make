#include "PEConstant.h"
#include "CEValuesStored.h"

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

PEConstant::CEVConstPtr PEConstant::evalFromFixings(
    const GenPDE::Date&       date,
    const TradeFixings&       fixings,
    const AuxiliaryVariables& av_defs,
    AVContext&                av_context // updated by the call
) const
{
    CEVConstPtr result(new CEValuesStored(mValue));
    return result;
}

