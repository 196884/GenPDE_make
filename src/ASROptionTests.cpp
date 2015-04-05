#include <vector>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "Framework.h"
#include "TestSuite.h"

#include "GpDate.h"
#include "AuxiliaryVariable.h"
#include "GenPDEParser.h"
#include "PDEPricingModelBlackScholes.h"
#include "PDETradePricer.h"

using std::vector;
using std::cerr;
using std::endl;
using namespace GenPDE;

REGISTER_TEST_MANUAL(ASROption1)
{
    // Payout:
    // S_T / Max( A_T, 90 )
    
    //boost::posix_time::ptime mst0 = boost::posix_time::microsec_clock::local_time();
    
    std::string file("../resources/ASROption_01_trade.xml");
    boost::shared_ptr<const TradeRepresentation> tradeRepresentation(GenPDEParser::parseTradeRepresentation(file, true));
    
    boost::posix_time::ptime mst1 = boost::posix_time::microsec_clock::local_time();
    //std::cout << "Finished parsing trade representation (" << (mst1 - mst0).total_microseconds() << ")" << std::endl;
    
    GenPDE::Date d0(GenPDE::dateFromString("20020101"));
	const double spot(100.0);
	const double rate(0.05);
	const double volatility(0.25);
	const size_t nbSpaceNodes(500);
	const double stdDevMultiple(5.0);
    const size_t nbRannacher(4);

    const size_t nbPlanes(250);
    const double stdDevMultPlanes(4.0);
    
    // For now, we hardcode the discretization of the AVs:
    AVDP_Hardcoded* avDisc(new AVDP_Hardcoded());
    boost::shared_ptr<const AuxiliaryVariables> avs = tradeRepresentation->getAuxiliaryVariables();
    boost::shared_ptr<const AuxiliaryVariable>  av0 = avs->getAuxiliaryVariable(0);
    std::vector<double> avValues0(1, 100.0);
    GenPDE::Date        prevDate   = av0->getDate();
    double              totalTenor = 0;
    avDisc->setAVDiscretizationValues(0, avValues0);
    for(size_t i=1; i<126; ++i)
    {
        boost::shared_ptr<const AuxiliaryVariable> av = avs->getAuxiliaryVariable(i);
        GenPDE::Date thisDate = av->getDate();
        totalTenor           += GenPDE::dateDifferenceInDays(thisDate, prevDate) / 365.0;
        double stdDev         = volatility * Double::sqrt(totalTenor);
        double width          = 2 * stdDevMultPlanes * stdDev;
        double lowerBound     = Double::log((i + 1) * spot) - 0.5 * width;
        double ds             = width / ((double) (nbPlanes-1));
        std::vector<double> avValues(nbPlanes);
        for(size_t j=0; j<nbPlanes; ++j)
            avValues[j] = Double::exp(lowerBound + j * ds);
        avDisc->setAVDiscretizationValues(i, avValues);
        prevDate              = thisDate;
    }
    
    BSModelParameters bsModelParams( spot, rate, volatility);
    PDEParameters1D   pdeParams1D( 3, nbRannacher, 0.3, nbSpaceNodes, stdDevMultiple );
    boost::shared_ptr<PDEPricingModelInterface> model(new PDEPricingModelBlackScholes(
        d0,
        bsModelParams,
        pdeParams1D,
        avDisc
    ));
    
    boost::shared_ptr<const MOFixingsStore> moFixings( new MOFixingsStore() );
    boost::shared_ptr<PDETradePricer> pricer(new PDETradePricer( model, tradeRepresentation, moFixings ));
    double price(pricer->price());
    delete avDisc;
    boost::posix_time::ptime mst2 = boost::posix_time::microsec_clock::local_time();
    std::cout << "Finished pricing (" << (mst2 - mst1).total_microseconds() << ")" << std::endl;
    std::cout << "Price: " << price << std::endl;
    TEST_EQ_DBL( price, 0.976813, 1e-5 );
    //double cf = BlackScholes::callPV(spot, rate, volatility / Double::sqrt(3), 1.0, 100);
}

REGISTER_TEST_MANUAL(ASROption2)
{
    // Payout:
    // S_T / Max( A_T, 90 )
    
    //boost::posix_time::ptime mst0 = boost::posix_time::microsec_clock::local_time();
    
    std::string tradeFile("../resources/ASROption_01_trade.xml");
    boost::shared_ptr<const TradeRepresentation> tradeRepresentation(GenPDEParser::parseTradeRepresentation(tradeFile, true));

    std::string modelFile( "../resources/ASROption_01_model.xml" );
    boost::shared_ptr<PDEPricingModelInterface> model(GenPDEParser::parsePDEModel(modelFile, true));
    
    boost::posix_time::ptime mst1 = boost::posix_time::microsec_clock::local_time();

    boost::shared_ptr<const MOFixingsStore> moFixings( new MOFixingsStore() );
    boost::shared_ptr<PDETradePricer> pricer(new PDETradePricer( model, tradeRepresentation, moFixings ));
    double price(pricer->price());
    boost::posix_time::ptime mst2 = boost::posix_time::microsec_clock::local_time();
    std::cout << "Finished pricing (" << (mst2 - mst1).total_microseconds() << ")" << std::endl;
    std::cout << "Price: " << price << std::endl;
    TEST_EQ_DBL( price, 0.976813, 1e-5 );
    //double cf = BlackScholes::callPV(spot, rate, volatility / Double::sqrt(3), 1.0, 100);
}
