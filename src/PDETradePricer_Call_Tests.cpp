//
//  PDETradePricer_Call_Tests.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/31/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include <boost/shared_ptr.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "Framework.h"
#include "TestSuite.h"

#include "PDETradePricer.h"
#include "PDEPricingModelBlackScholes.h"
#include "GenPDEParser.h"

REGISTER_TEST(PDETradePricer_Call_1)
{
    //boost::posix_time::ptime mst0 = boost::posix_time::microsec_clock::local_time();
    
    std::string file("../resources/Call_TR.xml");
    boost::shared_ptr<const TradeRepresentation> tradeRepresentation(GenPDEParser::parseTradeRepresentation(file, true));
    
    //boost::posix_time::ptime mst1 = boost::posix_time::microsec_clock::local_time();
    //std::cout << "Finished parsing trade representation (" << (mst1 - mst0).total_microseconds() << ")" << std::endl;
    
    GenPDE::Date d0(GenPDE::dateFromString("20010101"));
	const double spot(100.0);
	const double rate(0.05);
	const double volatility(0.25);
	const size_t nbSpaceNodes(500);
	const double stdDevMultiple(5.0);
    const size_t nbRannacher(4);
    
    boost::shared_ptr<PDEPricingModelInterface> model(new PDEPricingModelBlackScholes(
        d0,
        spot,
        rate,
        volatility,
        3,
        nbRannacher,
        0.3,
        nbSpaceNodes,
        stdDevMultiple
    ));
    
    boost::shared_ptr<PDETradePricer> pricer(new PDETradePricer(model, tradeRepresentation));
    double price(pricer->price());
    //boost::posix_time::ptime mst2 = boost::posix_time::microsec_clock::local_time();
    //std::cout << "Finished pricing (" << (mst2 - mst1).total_microseconds() << ")" << std::endl;
    //std::cout << "Price: " << price << std::endl;
    TEST_EQ_DBL(price, 12.3355, 1e-4);
}

REGISTER_TEST(PDETradePricer_Call_2)
{
    // We price on the only instruction date...
    //boost::posix_time::ptime mst0 = boost::posix_time::microsec_clock::local_time();
    
    std::string file("../resources/Call_TR.xml");
    boost::shared_ptr<const TradeRepresentation> tradeRepresentation(GenPDEParser::parseTradeRepresentation(file, true));
    
    //boost::posix_time::ptime mst1 = boost::posix_time::microsec_clock::local_time();
    //std::cout << "Finished parsing trade representation (" << (mst1 - mst0).total_microseconds() << ")" << std::endl;
    
    GenPDE::Date d0(GenPDE::dateFromString("20020101"));
	const double spot(100.123);
	const double rate(0.05);
	const double volatility(0.25);
	const size_t nbSpaceNodes(500);
	const double stdDevMultiple(5.0);
    const size_t nbRannacher(4);
    
    boost::shared_ptr<PDEPricingModelInterface> model(new PDEPricingModelBlackScholes(
        d0,
        spot,
        rate,
        volatility,
        3,
        nbRannacher,
        0.3,
        nbSpaceNodes,
        stdDevMultiple
    ));
    
    boost::shared_ptr<PDETradePricer> pricer(new PDETradePricer(model, tradeRepresentation));
    double price(pricer->price());
    //boost::posix_time::ptime mst2 = boost::posix_time::microsec_clock::local_time();
    //std::cout << "Finished pricing (" << (mst2 - mst1).total_microseconds() << ")" << std::endl;
    //std::cout << "Price: " << price << std::endl;
    TEST_EQ_DBL(price, 0.123, 1e-4);
}

REGISTER_TEST(PDETradePricer_Call_3)
{
    //boost::posix_time::ptime mst0 = boost::posix_time::microsec_clock::local_time();
    
    std::string tradeFile("../resources/Call_TR.xml");
    boost::shared_ptr<const TradeRepresentation> tradeRepresentation(GenPDEParser::parseTradeRepresentation(tradeFile, true));
    std::string modelFile("../resources/PDE_BS_Model_1.xml");
    boost::shared_ptr<PDEPricingModelInterface> model(GenPDEParser::parsePDEModel(modelFile, true));

    //boost::posix_time::ptime mst1 = boost::posix_time::microsec_clock::local_time();
    //std::cout << "Finished parsing trade representation (" << (mst1 - mst0).total_microseconds() << ")" << std::endl;
   
    boost::shared_ptr<PDETradePricer> pricer(new PDETradePricer(model, tradeRepresentation));
    double price(pricer->price());
    //boost::posix_time::ptime mst2 = boost::posix_time::microsec_clock::local_time();
    //std::cout << "Finished pricing (" << (mst2 - mst1).total_microseconds() << ")" << std::endl;
    //std::cout << "Price: " << price << std::endl;
    TEST_EQ_DBL(price, 12.3355, 1e-4);
}

REGISTER_TEST(PDETradePricer_ZC_1)
{
    //boost::posix_time::ptime mst0 = boost::posix_time::microsec_clock::local_time();
    
    std::string file("../resources/ZC_TR.xml");
    boost::shared_ptr<const TradeRepresentation> tradeRepresentation(GenPDEParser::parseTradeRepresentation(file, true));
    
    //boost::posix_time::ptime mst1 = boost::posix_time::microsec_clock::local_time();
    //std::cout << "Finished parsing trade representation (" << (mst1 - mst0).total_microseconds() << ")" << std::endl;
    
    GenPDE::Date d0(GenPDE::dateFromString("20010101"));
	const double spot(100.0);
	const double rate(0.05);
	const double volatility(0.25);
	const size_t nbSpaceNodes(500);
	const double stdDevMultiple(5.0);
    const size_t nbRannacher(4);
    
    boost::shared_ptr<PDEPricingModelInterface> model(new PDEPricingModelBlackScholes(
        d0,
        spot,
        rate,
        volatility,
        3,
        nbRannacher,
        0.3,
        nbSpaceNodes,
        stdDevMultiple
    ));
    
    boost::shared_ptr<PDETradePricer> pricer(new PDETradePricer(model, tradeRepresentation));
    double price(pricer->price());
    //boost::posix_time::ptime mst2 = boost::posix_time::microsec_clock::local_time();
    //std::cout << "Finished pricing (" << (mst2 - mst1).total_microseconds() << ")" << std::endl;
    //std::cout << "Price: " << price << std::endl;
    double check(Double::exp(-rate));
    TEST_EQ_DBL(price, check, 1e-8);
}
