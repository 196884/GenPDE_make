//
//  GenPDEParser.h
//  GenPDE
//
//  Created by Regis Dupont on 8/30/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef GENPDEPARSER_H_
#define GENPDEPARSER_H_

#include <boost/shared_ptr.hpp>
#include <string>
#include <boost/spirit/include/support_istream_iterator.hpp>

#include "TRParserBase.h"

class PayoutExpression;
class TradeLeg;
class PricingInstruction;
class TradeRepresentation;
class AuxiliaryVariables;

class GenPDEParser
{
protected:
    typedef boost::shared_ptr<const PayoutExpression>    PEPtr;
    typedef boost::shared_ptr<const TradeLeg>            TLPtr;
    typedef boost::shared_ptr<const PricingInstruction>  PIPtr;
    typedef boost::shared_ptr<const TradeRepresentation> TRPtr;
    typedef boost::shared_ptr<const AuxiliaryVariables>  AVsPtr;
    
public:
    static PEPtr  parsePayoutExpression(   const std::string& s, bool is_file = false);
    static TLPtr  parseTradeLeg(           const std::string& s, bool is_file = false);
    static PIPtr  parsePricingInstruction( const std::string& s, bool is_file = false);
    static TRPtr  parseTradeRepresentation(const std::string& s, bool is_file = false);
    static AVsPtr parseAuxiliaryVariables( const std::string& s, bool is_file = false);
    
private:
    static TRParserBase<std::string::const_iterator>     mParserString;
    static TRParserBase<boost::spirit::istream_iterator> mParserFile;
};

#endif
