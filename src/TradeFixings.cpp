//
//  TradeFixings.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/31/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "TradeFixings.h"

boost::shared_ptr<const TradeFixings> TradeFixings::NoFixings = boost::shared_ptr<TradeFixings>(new TradeFixings());

void MOFixings::addFixing(MOUid mo_uid, const GenPDE::Date& date, double value)
{
    mFixings[mo_uid][date] = value;
}

boost::optional<double> MOFixings::getFixing(MOUid mo_uid, const GenPDE::Date& date) const
{
    std::map<MOUid, std::map<GenPDE::Date, double> >::const_iterator it = mFixings.find(mo_uid);
    if( it == mFixings.end() )
        return boost::optional<double>();
    std::map<GenPDE::Date, double>::const_iterator it2 = it->second.find(date);
    if( it2 == it->second.end() )
        return boost::optional<double>();
    return it2->second;
}

void ChoiceFixings::addFixing(
    const Choice::Uid&  uid,
    const GenPDE::Date& date,
    Choice::Choice      choice
) 
{
    mFixings[uid][date] = choice;
}

boost::optional<Choice::Choice> ChoiceFixings::getFixing(
    const Choice::Uid&  uid,
    const GenPDE::Date& date
) const
{
    std::map<Choice::Uid, std::map<GenPDE::Date, Choice::Choice> >::const_iterator it = mFixings.find(uid);
    if( it == mFixings.end() )
        return boost::optional<Choice::Choice>();
    std::map<GenPDE::Date, Choice::Choice>::const_iterator it2 = it->second.find(date);
    if( it2 == it->second.end() )
        return boost::optional<Choice::Choice>();
    return it2->second;
}

TradeFixings::TradeFixings()
{
    mMOFixings     = boost::shared_ptr<MOFixings>(new MOFixings());
    mChoiceFixings = boost::shared_ptr<ChoiceFixings>(new ChoiceFixings());
}
    
TradeFixings::TradeFixings(
    const MOFixingsPtr&     mo_fixings,
    const ChoiceFixingsPtr& choice_fixings
)
:mMOFixings(mo_fixings)
,mChoiceFixings(choice_fixings)
{}
    
void TradeFixings::addMOFixing(MOUid mo_uid, const GenPDE::Date& date, double value)
{
    mMOFixings->addFixing(mo_uid, date, value);
}

boost::optional<double> TradeFixings::getMOFixing(MOUid mo_uid, const GenPDE::Date& date) const
{
    return mMOFixings->getFixing(mo_uid, date);
}
    
void TradeFixings::addChoiceFixing(
    const Choice::Uid&  uid,
    const GenPDE::Date& date,
    Choice::Choice      choice
)
{
    mChoiceFixings->addFixing(uid, date, choice);
}

boost::optional<Choice::Choice> TradeFixings::getChoiceFixing(
    const Choice::Uid&  uid,
    const GenPDE::Date& date
) const
{
    return mChoiceFixings->getFixing(uid, date);
}

