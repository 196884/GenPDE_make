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

std::ostream& operator<<(std::ostream& os, const MOFixings& fixings)
{
    os << "<MOFixings>" << std::endl;
    std::map<MOUid, std::map<GenPDE::Date, double> >::const_iterator itCurr = fixings.mFixings.begin();
    std::map<MOUid, std::map<GenPDE::Date, double> >::const_iterator itEnd  = fixings.mFixings.end();
    for(; itEnd != itCurr; ++itCurr )
    {
        MOUid uid = itCurr->first;
        const std::map<GenPDE::Date, double>& datedFixings = itCurr->second;
        std::map<GenPDE::Date, double>::const_iterator itCurr2 = datedFixings.begin();
        std::map<GenPDE::Date, double>::const_iterator itEnd2  = datedFixings.end();
        for(; itCurr2 != itEnd2; ++itCurr2)
            os << "<MOFixing uid=\""
               << uid
               << "\" date=\""
               << GenPDE::dateToString(itCurr2->first)
               << "\" value=\""
               << itCurr2->second
               << "\"/>"
               << std::endl;
    }
    os << "</MOFixings>" << std::endl;
    return os;
}

void ChoiceFixings::addFixing(
    const Choice::Uid&  uid,
    Choice::Chooser     chooser,
    const GenPDE::Date& date,
    Choice::Choice      choice
) 
{
    mFixings[chooser][uid][date] = choice;
}

boost::optional<Choice::Choice> ChoiceFixings::getFixing(
    const Choice::Uid&  uid,
    Choice::Chooser     chooser,
    const GenPDE::Date& date
) const
{
    std::map<Choice::Uid, std::map<GenPDE::Date, Choice::Choice> >::const_iterator it = mFixings[chooser].find(uid);
    if( it == mFixings[chooser].end() )
        return boost::optional<Choice::Choice>();
    std::map<GenPDE::Date, Choice::Choice>::const_iterator it2 = it->second.find(date);
    if( it2 == it->second.end() )
        return boost::optional<Choice::Choice>();
    return it2->second;
}

std::ostream& operator<<(std::ostream& os, const ChoiceFixings& fixings)
{
    os << "Not implemented" << std::endl;
    return os;
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
    Choice::Chooser     chooser,
    const GenPDE::Date& date,
    Choice::Choice      choice
)
{
    mChoiceFixings->addFixing(uid, chooser, date, choice);
}

boost::optional<Choice::Choice> TradeFixings::getChoiceFixing(
    const Choice::Uid&  uid,
    Choice::Chooser     chooser,
    const GenPDE::Date& date
) const
{
    return mChoiceFixings->getFixing(uid, chooser, date);
}

std::ostream& operator<<(std::ostream& os, const TradeFixings& fixings)
{
    os << "<TradeFixings>" << std::endl
       << *(fixings.mMOFixings)
       << *(fixings.mChoiceFixings)
       << "</TradeFixings>" << std::endl;
    return os;
}
