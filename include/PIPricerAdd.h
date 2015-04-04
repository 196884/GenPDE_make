#ifndef PI_PRICER_ADD_H
#define PI_PRICER_ADD_H

#include <boost/shared_ptr.hpp>

#include "PricingInstruction.h"
#include "PricerUid.h"

class TradeLeg;

class PIPricerAdd : public PricingInstruction
{
protected:
    typedef boost::shared_ptr<const TradeLeg> TLConstPtr;
    
public:
    virtual ~PIPricerAdd() {}
    
    PIPricerAdd(
        PricerUid         pricer_uid,
        const TLConstPtr& trade_leg_1,
        const TLConstPtr& trade_leg_2
    );
    
    void apply(const ModelPtr& model, const TradePtr& trade) const;
    
    friend std::ostream& operator<<(std::ostream& stream, const PIPricerAdd& pi);
    
protected:
    virtual void print(std::ostream& stream) const;
    
protected:
    PricerUid  mPricerUid;
    TLConstPtr mTL1;
    TLConstPtr mTL2;
};

#endif // PI_PRICER_ADD_H
