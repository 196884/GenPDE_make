//
//  PIPricerMult.h
//  GenPDE
//
//  Created by Regis Dupont on 8/26/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef PIPRICERMULT_H_
#define PIPRICERMULT_H_

#include <boost/shared_ptr.hpp>

#include "PricingInstruction.h"
#include "PricerUid.h"
#include "PayoutExpression.h"
#include "TradeLeg.h"

class PIPricerMult : public PricingInstruction
{
protected:
    typedef boost::shared_ptr<const PayoutExpression> PEConstPtr;
    typedef boost::shared_ptr<const TradeLeg>         TLConstPtr;
    
public:
    virtual ~PIPricerMult() {}
    
    PIPricerMult(
        PricerUid         pricer_uid,
        const PEConstPtr& payout_expression,
        const TLConstPtr& trade_leg
    );
    
    void apply(const ModelPtr& model) const;
    
    friend std::ostream& operator<<(std::ostream& stream, const PIPricerMult& pi);
    
protected:
    virtual void print(std::ostream& stream) const;
    
protected:
    PricerUid  mPricerUid;
    PEConstPtr mPE;
    TLConstPtr mTL;
};

#endif // PIPRICERMULT_H_

