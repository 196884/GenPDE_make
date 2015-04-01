//
//  PEComparisonOperation.h
//  GenPDE
//
//  Created by Regis Dupont on 9/21/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef PECOMPARISONOPERATION_H_
#define PECOMPARISONOPERATION_H_

#include <iostream>

#include <boost/shared_ptr.hpp>

#include "PayoutExpression.h"
#include "PEOperators.h"

class PEComparisonOperation : public PayoutExpression
{
    typedef boost::shared_ptr<PayoutExpression> PEPtr;
    
public:
    PEComparisonOperation(
        GenPDE::ComparisonOperator op,
        PEPtr                      pe1,
        PEPtr                      pe2
    )
    :mOperator(op)
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
    GenPDE::ComparisonOperator mOperator;
    PEPtr                      mPE1;
    PEPtr                      mPE2;
};

#endif // PECOMPARISONOPERATION_H_