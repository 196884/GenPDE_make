#ifndef GENPDE_PARSER_H
#define GENPDE_PARSER_H

#include <boost/shared_ptr.hpp>
#include <string>
#include <boost/spirit/include/support_istream_iterator.hpp>

#include "TradeParserBase.h"
#include "ModelParserBase.h"
#include "FixingsParserBase.h"

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

    typedef boost::shared_ptr<PDEPricingModelInterface>    ModelIfcPtr;
    typedef boost::shared_ptr<const MOFixingsIfc>          MOFixingsPtr;
    typedef boost::shared_ptr<const ChoiceFixings>         ChoiceFixingsPtr;
    
public:
    static PEPtr  parsePayoutExpression(        const std::string& s, bool is_file = false);
    static TLPtr  parseTradeLeg(                const std::string& s, bool is_file = false);
    static PIPtr  parsePricingInstruction(      const std::string& s, bool is_file = false);
    static TRPtr  parseTradeRepresentation(     const std::string& s, bool is_file = false);
    static AVsPtr parseAuxiliaryVariables(      const std::string& s, bool is_file = false);
    
    static ModelIfcPtr parsePDEModel(           const std::string& s, bool is_file = false);

    static MOFixingsPtr     parseMOFixings(     const std::string& s, bool is_file = false);
    static ChoiceFixingsPtr parseChoiceFixings( const std::string& s, bool is_file = false);

private:
    static TRParserBase<std::string::const_iterator>          m_tradeParserString;
    static TRParserBase<boost::spirit::istream_iterator>      m_tradeParserFile;

    static ModelParserBase<std::string::const_iterator>       m_modelParserString;
    static ModelParserBase<boost::spirit::istream_iterator>   m_modelParserFile;

    static FixingsParserBase<std::string::const_iterator>     m_fixingsParserString;
    static FixingsParserBase<boost::spirit::istream_iterator> m_fixingsParserFile;
};

#endif // GENPDE_PARSER_H
