#ifndef PI_PRICER_REMOVE_H
#define PI_PRICER_REMOVE_H

#include <boost/shared_ptr.hpp>

#include "PricingInstruction.h"
#include "PricerUid.h"

class PIPricerRemove : public PricingInstruction
{
public:
    virtual ~PIPricerRemove() {}
    
    PIPricerRemove(PricerUid         pricer_uid);
    
    void apply(const ModelPtr& model, const TradePtr& trade) const;
    
    friend std::ostream& operator<<(std::ostream& stream, const PIPricerRemove& pi);
    
protected:
    virtual void print(std::ostream& stream) const;
    
protected:
    PricerUid mPricerUid;
};

#endif // PI_PRICER_REMOVE_H
