#ifndef AV_REFERENCE_H
#define AV_REFERENCE_H

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
    GenPDE::VariableUID mUid;
};

#endif // AV_REFERENCE_H
