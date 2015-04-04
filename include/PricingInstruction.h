#ifndef PRICING_INSTRUCTION_H
#define PRICING_INSTRUCTION_H

#include <iostream>
#include <boost/shared_ptr.hpp>

class PDEPricingModelInterface;
class TradeRepresentation;
class CEValues;

class PricingInstruction
{
protected:
    typedef boost::shared_ptr<PDEPricingModelInterface>  ModelPtr;
    typedef boost::shared_ptr<const TradeRepresentation> TradePtr;
    typedef boost::shared_ptr<CEValues>                  CEVPtr;
    typedef boost::shared_ptr<const CEValues>            CEVConstPtr;
    
public:
    virtual ~PricingInstruction() {}
    
    virtual void apply(const ModelPtr& model, const TradePtr& trade) const = 0;
    
    friend std::ostream& operator<<(std::ostream& stream, const PricingInstruction&);
    
protected:
    virtual void print(std::ostream& stream) const = 0;
};

#endif // PRICING_INSTRUCTION_H
