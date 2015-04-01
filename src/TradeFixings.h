//
//  TradeFixings.h
//  GenPDE
//
//  Created by Regis Dupont on 8/28/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef TRADEFIXINGS_H_
#define TRADEFIXINGS_H_

#include <boost/optional.hpp>
#include <map>

#include "Date.h"
#include "MarketObservable.h"
#include "ChoiceEnums.h"

class MOFixings
{
public:
    boost::optional<double> getFixing(MOUid mo_uid, const GenPDE::Date& date) const
    {
        std::map<MOUid, std::map<GenPDE::Date, double> >::const_iterator it = mFixings.find(mo_uid);
        if( it == mFixings.end() )
            return boost::optional<double>();
        std::map<GenPDE::Date, double>::const_iterator it2 = it->second.find(date);
        if( it2 == it->second.end() )
            return boost::optional<double>();
        return it2->second;
    }
    
protected:
    std::map<MOUid, std::map<GenPDE::Date, double> > mFixings;
};

class ChoiceFixings
{
public:
    boost::optional<Choice::Choice> getFixing(
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
    
protected:
    std::map<Choice::Uid, std::map<GenPDE::Date, Choice::Choice> > mFixings;
};

class TradeFixings
{
protected:
    typedef boost::shared_ptr<const MOFixings>     MOFixingsPtr;
    typedef boost::shared_ptr<const ChoiceFixings> ChoiceFixingsPtr;
    
public:
    TradeFixings()
    {
        mMOFixings = boost::shared_ptr<MOFixings>(new MOFixings());
        mChoiceFixings = boost::shared_ptr<ChoiceFixings>(new ChoiceFixings());
    }
    
    TradeFixings(
        const MOFixingsPtr&     mo_fixings,
        const ChoiceFixingsPtr& choice_fixings
    )
    :mMOFixings(mo_fixings)
    ,mChoiceFixings(choice_fixings)
    {}
    
    boost::optional<double> getMOFixing(MOUid mo_uid, const GenPDE::Date& date) const
    {
        return mMOFixings->getFixing(mo_uid, date);
    }
    
    boost::optional<Choice::Choice> getChoiceFixing(
        const Choice::Uid&  uid,
        const GenPDE::Date& date
    ) const
    {
        return mChoiceFixings->getFixing(uid, date);
    }
    
    static boost::shared_ptr<const TradeFixings> NoFixings;
    
protected:
    MOFixingsPtr     mMOFixings;
    ChoiceFixingsPtr mChoiceFixings;
};

#endif // TRADEFIXINGS_H_