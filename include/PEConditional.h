#ifndef PE_CONDITIONAL_H
#define PE_CONDITIONAL_H

#include <iostream>
#include <boost/shared_ptr.hpp>

#include "PayoutExpression.h"

class PEConditional : public PayoutExpression
{
    typedef boost::shared_ptr<PayoutExpression> PEPtr;
    
public:
    PEConditional(
        PEPtr peCond,
        PEPtr pe1,
        PEPtr pe2
    )
    :mCond(peCond)
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

    virtual CEVConstPtr evalFromFixings(
        const GenPDE::Date&       date,
        const TradeFixings&       fixings,
        const AuxiliaryVariables& av_defs,
        AVContext&                av_context // updated by the call
    ) const;
     
protected:
    virtual void print(std::ostream& stream) const;
    
protected:
    PEPtr mCond;
    PEPtr mPE1;
    PEPtr mPE2;
};

#endif // PE_CONDITIONAL_H
