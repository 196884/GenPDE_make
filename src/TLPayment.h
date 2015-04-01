//
//  TLPayment.h
//  GenPDE
//
//  Created by Regis Dupont on 8/26/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef TLPAYMENT_H_
#define TLPAYMENT_H_

#include <boost/shared_ptr.hpp>

#include "TradeLeg.h"

#include "Date.h"
#include "PayoutExpression.h"

class TLPayment : public TradeLeg
{
protected:
    typedef boost::shared_ptr<const PayoutExpression> PEConstPtr;
    
public:
    virtual ~TLPayment() {}
    
    TLPayment(
        const GenPDE::Date& payment_date,
        const PEConstPtr&   payout_expression
    );
    
    virtual CEVConstPtr evalCE(
        const PDEModelPtr& model
    ) const;
    
    friend std::ostream& operator<<(std::ostream& stream, const TLPayment& payment);
    
protected:
    virtual void print(std::ostream& stream) const;
    
protected:
    GenPDE::Date mPaymentDate;
    PEConstPtr   mPE;
};

#endif // TLPAYMENT_H_