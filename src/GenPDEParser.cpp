//
//  GenPDEParser.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/30/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include <fstream>

#include "GenPDEParser.h"

TRParserBase<std::string::const_iterator>     GenPDEParser::mParserString;
TRParserBase<boost::spirit::istream_iterator> GenPDEParser::mParserFile;

GenPDEParser::PEPtr  GenPDEParser::parsePayoutExpression(const std::string& pe, bool is_file)
{
    if( is_file )
    {
        std::ifstream in(pe);
        in.unsetf(std::ios::skipws);
        boost::spirit::istream_iterator begin(in);
        boost::spirit::istream_iterator end;
        return mParserFile.parsePE(begin, end);
    } else
        return mParserString.parsePE(pe.begin(), pe.end());
}

GenPDEParser::TLPtr  GenPDEParser::parseTradeLeg(const std::string& tl, bool is_file)
{
    if( is_file )
    {
        std::ifstream in(tl);
        in.unsetf(std::ios::skipws);
        boost::spirit::istream_iterator begin(in);
        boost::spirit::istream_iterator end;
        return mParserFile.parseTL(begin, end);
    } else
        return mParserString.parseTL(tl.begin(), tl.end());
}

GenPDEParser::PIPtr  GenPDEParser::parsePricingInstruction(const std::string& pi, bool is_file)
{
    if( is_file )
    {
        std::ifstream in(pi);
        in.unsetf(std::ios::skipws);
        boost::spirit::istream_iterator begin(in);
        boost::spirit::istream_iterator end;
        return mParserFile.parsePI(begin, end);
    } else
        return mParserString.parsePI(pi.begin(), pi.end());
}

GenPDEParser::TRPtr  GenPDEParser::parseTradeRepresentation(const std::string& tr, bool is_file)
{
    if( is_file )
    {
        std::ifstream in(tr);
        in.unsetf(std::ios::skipws);
        boost::spirit::istream_iterator begin(in);
        boost::spirit::istream_iterator end;
        return mParserFile.parseTR(begin, end);
    } else
        return mParserString.parseTR(tr.begin(), tr.end());
}

GenPDEParser::AVCPtr GenPDEParser::parseAVContext(const std::string& avc, bool is_file)
{
    if( is_file )
    {
        std::ifstream in(avc);
        in.unsetf(std::ios::skipws);
        boost::spirit::istream_iterator begin(in);
        boost::spirit::istream_iterator end;
        return mParserFile.parseAVC(begin, end);
    } else
        return mParserString.parseAVC(avc.begin(), avc.end());
}


