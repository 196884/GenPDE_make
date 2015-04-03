#include <fstream>

#include "ModelParser.h"

ModelParserBase<std::string::const_iterator>     ModelParser::m_parserString;
ModelParserBase<boost::spirit::istream_iterator> ModelParser::m_parserFile;

ModelParser::ModelIfcPtr ModelParser::parsePDEModel(const std::string& model, bool is_file)
{
    if( is_file )
    {
        std::ifstream in(model);
        in.unsetf(std::ios::skipws);
        boost::spirit::istream_iterator begin(in);
        boost::spirit::istream_iterator end;
        return m_parserFile.parseModel(begin, end);
    } else
        return m_parserString.parseModel(model.begin(), model.end());
}
