#ifndef PE_UNARY_OPERATION_H
#define PE_UNARY_OPERATION_H

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

    virtual CEVConstPtr evalFromFixings(
        const GenPDE::Date&       date,
        const TradeFixings&       fixings,
        const AuxiliaryVariables& av_defs,
        AVContext&                av_context // updated by the call
    ) const;
    
protected:
    virtual void print(std::ostream& stream) const;
    
protected:
    GenPDE::UnaryOperator mOperator;
    PEPtr                 mPE;
};

#endif //  PE_UNARY_OPERATION_H
