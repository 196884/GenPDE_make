//
//  MOReference.h
//  GenPDE
//
//  Created by Regis Dupont on 8/20/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef MOREFERENCE_H_
#define MOREFERENCE_H_

#include <iostream>

#include "PayoutExpression.h"
#include "MarketObservable.h"

class MOReference : public PayoutExpression
{
public:
    MOReference(MOUid uid)
    :mUid(uid)
    {}
    
    virtual CEVConstPtr evalCE(
        const MOContext& mo_context,
        const AVContext& av_context
    ) const;
    
    virtual CEVConstPtr evalCE(
        const PDEModelPtr& model
    ) const
    {
        return model->marketObservableCE(mUid);
    }
    
protected:
    virtual void print(std::ostream& stream) const;
    
protected:
    MOUid mUid;
};

#endif // MOREFERENCE_H_
