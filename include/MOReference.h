#ifndef MO_REFERENCE_H
#define MO_REFERENCE_H

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
    ) const;

    virtual CEVConstPtr evalFromFixings(
        const GenPDE::Date&       date,
        const MOFixingsIfc&       mo_fixings,
        const AuxiliaryVariables& av_defs,
        AVContext&                av_context // updated by the call
    ) const;
    
protected:
    virtual void print(std::ostream& stream) const;
    
protected:
    MOUid mUid;
};

#endif // MO_REFERENCE_H
