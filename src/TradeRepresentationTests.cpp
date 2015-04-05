#include "Framework.h"
#include "TestSuite.h"

#include "AuxiliaryVariable.h"
#include "GenPDEParser.h"

REGISTER_TEST(AVsParsing1)
{
    std::string s0("<AuxiliaryVariables></AuxiliaryVariables>");
    boost::shared_ptr<const AuxiliaryVariables> avs0 = GenPDEParser::parseAuxiliaryVariables(s0);
    
    std::stringstream ss;
    ss << "<AuxiliaryVariables>";
    ss << "<AV uid=\"1\" date=\"20120101\"><MORef uid=\"1\"/></AV>";
    ss << "<AV uid=\"3\" date=\"20120102\"><MORef uid=\"1\"/></AV>";
    ss << "<AV uid=\"5\" date=\"20120103\"><MORef uid=\"2\"/></AV>";
    ss << "<AV uid=\"7\" date=\"20120104\"><MORef uid=\"1\"/></AV>";
    ss << "<AV uid=\"11\" date=\"20120105\"><MORef uid=\"1\"/></AV>";
    ss << "<AV uid=\"13\" date=\"20120106\"><MORef uid=\"2\"/></AV>";
    ss << "</AuxiliaryVariables>";
    std::string s(ss.str());
    boost::shared_ptr<const AuxiliaryVariables> avs1 = GenPDEParser::parseAuxiliaryVariables(s);
    std::vector<GenPDE::VariableUID> avUids = {1, 3, 5, 7, 11, 13};
    for(GenPDE::VariableUID avUid : avUids)
        TEST_EQ(avs1->getAuxiliaryVariable(avUid)->getUid(), avUid);
}

REGISTER_TEST(TradeRepresentationParsing1)
{
    // A zero coupon...
    std::stringstream ss;
    ss << "<TradeRepresentation>";
    ss << "<DatedPricingInstructions>";
    ss << "</DatedPricingInstructions>";
    ss << "<MainTradeLeg>";
    ss << "<Payment date=\"20140101\">";
    ss << "<Const value=\"1\"/>";
    ss << "</Payment>";
    ss << "</MainTradeLeg>";
    ss << "<AuxiliaryVariables>";
    ss << "</AuxiliaryVariables>";
    ss << "</TradeRepresentation>";
    std::string s0(ss.str());
    boost::shared_ptr<const TradeRepresentation> tr0 = GenPDEParser::parseTradeRepresentation(s0);
    TEST( true );
    
    // A forward starting call
    std::stringstream ss1;
    ss1 << "<TradeRepresentation>";
    ss1 << "<DatedPricingInstructions>";
    
    ss1 << "<PricingInstructions date=\"20160101\">";
    ss1 << "<PricerInit uid=\"1\">";
    ss1 << "<Payment date=\"20160105\">";
    ss1 << "<BinOp op=\"Max\">";
    ss1 << "<BinOp op=\"Sub\">";
    ss1 << "<BinOp op=\"Div\">";
    ss1 << "<MORef uid=\"1\"/>";
    ss1 << "<AVRef uid=\"1\"/>";
    ss1 << "</BinOp>";
    ss1 << "<Const value=\"1\"/>";
    ss1 << "</BinOp>";
    ss1 << "<Const value=\"0\"/>";
    ss1 << "</BinOp>";
    ss1 << "</Payment>";
    ss1 << "</PricerInit>";
    ss1 << "</PricingInstructions>";
    
    ss1 << "<PricingInstructions date=\"20150101\">";
    ss1 << "<PricerRmAVDeps uid=\"1\" AVs=\"1\"/>";
    ss1 << "</PricingInstructions>";
    
    ss1 << "</DatedPricingInstructions>";
    
    ss1 << "<MainTradeLeg>";
    ss1 << "<PricerRef uid=\"1\"/>";
    ss1 << "</MainTradeLeg>";
    
    ss1 << "<AuxiliaryVariables>";
    ss1 << "<AV uid=\"1\" date=\"20150101\">";
    ss1 << "<MORef uid=\"1\"/>";
    ss1 << "</AV>";
    ss1 << "</AuxiliaryVariables>";
    
    ss1 << "</TradeRepresentation>";
    std::string s1(ss1.str());
    boost::shared_ptr<const TradeRepresentation> tr1 = GenPDEParser::parseTradeRepresentation(s1);
    TEST( true );
}
