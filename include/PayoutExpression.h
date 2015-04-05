#ifndef PAYOUT_EXPRESSION_H
#define PAYOUT_EXPRESSION_H

#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

#include "GpDate.h"

class CEValues;
class PDEPricingModelInterface;
class MOContext;
class AVContext;
class MOFixingsIfc;
class AuxiliaryVariables;

class PayoutExpression
{
protected:
    typedef boost::shared_ptr<const CEValues>                 CEVConstPtr;
    typedef boost::shared_ptr<const PDEPricingModelInterface> PDEModelPtr;
    
public:
    virtual ~PayoutExpression(){}
    
    virtual CEVConstPtr evalCE(
        const MOContext& mo_context,
        const AVContext& av_context
    ) const = 0;
    
    virtual CEVConstPtr evalCE(
        const PDEModelPtr& model
    ) const = 0;

    virtual CEVConstPtr evalFromFixings(
        const GenPDE::Date&       date,
        const MOFixingsIfc&       mo_fixings,
        const AuxiliaryVariables& av_defs,
        AVContext&                av_context // updated by the call
    ) const = 0;
    
    friend std::ostream& operator<<(std::ostream& stream, const PayoutExpression& pe);
    
protected:
    virtual void print(std::ostream& stream) const = 0;
};

#endif // PAYOUT_EXPRESSION_H
