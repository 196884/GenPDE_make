#ifndef MODEL_PARSER_H
#define MODEL_PARSER_H

#include <boost/spirit/include/support_istream_iterator.hpp>

#include "ModelParserBase.h"

class ModelParser
{
protected:
    typedef boost::shared_ptr<PDEPricingModelInterface>    ModelIfcPtr;

public:
    static ModelIfcPtr parsePDEModel( const std::string& s, bool is_file = false );

private:
    static ModelParserBase<std::string::const_iterator>     m_parserString;
    static ModelParserBase<boost::spirit::istream_iterator> m_parserFile;
};

#endif // MODEL_PARSER_H
