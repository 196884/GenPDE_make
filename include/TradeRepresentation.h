#ifndef TRADE_REPRESENTATION_H
#define TRADE_REPRESENTATION_H

#include <boost/shared_ptr.hpp> 
#include "AuxiliaryVariable.h"
#include "TradeLeg.h"
#include "PricingInstructions.h"

class TradeRepresentation
{
protected:
    typedef boost::shared_ptr<const AuxiliaryVariables>       AVsConstPtr;
    typedef boost::shared_ptr<const TradeLeg>                 TLConstPtr;
    typedef boost::shared_ptr<const DatedPricingInstructions> DPISConstPtr;
    
public:
    virtual ~TradeRepresentation() {}
    
    TradeRepresentation(
        const DPISConstPtr& dated_pricing_instructions,
        const TLConstPtr&   main_trade_leg,
        const AVsConstPtr&  auxiliary_variables
    )
    :mDPIs(dated_pricing_instructions)
    ,mMainTradeLeg(main_trade_leg)
    ,mAVs(auxiliary_variables)
    {}
    
    AVsConstPtr  getAuxiliaryVariables()       const { return mAVs;          }
    DPISConstPtr getDatedPricingInstructions() const { return mDPIs;         }
    TLConstPtr   getMainTradeLeg()             const { return mMainTradeLeg; }
    
protected:
    DPISConstPtr mDPIs;
    TLConstPtr   mMainTradeLeg;
    AVsConstPtr  mAVs;
};

#endif // TRADE_REPRESENTATION_H
