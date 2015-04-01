//
//  TLPricerRef.h
//  GenPDE
//
//  Created by Regis Dupont on 8/26/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef TLPRICERREF_H_
#define TLPRICERREF_H_

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

#endif // TLPRICERREF_H_
