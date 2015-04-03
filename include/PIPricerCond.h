//
//  PIPricerCond.h
//  GenPDE
//
//  Created by Regis Dupont on 8/26/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef PIPRICERCOND_H_
#define PIPRICERCOND_H_

#include <boost/shared_ptr.hpp>

#include "PricingInstruction.h"
#include "PricerUid.h"
#include "TradeLeg.h"
#include "PayoutExpression.h"

class PIPricerCond : public PricingInstruction
{
protected:
    typedef boost::shared_ptr<const TradeLeg>         TLConstPtr;
    typedef boost::shared_ptr<const PayoutExpression> PEConstPtr;
    
public:
    virtual ~PIPricerCond() {}
    
    PIPricerCond(
        PricerUid         pricer_uid,
        const PEConstPtr& cond,
        const TLConstPtr& trade_leg_1,
        const TLConstPtr& trade_leg_2
    );
    
    void apply(const ModelPtr& model, const TradePtr& trade) const;
    
    friend std::ostream& operator<<(std::ostream& stream, const PIPricerCond& pi);
    
protected:
    virtual void print(std::ostream& stream) const;
    
protected:
    PricerUid  mPricerUid;
    PEConstPtr mCond;
    TLConstPtr mTL1;
    TLConstPtr mTL2;
};

#endif // PIPRICERCOND_H_

