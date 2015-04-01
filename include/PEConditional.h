//
//  PEConditional.h
//  GenPDE
//
//  Created by Regis Dupont on 9/21/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef PECONDITIONAL_H_
#define PECONDITIONAL_H_

#include <iostream>

#include <boost/shared_ptr.hpp>

#include "PayoutExpression.h"

class PEConditional : public PayoutExpression
{
    typedef boost::shared_ptr<PayoutExpression> PEPtr;
    
public:
    PEConditional(
        PEPtr peCond,
        PEPtr pe1,
        PEPtr pe2
    )
    :mCond(peCond)
    ,mPE1(pe1)
    ,mPE2(pe2)
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
    PEPtr mCond;
    PEPtr mPE1;
    PEPtr mPE2;
};



#endif // PECONDITIONAL_H_