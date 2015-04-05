#include <vector>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "Framework.h"
#include "TestSuite.h"

#include "GpDate.h"
#include "AuxiliaryVariable.h"
#include "GenPDEParser.h"
#include "BlackScholesClosedForms.h"
#include "PDETradePricer.h"
#include "PDEPricingModelBlackScholes.h"

using std::vector;
using std::cerr;
using std::endl;

REGISTER_TEST(FwdStartingCall1)
{
    boost::posix_time::ptime mst1 = boost::posix_time::microsec_clock::local_time();
    
    /// The initial payout definition:
    std::stringstream ss1;
    ss1 << "<Payment date=\"20030101\">";
    ss1 <<     "<BinOp op=\"Max\">";
    ss1 <<         "<BinOp op=\"Sub\">";
    ss1 <<             "<BinOp op=\"Div\">";
    ss1 <<                 "<MORef uid=\"1\"/>";
    ss1 <<                 "<AVRef uid=\"1\"/>";
    ss1 <<             "</BinOp>";
    ss1 <<             "<Const value=\"1\"/>";
    ss1 <<         "</BinOp>";
    ss1 <<         "<Const value=\"0\"/>";
    ss1 <<     "</BinOp>";
    ss1 << "</Payment>";
    std::string s1(ss1.str());
    
    /// The auxiliary variable:
    std::string s2("<MORef uid=\"1\"/>");
    
    boost::shared_ptr<const TradeLeg> pePayout     = GenPDEParser::parseTradeLeg(s1);
    boost::shared_ptr<const PayoutExpression> peAV = GenPDEParser::parsePayoutExpression(s2);
    
	const double spot(100.0);
	const double rate(0.05);
	const double volatility(0.25);
	const size_t nbSpaceNodes(500);
	const double stdDevMultiple(5.0);
    const size_t nbPlanes(200);
    const size_t nbRannacher(4);
    
    GenPDE::Date d0 = GenPDE::dateFromString("20010101");
    GenPDE::Date d1 = GenPDE::dateFromString("20020101");
    GenPDE::Date d2 = GenPDE::dateFromString("20030101");
    const double tenor(GenPDE::dateDifferenceInDays(d2, d0) / GenPDE::NbDaysPerYear);
    const double fwdStart(GenPDE::dateDifferenceInDays(d1, d0) / GenPDE::NbDaysPerYear);
    
    const double lowerAVBoundary(Double::log(spot) + rate * fwdStart - stdDevMultiple * volatility * Double::sqrt(fwdStart));
    const double upperAVBoundary(Double::log(spot) + rate * fwdStart + stdDevMultiple * volatility * Double::sqrt(fwdStart));
    const double dsAV((upperAVBoundary - lowerAVBoundary) / static_cast<double>(nbPlanes - 1));
    vector<double> av1Values(nbPlanes);
    for(size_t i=0; i<nbPlanes; ++i)
        av1Values[i] = Double::exp(lowerAVBoundary + i * dsAV);
    
    std::vector<GenPDE::Date> dates = {d0, d1, d2};
    
    std::vector<boost::shared_ptr<const AuxiliaryVariable> > avs;
    boost::shared_ptr<AuxiliaryVariable> av1(new AuxiliaryVariable(1, d1, peAV));
    avs.push_back(av1);
    AVDP_Hardcoded* avDisc(new AVDP_Hardcoded());
    avDisc->setAVDiscretizationValues(1, av1Values);
    
    BSModelParameters bsModelParams(spot, rate, volatility);
    PDEParameters1D   pdeParams1D( 3, nbRannacher, 0.3, nbSpaceNodes, stdDevMultiple );
    boost::shared_ptr<PDEPricingModelInterface> model(new PDEPricingModelBlackScholes(
        d0,
        bsModelParams,
        pdeParams1D,
        avDisc
    ));
    boost::shared_ptr<const AuxiliaryVariables> avsPtr(new AuxiliaryVariables(avs));
    MOFixingsStore* moFixings( new MOFixingsStore() );
    model->setupForTrade( dates, *avsPtr, moFixings );
    boost::shared_ptr<TradeRepresentation> trade(new TradeRepresentation(
        boost::shared_ptr<DatedPricingInstructions>(),
        boost::shared_ptr<TradeLeg>(),
        avsPtr
    ));
    boost::shared_ptr<PDETradePricer> solverTradePricer(new PDETradePricer(model, trade, boost::shared_ptr<MOFixingsStore>(moFixings)));
    
    solverTradePricer->pricerInit(1, pePayout);
    solverTradePricer->timeStepToNextDate();
    solverTradePricer->removeAVDependency(1, 1);
    solverTradePricer->timeStepToNextDate();
    
    //TEST_EQ(model->getPricer(1)->getAVDependencies().getNbConfigurations(), 1);
    //TEST_EQ(model->getPricer(1)->getSVDependencies().getNbConfigurations(), 1);
    double v = *(model->getPricer(1)->getDataPtr());
    delete avDisc;
    boost::posix_time::ptime mst2 = boost::posix_time::microsec_clock::local_time();
    cerr << "Time: " << (mst2 - mst1).total_microseconds() << endl;
    double c = BlackScholes::callPV(spot, rate, volatility, tenor - fwdStart, spot);
    c *= Double::exp(-rate * fwdStart) / 100;
    TEST_EQ_DBL(v, c, 1e-4);
    cerr << "Value: " << v << endl;
    cerr << "Check: " << c << endl;
}
