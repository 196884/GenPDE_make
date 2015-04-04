#ifndef PI_PRICER_MULT_H
#define PI_PRICER_MULT_H

#include <boost/shared_ptr.hpp>

#include "PricingInstruction.h"
#include "PricerUid.h"

class TradeLeg;
class PayoutExpression;

class PIPricerMult : public PricingInstruction
{
protected:
    typedef boost::shared_ptr<const PayoutExpression> PEConstPtr;
    typedef boost::shared_ptr<const TradeLeg>         TLConstPtr;
    
public:
    virtual ~PIPricerMult() {}
    
    PIPricerMult(
        PricerUid         pricer_uid,
        const PEConstPtr& payout_expression,
        const TLConstPtr& trade_leg
    );
    
    void apply(const ModelPtr& model, const TradePtr& trade) const;
    
    friend std::ostream& operator<<(std::ostream& stream, const PIPricerMult& pi);
    
protected:
    virtual void print(std::ostream& stream) const;
    
protected:
    PricerUid  mPricerUid;
    PEConstPtr mPE;
    TLConstPtr mTL;
};

#endif // PI_PRICER_MULT_H
