//
//  PEUnaryOperation.h
//  GenPDE
//
//  Created by Regis Dupont on 8/21/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef PEUNARYOPERATION_H_
#define PEUNARYOPERATION_H_

#include <iostream>

#include <boost/shared_ptr.hpp>

#include "PayoutExpression.h"
#include "PEOperators.h"

class PEUnaryOperation : public PayoutExpression
{
    typedef boost::shared_ptr<PayoutExpression> PEPtr;
    
public:
    PEUnaryOperation(
        GenPDE::UnaryOperator op,
        PEPtr                 pe
    )
    :mOperator(op)
    ,mPE(pe)
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
    GenPDE::UnaryOperator mOperator;
    PEPtr                 mPE;
};

#endif // PEUNARYOPERATION_H_
