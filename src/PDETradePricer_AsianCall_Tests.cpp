#include <boost/shared_ptr.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "Framework.h"
#include "TestSuite.h"

#include "PDETradePricer.h"
#include "PDEPricingModelBlackScholes.h"
#include "BlackScholesClosedForms.h"
#include "GenPDEParser.h"

REGISTER_TEST_MANUAL(PDETradePricer_AsianCall_1)
{
    //boost::posix_time::ptime mst0 = boost::posix_time::microsec_clock::local_time();
    
    std::string file("../resources/AsianCall_TR.xml");
    boost::shared_ptr<const TradeRepresentation> tradeRepresentation(GenPDEParser::parseTradeRepresentation(file, true));
    
    boost::posix_time::ptime mst1 = boost::posix_time::microsec_clock::local_time();
    //std::cout << "Finished parsing trade representation (" << (mst1 - mst0).total_microseconds() << ")" << std::endl;
    
    GenPDE::Date d0(GenPDE::dateFromString("20020101"));
	const double spot(100.0);
	const double rate(0.05);
	const double volatility(0.25);
	const size_t nbSpaceNodes(500);
    const size_t nbPlanes(250);
	const double stdDevMultiple(5.0);
    const size_t nbRannacher(4);
    
    // For now, we hardcode the discretization of the AVs:
    AVDP_Hardcoded* avDisc(new AVDP_Hardcoded());
    for(size_t i=1; i<365; ++i)
    {
        double tenor      = ((double) i) / ((double) 365);
        double stdDev     = volatility * Double::sqrt(tenor);
        double width      = 2 * stdDevMultiple * stdDev;
        double lowerBound = Double::log(i * spot) - 0.5 * width;
        double ds         = width / ((double) (nbPlanes-1));
        std::vector<double> avValues(nbPlanes);
        for(size_t j=0; j<nbPlanes; ++j)
            avValues[j] = Double::exp(lowerBound + j * ds);
        avDisc->setAVDiscretizationValues(i, avValues);
    }
    
    BSModelParameters bsModelParams( spot, rate, volatility );
    PDEParameters1D   pdeParams1D( 3, nbRannacher, 0.3, nbSpaceNodes, stdDevMultiple );
    boost::shared_ptr<PDEPricingModelInterface> model(new PDEPricingModelBlackScholes(
        d0,
        bsModelParams,
        pdeParams1D,
        avDisc
    ));
   
    boost::shared_ptr<MOFixingsStore> moFixings( new MOFixingsStore() );
    boost::shared_ptr<PDETradePricer> pricer(new PDETradePricer(model, tradeRepresentation, moFixings));
    double price(pricer->price());
    boost::posix_time::ptime mst2 = boost::posix_time::microsec_clock::local_time();
    std::cout << "Finished pricing (" << (mst2 - mst1).total_microseconds() << ")" << std::endl;
    std::cout << "Price: " << price << std::endl;
    //double cf = BlackScholes::callPV(spot, rate, volatility / Double::sqrt(3), 1.0, 100);
}
