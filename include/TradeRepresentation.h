//
//  TradeRepresentation.h
//  GenPDE
//
//  Created by Regis Dupont on 8/27/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef TRADEREPRESENTATION_H_
#define TRADEREPRESENTATION_H_

#include <boost/shared_ptr.hpp> 
#include "AVContext.h"
#include "TradeLeg.h"
#include "PricingInstructions.h"

class TradeRepresentation
{
protected:
    typedef boost::shared_ptr<AVContext>                      AVCPtr;
    typedef boost::shared_ptr<const TradeLeg>                 TLConstPtr;
    typedef boost::shared_ptr<const DatedPricingInstructions> DPISConstPtr;
    
public:
    virtual ~TradeRepresentation() {}
    
    TradeRepresentation(
        const DPISConstPtr& dated_pricing_instructions,
        const TLConstPtr&   main_trade_leg,
        const AVCPtr&       av_context
    )
    :mDPIs(dated_pricing_instructions)
    ,mMainTradeLeg(main_trade_leg)
    ,mAVContext(av_context)
    {}
    
    AVCPtr       getAVContext() { return mAVContext; }
    
    DPISConstPtr getDatedPricingInstructions() const { return mDPIs; }
    
    TLConstPtr   getMainTradeLeg() const { return mMainTradeLeg; }
    
protected:
    DPISConstPtr mDPIs;
    TLConstPtr   mMainTradeLeg;
    // FIXME: Rigorously, this should be the definition of the AVs, not their discretization...
    AVCPtr       mAVContext;
};

#endif // TRADEREPRESENTATION_H_
