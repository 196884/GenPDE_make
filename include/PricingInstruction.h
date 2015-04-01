//
//  PricingInstruction.h
//  GenPDE
//
//  Created by Regis Dupont on 8/26/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef PRICINGINSTRUCTION_H_
#define PRICINGINSTRUCTION_H_

#include <iostream>
#include <boost//shared_ptr.hpp>

#include "PDEPricingModelInterface.h"
#include "CEValues.h"

class PricingInstruction
{
protected:
    typedef boost::shared_ptr<PDEPricingModelInterface> ModelPtr;
    typedef boost::shared_ptr<CEValues>                 CEVPtr;
    typedef boost::shared_ptr<const CEValues>           CEVConstPtr;
    
public:
    virtual ~PricingInstruction() {}
    
    virtual void apply(const ModelPtr& model) const = 0;
    
    friend std::ostream& operator<<(std::ostream& stream, const PricingInstruction&);
    
protected:
    virtual void print(std::ostream& stream) const = 0;
};

#endif // PRICINGINSTRUCTION_H_