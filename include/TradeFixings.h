#ifndef TRADE_FIXINGS_H
#define TRADE_FIXINGS_H

#include <boost/optional.hpp>
#include <map>

#include "GpDate.h"
#include "MarketObservable.h"
#include "ChoiceEnums.h"

class MOFixings
{
public:
    void                    addFixing(MOUid mo_uid, const GenPDE::Date& date, double value);

    boost::optional<double> getFixing(MOUid mo_uid, const GenPDE::Date& date) const;
   
protected:
    std::map<MOUid, std::map<GenPDE::Date, double> > mFixings;
};

class ChoiceFixings
{
public:
    void                            addFixing(
        const Choice::Uid&  uid, 
        const GenPDE::Date& date, 
        Choice::Choice      choice
    );

    boost::optional<Choice::Choice> getFixing(
        const Choice::Uid&  uid,
        const GenPDE::Date& date
    ) const;
   
protected:
    std::map<Choice::Uid, std::map<GenPDE::Date, Choice::Choice> > mFixings;
};

class TradeFixings
{
protected:
    typedef boost::shared_ptr<MOFixings>     MOFixingsPtr;
    typedef boost::shared_ptr<ChoiceFixings> ChoiceFixingsPtr;
    
public:
    TradeFixings();

    TradeFixings(
        const MOFixingsPtr&     mo_fixings,
        const ChoiceFixingsPtr& choice_fixings
    );
    
    void                    addMOFixing(MOUid mo_uid, const GenPDE::Date& date, double value);
    boost::optional<double> getMOFixing(MOUid mo_uid, const GenPDE::Date& date) const;
    
    void                            addChoiceFixing(
        const Choice::Uid&  uid,
        const GenPDE::Date& date,
        Choice::Choice      choice
    );
    boost::optional<Choice::Choice> getChoiceFixing(
        const Choice::Uid&  uid,
        const GenPDE::Date& date
    ) const;
    
    static boost::shared_ptr<const TradeFixings> NoFixings;
    
protected:
    MOFixingsPtr     mMOFixings;
    ChoiceFixingsPtr mChoiceFixings;
};

#endif // TRADE FIXINGS_H
