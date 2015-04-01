//
//  AVReference.h
//  GenPDE
//
//  Created by Regis Dupont on 8/20/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef AVREFERENCE_H_
#define AVREFERENCE_H_

#include <iostream>

#include "PayoutExpression.h"
#include "AuxiliaryVariable.h"

class AVReference : public PayoutExpression
{
public:
    AVReference(GenPDE::VariableUID av_uid)
    :mUid(av_uid)
    {}
    
    virtual CEVConstPtr evalCE(
        const MOContext& mo_context,
        const AVContext& av_context
    ) const;
    
    virtual CEVConstPtr evalCE(
        const PDEModelPtr& model
    ) const
    {
        return model->auxiliaryVariableCE(mUid);
    }
    
protected:
    virtual void print(std::ostream& stream) const;
    
protected:
    GenPDE::VariableUID mUid;
};

#endif // AVREFERENCE_H_