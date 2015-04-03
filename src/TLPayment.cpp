//
//  TLPayment.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/26/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "TLPayment.h"
#include "CEValuesStored.h"

TLPayment::TLPayment(
    const GenPDE::Date& payment_date,
    const PEConstPtr&   payout_expression
)
:mPaymentDate(payment_date)
,mPE(payout_expression)
{}

std::ostream& operator<<(std::ostream& stream, const TLPayment& payment)
{
    payment.print(stream);
    return stream;
}
    
void TLPayment::print(std::ostream& stream) const
{
    stream << "<Payment date=\"" << GenPDE::dateToString(mPaymentDate) << "\">" << *mPE << "</Payment>";
}

TLPayment::CEVConstPtr TLPayment::evalCE(
    const PDEModelPtr& model
) const
{
    CEVConstPtr peValue(mPE->evalCE(model));
    if( model->getCurrentDate() == mPaymentDate )
        return peValue; // no discount needed
    CEVConstPtr dfValue(model->discountFactorCE(mPaymentDate));
    boost::shared_ptr<CEValues> result(new CEValuesStored(peValue, dfValue));
    result->evalBinaryOperation(GenPDE::Mul, peValue, dfValue);
    return result;
}
