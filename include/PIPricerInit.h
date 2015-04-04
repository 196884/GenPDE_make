#ifndef PI_PRICER_INIT_H
#define PI_PRICER_INIT_H

#include <boost/shared_ptr.hpp>

#include "PricingInstruction.h"
#include "PricerUid.h"
#include "TradeLeg.h"

class PIPricerInit : public PricingInstruction
{
protected:
    typedef boost::shared_ptr<const TradeLeg> TLConstPtr;
    
public:
    virtual ~PIPricerInit() {}
    
    PIPricerInit(
        PricerUid         pricer_uid,
        const TLConstPtr& trade_leg
    );
    
    void apply(const ModelPtr& model, const TradePtr& trade) const;
    
    friend std::ostream& operator<<(std::ostream& stream, const PIPricerInit& pi);
    
protected:
    virtual void print(std::ostream& stream) const;
    
protected:
    PricerUid  mPricerUid;
    TLConstPtr mTL;
};

#endif // PI_PRICER_INIT_H
