//
//  PIPricerRemove.h
//  GenPDE
//
//  Created by Regis Dupont on 8/26/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef PIPRICERREMOVE_H_
#define PIPRICERREMOVE_H_

#include <boost/shared_ptr.hpp>

#include "PricingInstruction.h"
#include "PricerUid.h"

class PIPricerRemove : public PricingInstruction
{
public:
    virtual ~PIPricerRemove() {}
    
    PIPricerRemove(PricerUid         pricer_uid);
    
    void apply(const ModelPtr& model) const;
    
    friend std::ostream& operator<<(std::ostream& stream, const PIPricerRemove& pi);
    
protected:
    virtual void print(std::ostream& stream) const;
    
protected:
    PricerUid mPricerUid;
};

#endif // PIPRICERREMOVE_H_

