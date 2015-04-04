#ifndef PI_PRICER_CHOICE_H
#define PI_PRICER_CHOICE_H

#include <boost/shared_ptr.hpp>

#include "PricingInstruction.h"
#include "PricerUid.h"
#include "TradeLeg.h"
#include "ChoiceEnums.h"

class PIPricerChoice : public PricingInstruction
{
protected:
    typedef boost::shared_ptr<const TradeLeg> TLConstPtr;
    
public:
    virtual ~PIPricerChoice() {}
    
    PIPricerChoice(
        PricerUid         pricer_uid,
        Choice::Chooser   chooser,
        Choice::Uid       choice_uid,
        Choice::Choice    def,
        const TLConstPtr& trade_leg_0,
        const TLConstPtr& trade_leg_1
    );
    
    void apply(const ModelPtr& model, const TradePtr& trade) const;
    
    friend std::ostream& operator<<(std::ostream& stream, const PIPricerChoice& pi);
    
protected:
    virtual void print(std::ostream& stream) const;
    
protected:
    PricerUid          mPricerUid;
    Choice::Chooser    mChooser;
    Choice::Uid        mChoiceUid;
    Choice::Choice     mDefault;
    TLConstPtr         mTL0;
    TLConstPtr         mTL1;
};

#endif // PI_PRICER_CHOICE_H
