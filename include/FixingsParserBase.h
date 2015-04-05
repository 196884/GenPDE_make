#ifndef FIXINGS_PARSER_BASE_H
#define FIXINGS_PARSER_BASE_H

#include <string>

#include <boost/shared_ptr.hpp>

#include "Framework.h"

#include "GpDate.h"

#include "TradeFixings.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix.hpp>

namespace qi  = boost::spirit::qi;
namespace phx = boost::phoenix;
namespace ascii = boost::spirit::ascii;

template <typename Iterator>
class FixingsParserBase
{
protected:
    typedef boost::shared_ptr< const MOFixingsIfc >  MOFixingsCPtr;
    typedef boost::shared_ptr< const ChoiceFixings > ChoiceFixingsCPtr;

public:
    FixingsParserBase()
    {
        mDateAsStr    = qi::repeat(8)[qi::char_("0-9")];
        mDate         = mDateAsStr[qi::_val = phx::bind(&GenPDE::dateFromString, qi::_1)];
        mChooser      =   qi::string("Us")[qi::_val = Choice::Chooser_Us]
                        | qi::string("Client")[qi::_val = Choice::Chooser_Client];
        mChoice       =   qi::string("None")[qi::_val = Choice::Choice_None]
                        | qi::string("Leg0")[qi::_val = Choice::Choice_Leg0]
                        | qi::string("Leg1")[qi::_val = Choice::Choice_Leg1];
        mChoiceId     = +(~qi::char_('"'));

        mMOFixing     = ("<MOFixing uid=\"" >> qi::uint_ >> "\" date=\"" >> mDate >> "\" value=\"" >> qi::double_ >> "\"/>")
                        [phx::bind(&MOFixingsStore::addFixing, qi::_r1, qi::_1, qi::_2, qi::_3)];

        mChoiceFixing = ("<ChoiceFixing choiceId=\"" >> mChoiceId >> "\" chooser=\"" >> mChooser >> "\" date=\"" >> mDate >> "\" choice=\"" >> mChoice >> "\"/>")
                        [phx::bind(&ChoiceFixings::addFixing, qi::_r1, qi::_1, qi::_2, qi::_3, qi::_4)];

        mMOFixings    = (
            qi::eps[qi::_val = phx::construct<MOFixingsStore*>(phx::new_<MOFixingsStore>())] >>
            "<MOFixings>"            >>
            *mMOFixing(qi::_val)     >>
            "</MOFixings>"      
        );

        mChoiceFixings = (
            qi::eps[qi::_val = phx::construct<ChoiceFixings*>(phx::new_<ChoiceFixings>())] >>
            "<ChoiceFixings>"        >>
            *mChoiceFixing(qi::_val) >>
            "</ChoiceFixings>"
        );
    }

    MOFixingsCPtr parseMOFixings( Iterator first, Iterator last )
    {
        MOFixingsStore* result;
        bool r = qi::phrase_parse(
            first,
            last,
            mMOFixings,
            qi::space,
            result
        );
        if( !r )
        {
            std::cerr << "Stopped at:" << std::endl << std::string(first, last) << std::endl;
            Exception::raise("GenPDEParser::parseFixings", "Could not parse TradeFixings");
        }
        return MOFixingsCPtr(result);
    }

    ChoiceFixingsCPtr parseChoiceFixings( Iterator first, Iterator last )
    {
        ChoiceFixings* result;
        bool r = qi::phrase_parse(
            first,
            last,
            mChoiceFixings,
            qi::space,
            result
        );
        if( !r )
        {
            std::cerr << "Stopped at:" << std::endl << std::string(first, last) << std::endl;
            Exception::raise("GenPDEParser::parseFixings", "Could not parse TradeFixings");
        }
        return ChoiceFixingsCPtr(result);
    }


protected:
    qi::rule<Iterator, std::string()                   > mDateAsStr;
    qi::rule<Iterator, GenPDE::Date()                  > mDate;
    qi::rule<Iterator, Choice::Chooser()               > mChooser;
    qi::rule<Iterator, Choice::Choice()                > mChoice;
    qi::rule<Iterator, Choice::Uid()                   > mChoiceId;

    qi::rule<Iterator, MOFixingsStore*(),      qi::space_type> mMOFixings;
    qi::rule<Iterator, void(MOFixingsStore*),  qi::space_type> mMOFixing;

    qi::rule<Iterator, ChoiceFixings*(),       qi::space_type> mChoiceFixings;
    qi::rule<Iterator, void(ChoiceFixings*),   qi::space_type> mChoiceFixing;
};

#endif // FIXINGS_PARSER_BASE_H
