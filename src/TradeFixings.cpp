#include "TradeFixings.h"
#include "CEValuesStored.h"

std::ostream& operator<<( std::ostream& os, const MOFixingsIfc& fixings )
{
    fixings.dump( os );
    return os;
}

void MOFixingsIfc::dump( std::ostream& os ) const 
{
    os << "MOFixingsIfc - dump not implemented" << std::endl;
}

MOFixingsStore::CEValuesCPtr MOFixingsStore::getFixing(
    MOUid               mo_uid,
    const GenPDE::Date& date
) const
{
    FixingsMap::const_iterator it1 = m_fixings.find( mo_uid );
    if( it1 == m_fixings.end() )
        return CEValuesPtr();
    
    const DatedValues& datedValues = it1->second;
    DatedValues::const_iterator it2 = datedValues.find( date );
    if( it2 == datedValues.end() )
        return CEValuesPtr();

    return CEValuesPtr(new CEValuesStored( it2->second ) );
}

void MOFixingsStore::addFixing(MOUid mo_uid, const GenPDE::Date& date, double value)
{
    m_fixings[mo_uid][date] = value;
}

void MOFixingsStore::dump( std::ostream& os ) const
{
    os << "<MOFixings>" << std::endl;
    std::map<MOUid, std::map<GenPDE::Date, double> >::const_iterator itCurr = m_fixings.begin();
    std::map<MOUid, std::map<GenPDE::Date, double> >::const_iterator itEnd  = m_fixings.end();
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
    // Argh...
    os << "<ChoiceFixings>" << std::endl;
    for(size_t i=0; i<2; ++i)
    {
        const std::map<Choice::Uid, std::map<GenPDE::Date, Choice::Choice> >& map1 = fixings.mFixings[i];
        std::map<Choice::Uid, std::map<GenPDE::Date, Choice::Choice> >::const_iterator itCurr1 = map1.begin();
        std::map<Choice::Uid, std::map<GenPDE::Date, Choice::Choice> >::const_iterator itEnd1  = map1.end();
        for(; itEnd1 != itCurr1; ++itCurr1)
        {
            Choice::Uid cUid                                   = itCurr1->first;
            const std::map<GenPDE::Date, Choice::Choice>& map2 = itCurr1->second;
            std::map<GenPDE::Date, Choice::Choice>::const_iterator itCurr2 = map2.begin();
            std::map<GenPDE::Date, Choice::Choice>::const_iterator itEnd2  = map2.end();
            for(; itEnd2 != itCurr2; ++itCurr2)
            {
                os << "<ChoiceFixing choiceId=\""
                   << cUid
                   << "\" chooser=\""
                   << ((i == Choice::Chooser_Us) ? "Us" : "Client")
                   << "\" date=\""
                   << GenPDE::dateToString(itCurr2->first)
                   << "\" choice=\""
                   << ((itCurr2->second == Choice::Choice_Leg0) ? "Leg0" : "Leg1")
                   << "\"/>" << std::endl;
            }
        }
    }
    os << "</ChoiceFixings>" << std::endl;
    return os;
}

