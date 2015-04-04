#include "PEUnaryOperation.h"

#include "CEValuesStored.h"

void PEUnaryOperation::print(std::ostream& stream) const
{
    stream << "<UnOp op=\"" << mOperator << "\">" << *mPE << "</UnOp>";
}

PEUnaryOperation::CEVConstPtr PEUnaryOperation::evalCE(
    const MOContext& mo_context,
    const AVContext& av_context
) const
{
    CEVConstPtr arg1 = mPE->evalCE(mo_context, av_context);
    boost::shared_ptr<CEValues> result(new CEValuesStored(arg1));
    result->evalUnaryOperation(mOperator, arg1);
    return result;
}

PEUnaryOperation::CEVConstPtr PEUnaryOperation::evalCE(
    const PDEModelPtr& model
) const
{
    CEVConstPtr arg1 = mPE->evalCE(model);
    boost::shared_ptr<CEValues> result(new CEValuesStored(arg1));
    result->evalUnaryOperation(mOperator, arg1);
    return result;
}

PEUnaryOperation::CEVConstPtr PEUnaryOperation::evalFromFixings(
    const GenPDE::Date&       date,
    const TradeFixings&       fixings,
    const AuxiliaryVariables& av_defs,
    AVContext&                av_context
) const
{
    CEVConstPtr arg1 = mPE->evalFromFixings(date, fixings, av_defs, av_context);
    if( !arg1 )
        return arg1;
    boost::shared_ptr<CEValues> result(new CEValuesStored(arg1));
    result->evalUnaryOperation(mOperator, arg1);
    return result;
} 
