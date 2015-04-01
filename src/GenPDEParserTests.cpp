//
//  GenPDEParserTests.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/31/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include <iostream>
#include <fstream>

#include "Framework.h"
#include "TestSuite.h"

#include "GenPDEParser.h"

REGISTER_TEST(FileParsing_BinOp)
{
    std::string file("/Users/regisdupont/Documents/Code/GenPDE/GenPDE/Resources/BinOp.xml");
    boost::shared_ptr<const PayoutExpression> pe(GenPDEParser::parsePayoutExpression(file, true));
    TEST(true);
}

REGISTER_TEST(FileParsing_CmpOp)
{
    std::string file("/Users/regisdupont/Documents/Code/GenPDE/GenPDE/Resources/CmpOp.xml");
    boost::shared_ptr<const PayoutExpression> pe(GenPDEParser::parsePayoutExpression(file, true));
    TEST(true);
}

REGISTER_TEST(FileParsing_PECond)
{
    std::string file("/Users/regisdupont/Documents/Code/GenPDE/GenPDE/Resources/PECond.xml");
    boost::shared_ptr<const PayoutExpression> pe(GenPDEParser::parsePayoutExpression(file, true));
    TEST(true);
}

REGISTER_TEST(FileParsing_Call_TR)
{
    std::string file("/Users/regisdupont/Documents/Code/GenPDE/GenPDE/Resources/Call_TR.xml");
    boost::shared_ptr<const TradeRepresentation> pe(GenPDEParser::parseTradeRepresentation(file, true));
    TEST(true);
}

REGISTER_TEST(FileParsing_DigitalCall_TR)
{
    std::string file("/Users/regisdupont/Documents/Code/GenPDE/GenPDE/Resources/DigitalCall_TR.xml");
    boost::shared_ptr<const TradeRepresentation> pe(GenPDEParser::parseTradeRepresentation(file, true));
    TEST(true);
}

REGISTER_TEST(FileParsing_FwdStartingCall_TR)
{
    std::string file("/Users/regisdupont/Documents/Code/GenPDE/GenPDE/Resources/FwdStartingCall_TR.xml");
    boost::shared_ptr<const TradeRepresentation> pe(GenPDEParser::parseTradeRepresentation(file, true));
    TEST(true);
}
