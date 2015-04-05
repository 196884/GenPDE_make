#ifndef TRADE_FIXINGS_H
#define TRADE_FIXINGS_H

#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

#include "GpDate.h"
#include "MarketObservable.h"
#include "ChoiceEnums.h"

class CEValues;

/// The interface for providing Market Observable fixings:
class MOFixingsIfc
{
public:
    typedef boost::shared_ptr<       CEValues > CEValuesPtr;
    typedef boost::shared_ptr< const CEValues > CEValuesCPtr;

public:
    virtual ~MOFixingsIfc() {}

    virtual CEValuesCPtr getFixing( MOUid mo_uid, const GenPDE::Date& date ) const = 0;

    friend std::ostream& operator<<( std::ostream& os, const MOFixingsIfc& fixings );

protected:
    virtual void dump( std::ostream& os ) const;
};

/// Basic implementation that stores the data
class MOFixingsStore : public MOFixingsIfc
{
public:
    MOFixingsStore() {}
    
    virtual ~MOFixingsStore() {}

    // MOFixingsIfc
    ///////////////
    virtual CEValuesCPtr getFixing( MOUid mo_uid, const GenPDE::Date& date ) const;

    void                 addFixing( MOUid mo_uid, const GenPDE::Date& date, double value );
   
protected:
    virtual void dump( std::ostream& os ) const;

protected:
    typedef std::map< GenPDE::Date, double >         DatedValues;
    typedef std::map< MOUid, DatedValues >           FixingsMap;

    FixingsMap                                       m_fixings;
};

class ChoiceFixings
{
public:
    void                            addFixing(
        const Choice::Uid&  uid, 
        Choice::Chooser     chooser,
        const GenPDE::Date& date, 
        Choice::Choice      choice
    );

    boost::optional<Choice::Choice> getFixing(
        const Choice::Uid&  uid,
        Choice::Chooser     chooser,
        const GenPDE::Date& date
    ) const;

    friend std::ostream& operator<<(std::ostream& stream, const ChoiceFixings& fixings);
   
protected:
    std::map<Choice::Uid, std::map<GenPDE::Date, Choice::Choice> > mFixings[2];
};

#endif // TRADE FIXINGS_H
