#ifndef MODEL_PARSER_BASE_H
#define MODEL_PARSER_BASE_H

#include <string>

#include <boost/shared_ptr.hpp>

#include "Framework.h"

#include "GpDate.h"
#include "PDEPricingModelInterface.h"
#include "PDEPricingModelBlackScholes.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix.hpp>

namespace qi    = boost::spirit::qi;
namespace phx   = boost::phoenix;
namespace ascii = boost::spirit::ascii;

template <typename Iterator>
class ModelParserBase
{
protected:
    typedef boost::shared_ptr<PDEPricingModelInterface>    ModelIfcPtr;

public:
    ModelParserBase()
    {
        mDateAsStr = qi::repeat(8)[qi::char_("0-9")];
        mDate      = mDateAsStr[qi::_val = phx::bind(&GenPDE::dateFromString, qi::_1)];

        mBSModel   = (
            "<PDEPricingModelBlackScholes>"                                          >>
            qi::lexeme["<PricingDate value=\""            >> mDate       >> "\"/>" ] >>
            qi::lexeme["<Spot value=\""                   >> qi::double_ >> "\"/>" ] >>
            qi::lexeme["<RiskFreeRate value=\""           >> qi::double_ >> "\"/>" ] >>
            qi::lexeme["<Volatility value=\""             >> qi::double_ >> "\"/>" ] >>
            qi::lexeme["<MaxTimestepLength value=\""      >> qi::double_ >> "\"/>" ] >>
            qi::lexeme["<NbRannacherSteps value=\""       >> qi::double_ >> "\"/>" ] >>
            qi::lexeme["<MaxRannacherStepLength value=\"" >> qi::double_ >> "\"/>" ] >>
            qi::lexeme["<SpaceGridSize value=\""          >> qi::ulong_  >> "\"/>" ] >>
            qi::lexeme["<SpaceGridNbStdDevs value=\""     >> qi::double_ >> "\"/>" ] >>
            "</PDEPricingModelBlackScholes>"
        )[qi::_val = phx::construct<ModelIfcPtr>(phx::new_<PDEPricingModelBlackScholes>(qi::_1, qi::_2, qi::_3, qi::_4, qi::_5, qi::_6, qi::_7, qi::_8, qi::_9))];

        mModel     = mBSModel;
    }

    ModelIfcPtr parseModel(Iterator first, Iterator last)
    {
        ModelIfcPtr result;
        bool r = qi::phrase_parse(
            first,
            last,
            mModel,
            qi::space,
            result
        );

        if( !r )
        {
            std::cerr << "Stopped at:" << std::endl << std::string(first, last) << std::endl;
            Exception::raise("ModelParseBase::parseModel", "Could not parse model");
        }

        return result;
    }

protected:
    qi::rule<Iterator, std::string()>                 mDateAsStr;
    qi::rule<Iterator, GenPDE::Date()>                mDate;

    qi::rule<Iterator, ModelIfcPtr(), qi::space_type> mBSModel;
    qi::rule<Iterator, ModelIfcPtr(), qi::space_type> mModel;
};

#endif // MODEL_PARSER_BASE_H
