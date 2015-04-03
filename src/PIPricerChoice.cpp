//
//  PIPricerChoice.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/26/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "PIPricerChoice.h"

PIPricerChoice::PIPricerChoice(
    PricerUid         pricer_uid,
    Choice::Chooser   chooser,
    Choice::Uid       choice_uid,
    Choice::Choice    def,
    const TLConstPtr& trade_leg_0,
    const TLConstPtr& trade_leg_1
)
:mPricerUid(pricer_uid)
,mChooser(chooser)
,mChoiceUid(choice_uid)
,mDefault(def)
,mTL0(trade_leg_0)
,mTL1(trade_leg_1)
{}

void PIPricerChoice::apply(const ModelPtr& model, const TradePtr& trade) const
{
    CEVConstPtr     leg0Val(mTL0->evalCE(model));
    CEVConstPtr     leg1Val(mTL1->evalCE(model));
    VarDependencies avDeps(leg0Val->getVarMemoryLayout(), leg1Val->getVarMemoryLayout());
    CEVPtr          pricer(model->addPricer(mPricerUid, avDeps));
    switch (mChooser)
    {
    case Choice::Chooser_Us:
        pricer->evalBinaryOperation(GenPDE::Min, leg0Val, leg1Val);
        break;
    case Choice::Chooser_Client:
        pricer->evalBinaryOperation(GenPDE::Max, leg0Val, leg1Val);
        break;
    default:
        Exception::raise("PIPricerChoice::apply", "Unhandled chooser");
    }
}

void PIPricerChoice::print(std::ostream& stream) const
{
    stream << "<PricerChoice uid=\"" << mPricerUid << "\" chooser=\"";
    stream << ((mChooser == Choice::Chooser_Us) ? "Us" : "Client");
    stream << "\" choiceID=\"" << mChoiceUid << "\" default=\"";
    stream << ((mDefault == Choice::Choice_Leg0) ? "Leg0" : ((mDefault == Choice::Choice_Leg1) ? "Leg1" : "None"));
    stream << "\">" << *mTL0 << *mTL1 << "</PricerChoice>";
}

std::ostream& operator<<(std::ostream& stream, const PIPricerChoice& pa)
{
    pa.print(stream);
    return stream;
}
