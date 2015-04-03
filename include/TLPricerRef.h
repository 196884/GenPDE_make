#ifndef TL_PRICER_REF_H
#define TL_PRICER_REF_H

#include "TradeLeg.h"
#include "PricerUid.h"

class TLPricerRef : public TradeLeg
{
public:
    virtual ~TLPricerRef() {}
    
    TLPricerRef(PricerUid uid);
    
    virtual CEVConstPtr evalCE(
        const PDEModelPtr& model
    ) const
    {
        return model->getPricer(mUid);
    }
    
    friend std::ostream& operator<<(std::ostream& stream, const TLPricerRef& pricer);
    
protected:
    virtual void print(std::ostream& stream) const;
    
protected:
    PricerUid          mUid;
};

#endif // TL_PRICER_REF_H
