#include <boost/shared_ptr.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "Framework.h"
#include "TestSuite.h"

#include "PDETradePricer.h"
#include "PDEPricingModelBlackScholes.h"
#include "BlackScholesClosedForms.h"
#include "GenPDEParser.h"

REGISTER_TEST(PDETradePricer_DigitalCall_1)
{
    //boost::posix_time::ptime mst0 = boost::posix_time::microsec_clock::local_time();
    
    std::string file("../resources/DigitalCall_TR.xml");
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
    
    AVDP_None* avDisc = new AVDP_None();
    BSModelParameters bsModelParams( spot, rate, volatility );
    boost::shared_ptr<PDEPricingModelInterface> model(new PDEPricingModelBlackScholes(
        d0,
        bsModelParams,
        3,
        nbRannacher,
        0.3,
        nbSpaceNodes,
        stdDevMultiple,
        avDisc
    ));

    boost::shared_ptr<MOFixingsStore> moFixings( new MOFixingsStore() );
    boost::shared_ptr<PDETradePricer> pricer(new PDETradePricer(model, tradeRepresentation, moFixings ));
    double price(pricer->price());
    double cf = BlackScholes::digitalCallPV(spot, rate, volatility, 1.0, spot);
    //boost::posix_time::ptime mst2 = boost::posix_time::microsec_clock::local_time();
    //std::cout << "Finished pricing (" << (mst2 - mst1).total_microseconds() << ")" << std::endl;
    //std::cout << "Price: " << price << std::endl;
    TEST_EQ_DBL(price, cf, 1e-4);
}
