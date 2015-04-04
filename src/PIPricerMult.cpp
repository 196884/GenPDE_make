#include "PIPricerMult.h"
#include "PayoutExpression.h"
#include "TradeLeg.h"

PIPricerMult::PIPricerMult(
    PricerUid         pricer_uid,
    const PEConstPtr& payout_expression,
    const TLConstPtr& trade_leg
)
:mPricerUid(pricer_uid)
,mPE(payout_expression)
,mTL(trade_leg)
{}

void PIPricerMult::apply(const ModelPtr& model, const TradePtr& trade) const
{
    CEVConstPtr     peVal(mPE->evalCE(model));
    CEVConstPtr     legVal(mTL->evalCE(model));
    VarDependencies avDeps(legVal->getVarMemoryLayout(), peVal->getVarMemoryLayout());
    CEVPtr          pricer(model->addPricer(mPricerUid, avDeps));
    pricer->evalBinaryOperation(GenPDE::Mul, legVal, peVal);
}

void PIPricerMult::print(std::ostream& stream) const
{
    stream << "<PricerMult uid=\"" << mPricerUid << "\">" << *mPE << *mTL << "</PricerMult>";
}

std::ostream& operator<<(std::ostream& stream, const PIPricerMult& pm)
{
    pm.print(stream);
    return stream;
}
