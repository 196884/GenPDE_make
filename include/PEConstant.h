#ifndef PE_CONSTANT_H
#define PE_CONSTANT_H

#include <iostream>

#include "PayoutExpression.h"

class PEConstant : public PayoutExpression
{
public:
    PEConstant(double value)
    :mValue(value)
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
    double mValue;
};

#endif // PE_CONSTANT_H
