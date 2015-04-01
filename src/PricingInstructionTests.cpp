//
//  PricingInstructionTests.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/26/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "Framework.h"
#include "TestSuite.h"

#include "PricingInstruction.h"
#include "GenPDEParser.h"

REGISTER_TEST(PricingInstructionParsing)
{
    std::string s1("<PricerInit uid=\"33\"><Payment date=\"20140103\"><Const value=\"1\"/></Payment></PricerInit>");
    std::string s2("<PricerAdd uid=\"3\"><PricerRef uid=\"1\"/><PricerRef uid=\"2\"/></PricerAdd>");
    std::string s3("<PricerMult uid=\"3\"><Const value=\"2\"/><PricerRef uid=\"2\"/></PricerMult>");
    std::string s4("<PricerCond uid=\"3\"><Const value=\"1\"/><PricerRef uid=\"2\"/><PricerRef uid=\"3\"/></PricerCond>");
    std::string s5("<PricerChoice uid=\"3\" chooser=\"Client\" choiceID=\"Early exercise?\" default=\"Leg1\"><PricerRef uid=\"2\"/><PricerRef uid=\"3\"/></PricerChoice>");
    std::string s6("<PricerRm uid=\"42\"/>");
    std::string s7("<PricerRmAVDeps uid=\"7\" AVs=\"2,3,5,7,11\"/>");
    std::string s8("<PricerRmAVDeps uid=\"7\" AVs=\"13\"/>");
    std::vector<std::string> testCases = {s1, s2, s3, s4, s5, s6, s7, s8};
    
    for(const std::string& s : testCases)
    {
        boost::shared_ptr<const PricingInstruction> pi = GenPDEParser::parsePricingInstruction(s);
        std::stringstream ss;
        ss << *pi;
        //std::cerr << "***************" << std::endl;
        //std::cerr << s << std::endl;
        //std::cerr << ss.str() << std::endl;
        TEST_EQ(s, ss.str());
    }
}