#ifndef TL_PAYMENT_H
#define TL_PAYMENT_H

#include <boost/shared_ptr.hpp>

#include "TradeLeg.h"

#include "GpDate.h"
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

#endif // TL_PAYMENT_H
