#ifndef TRADE_LEG_H
#define TRADE_LEG_H

#include <iostream>
#include <boost/shared_ptr.hpp>

#include "CEValues.h"
#include "PDEPricingModelInterface.h"

class TradeLeg
{
protected:
    typedef boost::shared_ptr<const CEValues> CEVConstPtr;
    typedef boost::shared_ptr<const PDEPricingModelInterface> PDEModelPtr;
    
public:
    virtual ~TradeLeg() {}
    
    virtual CEVConstPtr evalCE(
        const PDEModelPtr& model
    ) const = 0;
    
    friend std::ostream& operator<<(std::ostream& stream, const TradeLeg& leg);
    
protected:
    virtual void print(std::ostream& stream) const = 0;
};

#endif // TRADE_LEG_H
