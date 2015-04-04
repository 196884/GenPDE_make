#include "Framework.h"
#include "TestSuite.h"

#include "GenPDEParser.h"

REGISTER_TEST(TLParsing1)
{
    std::string s1("<PricerRef uid=\"123\"/>");
    boost::shared_ptr<const TradeLeg> tl1 = GenPDEParser::parseTradeLeg(s1);
    std::stringstream ss1;
    ss1 << *tl1;
    TEST_EQ(s1, ss1.str());
    
    std::string s2("<Payment date=\"20151225\"><Const value=\"1\"/></Payment>");
    boost::shared_ptr<const TradeLeg> tl2 = GenPDEParser::parseTradeLeg(s2);
    std::stringstream ss2;
    ss2 << *tl2;
    TEST_EQ(s2, ss2.str());
}
