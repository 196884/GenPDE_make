#include <iostream>
#include <fstream>

#include "Framework.h"
#include "TestSuite.h"

#include "GenPDEParser.h"

REGISTER_TEST(FileParsing_BinOp)
{
    std::string file("../resources/BinOp.xml");
    boost::shared_ptr<const PayoutExpression> pe(GenPDEParser::parsePayoutExpression(file, true));
    TEST(true);
}

REGISTER_TEST(FileParsing_CmpOp)
{
    std::string file("../resources/CmpOp.xml");
    boost::shared_ptr<const PayoutExpression> pe(GenPDEParser::parsePayoutExpression(file, true));
    TEST(true);
}

REGISTER_TEST(FileParsing_PECond)
{
    std::string file("../resources/PECond.xml");
    boost::shared_ptr<const PayoutExpression> pe(GenPDEParser::parsePayoutExpression(file, true));
    TEST(true);
}

REGISTER_TEST(FileParsing_Call_TR)
{
    std::string file("../resources/Call_TR.xml");
    boost::shared_ptr<const TradeRepresentation> pe(GenPDEParser::parseTradeRepresentation(file, true));
    TEST(true);
}

REGISTER_TEST(FileParsing_DigitalCall_TR)
{
    std::string file("../resources/DigitalCall_TR.xml");
    boost::shared_ptr<const TradeRepresentation> pe(GenPDEParser::parseTradeRepresentation(file, true));
    TEST(true);
}

REGISTER_TEST(FileParsing_FwdStartingCall_TR)
{
    std::string file("../resources/FwdStartingCall_TR.xml");
    boost::shared_ptr<const TradeRepresentation> pe(GenPDEParser::parseTradeRepresentation(file, true));
    TEST(true);
}
