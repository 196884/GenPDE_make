#ifndef PDE_TRADE_PRICER_H
#define PDE_TRADE_PRICER_H

#include <boost/shared_ptr.hpp>

#include "PricerUid.h"
#include "VariableEnums.h"
#include "ChoiceEnums.h"

class PDEPricingModelInterface;
class PayoutExpression;
class PricingInstruction;
class MOContext;
class AVContext;
class CEValues;
class TradeLeg;
class TradeRepresentation;

class PDETradePricer
{
protected:
    typedef boost::shared_ptr<PDEPricingModelInterface>  PDEModelPtr;
    typedef boost::shared_ptr<const PayoutExpression>    PEPtr;
    typedef boost::shared_ptr<const PricingInstruction>  PIPtr;
    typedef boost::shared_ptr<MOContext>                 MOCPtr;
    typedef boost::shared_ptr<AVContext>                 AVCPtr;
    typedef boost::shared_ptr<CEValues>                  CEVPtr;
    typedef boost::shared_ptr<const CEValues>            CEVConstPtr;
    typedef boost::shared_ptr<const TradeLeg>            TLPtr;
    typedef boost::shared_ptr<const TradeRepresentation> TRPtr;
    
public:
    PDETradePricer(
        const PDEModelPtr& pde_model,
        const TRPtr&       trade_representation
    )
    :mPDEModel(pde_model)
    ,mTradeRepresentation(trade_representation)
    {}
    
    double price();
    
    void timeStepToNextDate();
    
    void pricerInit(
        PricerUid       uid,
        const TLPtr&    leg
    );
    
    void pricerChoice(
        PricerUid       uid,
        Choice::Chooser chooser,
        const TLPtr&    leg_0,
        const TLPtr&    leg_1
    );
    
    void pricerAdd(
        PricerUid       uid,
        const TLPtr&    leg_0,
        const TLPtr&    leg_1
    );
    
    void pricerMult(
        PricerUid       uid,
        const PEPtr&    pe,
        const TLPtr&    leg
    );
    
    void removeAVDependency(PricerUid uid, GenPDE::VariableUID av_uid);
    
    void pricerRemove(PricerUid uid);

protected:
    PDEModelPtr mPDEModel;
    TRPtr       mTradeRepresentation;
};

#endif // PDE_TRADE_PRICER_H
