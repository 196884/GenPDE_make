#include <fstream>

#include "GenPDEParser.h"

TRParserBase<std::string::const_iterator>                GenPDEParser::m_tradeParserString;
TRParserBase<boost::spirit::istream_iterator>            GenPDEParser::m_tradeParserFile;
ModelParserBase<std::string::const_iterator>             GenPDEParser::m_modelParserString;
ModelParserBase<boost::spirit::istream_iterator>         GenPDEParser::m_modelParserFile;
FixingsParserBase<std::string::const_iterator>           GenPDEParser::m_fixingsParserString;
FixingsParserBase<boost::spirit::istream_iterator>       GenPDEParser::m_fixingsParserFile;

GenPDEParser::PEPtr  GenPDEParser::parsePayoutExpression(const std::string& pe, bool is_file)
{
    if( is_file )
    {
        std::ifstream in(pe);
        in.unsetf(std::ios::skipws);
        boost::spirit::istream_iterator begin(in);
        boost::spirit::istream_iterator end;
        return m_tradeParserFile.parsePE(begin, end);
    } else
        return m_tradeParserString.parsePE(pe.begin(), pe.end());
}

GenPDEParser::TLPtr  GenPDEParser::parseTradeLeg(const std::string& tl, bool is_file)
{
    if( is_file )
    {
        std::ifstream in(tl);
        in.unsetf(std::ios::skipws);
        boost::spirit::istream_iterator begin(in);
        boost::spirit::istream_iterator end;
        return m_tradeParserFile.parseTL(begin, end);
    } else
        return m_tradeParserString.parseTL(tl.begin(), tl.end());
}

GenPDEParser::PIPtr  GenPDEParser::parsePricingInstruction(const std::string& pi, bool is_file)
{
    if( is_file )
    {
        std::ifstream in(pi);
        in.unsetf(std::ios::skipws);
        boost::spirit::istream_iterator begin(in);
        boost::spirit::istream_iterator end;
        return m_tradeParserFile.parsePI(begin, end);
    } else
        return m_tradeParserString.parsePI(pi.begin(), pi.end());
}

GenPDEParser::TRPtr  GenPDEParser::parseTradeRepresentation(const std::string& tr, bool is_file)
{
    if( is_file )
    {
        std::ifstream in(tr);
        in.unsetf(std::ios::skipws);
        boost::spirit::istream_iterator begin(in);
        boost::spirit::istream_iterator end;
        return m_tradeParserFile.parseTR(begin, end);
    } else
        return m_tradeParserString.parseTR(tr.begin(), tr.end());
}

GenPDEParser::AVsPtr GenPDEParser::parseAuxiliaryVariables(const std::string& avc, bool is_file)
{
    if( is_file )
    {
        std::ifstream in(avc);
        in.unsetf(std::ios::skipws);
        boost::spirit::istream_iterator begin(in);
        boost::spirit::istream_iterator end;
        return m_tradeParserFile.parseAVs(begin, end);
    } else
        return m_tradeParserString.parseAVs(avc.begin(), avc.end());
}

GenPDEParser::ModelIfcPtr GenPDEParser::parsePDEModel(const std::string& model, bool is_file)
{
    if( is_file )
    {
        std::ifstream in(model);
        in.unsetf(std::ios::skipws);
        boost::spirit::istream_iterator begin(in);
        boost::spirit::istream_iterator end;
        return m_modelParserFile.parseModel(begin, end);
    } else
        return m_modelParserString.parseModel(model.begin(), model.end());
}

GenPDEParser::MOFixingsPtr GenPDEParser::parseMOFixings(const std::string& model, bool is_file)
{
    if( is_file )
    {
        std::ifstream in(model);
        in.unsetf(std::ios::skipws);
        boost::spirit::istream_iterator begin(in);
        boost::spirit::istream_iterator end;
        return m_fixingsParserFile.parseMOFixings(begin, end);
    } else
        return m_fixingsParserString.parseMOFixings(model.begin(), model.end());
}

GenPDEParser::ChoiceFixingsPtr GenPDEParser::parseChoiceFixings(const std::string& model, bool is_file)
{
    if( is_file )
    {
        std::ifstream in(model);
        in.unsetf(std::ios::skipws);
        boost::spirit::istream_iterator begin(in);
        boost::spirit::istream_iterator end;
        return m_fixingsParserFile.parseChoiceFixings(begin, end);
    } else
        return m_fixingsParserString.parseChoiceFixings(model.begin(), model.end());
}


