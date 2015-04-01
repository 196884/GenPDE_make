//
//  TradeLeg.h
//  GenPDE
//
//  Created by Regis Dupont on 8/26/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef TRADELEG_H_
#define TRADELEG_H_

#include <iostream>
#include <boost/shared_ptr.hpp>

#include "CEValues.h"
#include "PDEPricingModelInterface.h"

class TradeLeg
{
protected:
    typedef boost::shared_ptr<const CEValues> CEVConstPtr;
    typedef boost::shared_ptr<const PDEPricingModelInterface> PDEModelPtr;
    
public:
    virtual ~TradeLeg() {}
    
    virtual CEVConstPtr evalCE(
        const PDEModelPtr& model
    ) const = 0;
    
    friend std::ostream& operator<<(std::ostream& stream, const TradeLeg& leg);
    
protected:
    virtual void print(std::ostream& stream) const = 0;
};

#endif // TRADELEG_H_