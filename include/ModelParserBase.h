#ifndef MODEL_PARSER_BASE_H
#define MODEL_PARSER_BASE_H

#include <string>

#include <boost/shared_ptr.hpp>

#include "Framework.h"

#include "GpDate.h"
#include "PDEPricingModelInterface.h"
#include "PDEPricingModelBlackScholes.h"
#include "BSModelParameters.h"
#include "PDEParameters1D.h"

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

        mAVDPNone  = (
            qi::string("<AVDiscretizationPolicy type=\"None\"/>")
        )[qi::_val = phx::construct<AVDiscretizationPolicy*>(phx::new_<AVDP_None>())];

        mAVDPSum   = (
            qi::string("<AVDiscretizationPolicy type=\"Sum\">")                      >>
            "<NbValues value=\"" >> qi::uint_ >> "\"/>"                              >>
            "<NbStdDevs value=\"" >> qi::double_ >> "\"/>"                           >>
            "</AVDiscretizationPolicy>"
        )[qi::_val = phx::construct<AVDiscretizationPolicy*>(phx::new_<AVDP_Sum>(qi::_2, qi::_3))];

        mAVDisc    = (
              mAVDPNone
            | mAVDPSum
        );

        mBSModelParams  = (
            qi::lexeme["<Spot value=\""                   >> qi::double_ >> "\"/>" ] >>
            qi::lexeme["<RiskFreeRate value=\""           >> qi::double_ >> "\"/>" ] >>
            qi::lexeme["<Volatility value=\""             >> qi::double_ >> "\"/>" ]
        )[qi::_val = phx::construct<BSModelParameters>(qi::_1, qi::_2, qi::_3)];

        mPDEParams1D    = (
            qi::lexeme["<MaxTimestepLength value=\""      >> qi::double_ >> "\"/>" ] >>
            qi::lexeme["<NbRannacherSteps value=\""       >> qi::double_ >> "\"/>" ] >>
            qi::lexeme["<MaxRannacherStepLength value=\"" >> qi::double_ >> "\"/>" ] >>
            qi::lexeme["<SpaceGridSize value=\""          >> qi::ulong_  >> "\"/>" ] >>
            qi::lexeme["<SpaceGridNbStdDevs value=\""     >> qi::double_ >> "\"/>" ]
        )[qi::_val = phx::construct<PDEParameters1D>(qi::_1, qi::_2, qi::_3, qi::_4, qi::_5)];

        mBSModel   = (
            "<PDEPricingModelBlackScholes>"                         >>
            qi::lexeme["<PricingDate value=\"" >> mDate >> "\"/>" ] >>
            mBSModelParams                                          >>
            mPDEParams1D                                            >>   
            mAVDisc                                                 >>
            "</PDEPricingModelBlackScholes>"
        )[qi::_val = phx::construct<ModelIfcPtr>(phx::new_<PDEPricingModelBlackScholes>(qi::_1, qi::_2, qi::_3, qi::_4))];

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

    qi::rule<Iterator, AVDiscretizationPolicy*(), qi::space_type> mAVDPNone;
    qi::rule<Iterator, AVDiscretizationPolicy*(), qi::space_type> mAVDPSum;
    qi::rule<Iterator, AVDiscretizationPolicy*(), qi::space_type> mAVDisc;
    qi::rule<Iterator, BSModelParameters(),       qi::space_type> mBSModelParams;
    qi::rule<Iterator, PDEParameters1D(),         qi::space_type> mPDEParams1D;
    qi::rule<Iterator, ModelIfcPtr(),             qi::space_type> mBSModel;
    qi::rule<Iterator, ModelIfcPtr(),             qi::space_type> mModel;
};

#endif // MODEL_PARSER_BASE_H
