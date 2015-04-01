//
//  PEConstant.h
//  GenPDE
//
//  Created by Regis Dupont on 8/20/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef PECONSTANT_H_
#define PECONSTANT_H_

#include <iostream>

#include "PayoutExpression.h"

class PEConstant : public PayoutExpression
{
public:
    PEConstant(double value)
    :mValue(value)
    {}
    
    virtual CEVConstPtr evalCE(
        const MOContext& mo_context,
        const AVContext& av_context
    ) const;
    
    virtual CEVConstPtr evalCE(
        const PDEModelPtr& model
    ) const;
    
protected:
    virtual void print(std::ostream& stream) const;
    
protected:
    double mValue;
};

#endif // PECONSTANT_H_