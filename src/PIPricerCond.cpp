#include "PIPricerCond.h"
#include "Framework.h"
#include "PayoutExpression.h"
#include "TradeLeg.h"

PIPricerCond::PIPricerCond(
    PricerUid         pricer_uid,
    const PEConstPtr& cond,
    const TLConstPtr& trade_leg_1,
    const TLConstPtr& trade_leg_2
)
:mPricerUid(pricer_uid)
,mCond(cond)
,mTL1(trade_leg_1)
,mTL2(trade_leg_2)
{}

void PIPricerCond::apply(const ModelPtr& model, const TradePtr& trade) const
{
    Exception::notImplemented("PIPricerCond::apply");
}

void PIPricerCond::print(std::ostream& stream) const
{
    stream << "<PricerCond uid=\"" << mPricerUid << "\">" << *mCond << *mTL1 << *mTL2 << "</PricerCond>";
}

std::ostream& operator<<(std::ostream& stream, const PIPricerCond& pa)
{
    pa.print(stream);
    return stream;
}
