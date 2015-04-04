#include "Framework.h"
#include "TestSuite.h"

#include "PayoutExpression.h"
#include "GenPDEParser.h"

REGISTER_TEST(PayoutExpressionParsing)
{
    std::string s1("<Const value=\"3.14\"/>");
    std::string s2("<AVRef uid=\"333\"/>");
    std::string s3("<MORef uid=\"1\"/>");
    std::string s4("<BinOp op=\"Add\"><Const value=\"3\"/><AVRef uid=\"1\"/></BinOp>");
    std::string s5("<BinOp op=\"Sub\"><Const value=\"3\"/><AVRef uid=\"1\"/></BinOp>");
    std::string s6("<BinOp op=\"Mul\"><Const value=\"3\"/><AVRef uid=\"1\"/></BinOp>");
    std::string s7("<BinOp op=\"Div\"><Const value=\"3\"/><AVRef uid=\"1\"/></BinOp>");
    std::string s8("<BinOp op=\"Max\"><Const value=\"3\"/><AVRef uid=\"1\"/></BinOp>");
    std::string s9("<BinOp op=\"Min\"><Const value=\"3\"/><AVRef uid=\"1\"/></BinOp>");
    std::string s10("<UnOp op=\"Neg\"><MORef uid=\"1\"/></UnOp>");
    std::string s11("<UnOp op=\"Id\"><MORef uid=\"1\"/></UnOp>");
    std::vector<std::string> testCases = {s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11};
    
    for(const std::string& s : testCases)
    {
        boost::shared_ptr<const PayoutExpression> pe = GenPDEParser::parsePayoutExpression(s);
        std::stringstream ss;
        ss << *pe;
        //std::cerr << "***************" << std::endl;
        //std::cerr << s << std::endl;
        //std::cerr << ss.str() << std::endl;
        TEST_EQ(s, ss.str());
    }
}
