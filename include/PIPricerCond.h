#ifndef PI_PRICER_COND_H
#define PI_PRICER_COND_H

#include <boost/shared_ptr.hpp>

#include "PricingInstruction.h"
#include "PricerUid.h"

class TradeLeg;
class PayoutExpression;

class PIPricerCond : public PricingInstruction
{
protected:
    typedef boost::shared_ptr<const TradeLeg>         TLConstPtr;
    typedef boost::shared_ptr<const PayoutExpression> PEConstPtr;
    
public:
    virtual ~PIPricerCond() {}
    
    PIPricerCond(
        PricerUid         pricer_uid,
        const PEConstPtr& cond,
        const TLConstPtr& trade_leg_1,
        const TLConstPtr& trade_leg_2
    );
    
    void apply(const ModelPtr& model, const TradePtr& trade) const;
    
    friend std::ostream& operator<<(std::ostream& stream, const PIPricerCond& pi);
    
protected:
    virtual void print(std::ostream& stream) const;
    
protected:
    PricerUid  mPricerUid;
    PEConstPtr mCond;
    TLConstPtr mTL1;
    TLConstPtr mTL2;
};

#endif // PI_PRICER_COND_H
