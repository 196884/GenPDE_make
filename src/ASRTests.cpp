//
//  ASRTests.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/25/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

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

REGISTER_TEST(ASR1)
{
    // Payout:
    // S_T * (1 / S_0 - 1 / A_T)
    // where A_T is the (daily) average
    
    boost::posix_time::ptime mst1 = boost::posix_time::microsec_clock::local_time();
    
	const double spot(100.0);
	const double rate(0.01);
	const double volatility(0.25);
	const size_t nbSpaceNodes(500);
	const double stdDevMultiple(5.0);
    const size_t nbRannacher(4);
    const double avStdDevMultiple(4);
    const size_t nbPlanes(200);
    
    const double logSpot(Double::log(spot));
    
    Date d0 = dateFromString("20010101");
    vector<Date> dates(366);
    for(long i=0; i<366; ++i)
        dates[i] = dateAddDays(d0, i);
    
    boost::shared_ptr<AVContext> avc(new AVContext());;
    std::string sAV1("<MORef uid=\"1\"/>");
    boost::shared_ptr<const PayoutExpression> av1Def = GenPDEParser::parsePayoutExpression(sAV1);
    boost::shared_ptr<AuxiliaryVariable> av1(new AuxiliaryVariable(1, dates[1], av1Def));
    vector<double> av1Values(nbPlanes);
    double stdDev1  = volatility * Double::sqrt(dateDifferenceInDays(dates[1], dates[0]) / NbDaysPerYear);
    double av1Lower = logSpot - avStdDevMultiple * stdDev1;
    double av1Step  = 2 * avStdDevMultiple * stdDev1 / (nbPlanes - 1);
    for(size_t i=0; i<nbPlanes; ++i)
        av1Values[i] = Double::exp(av1Lower + i * av1Step);
    av1->setDiscretizationValues(av1Values);
    avc->setAuxiliaryVariable(av1);
    for(size_t i=2; i<dates.size()-1; ++i)
    {
        std::stringstream ss;
        ss << "<BinOp op=\"Add\"><MORef uid=\"1\"/><AVRef uid=\"";
        ss << i-1;
        ss << "\"/></BinOp>";
        std::string sAVi(ss.str());
        boost::shared_ptr<const PayoutExpression> aviDef = GenPDEParser::parsePayoutExpression(sAVi);
        boost::shared_ptr<AuxiliaryVariable> avi(new AuxiliaryVariable((unsigned int) i, dates[i], aviDef));
        vector<double> aviValues(nbPlanes);
        double stdDevi  = volatility * Double::sqrt(dateDifferenceInDays(dates[i], dates[0]) / NbDaysPerYear);
        double aviLower = logSpot - avStdDevMultiple * stdDevi;
        double aviStep = 2 * avStdDevMultiple * stdDevi / (nbPlanes - 1);
        for(size_t j=0; j<nbPlanes; ++j)
            aviValues[j] = ((double) i) * Double::exp(aviLower + j * aviStep);
        avi->setDiscretizationValues(aviValues);
        avc->setAuxiliaryVariable(avi);
    }
    
    /// The final payout
    std::stringstream ssF;
    ssF << "<Payment date=\"20020101\">";
    ssF <<     "<BinOp op=\"Mul\">";
    ssF <<         "<MORef uid=\"1\"/>";
    ssF <<         "<BinOp op=\"Sub\">";
    ssF <<             "<Const value=\"0.01\"/>";
    ssF <<             "<BinOp op=\"Div\">";
    ssF <<                 "<Const value=\"" << dates.size() - 1 << "\"/>";
    ssF <<                 "<BinOp op=\"Add\">";
    ssF <<                     "<MORef uid=\"1\"/>";
    ssF <<                     "<AVRef uid=\"" << dates.size() - 2 << "\"/>";
    ssF <<                 "</BinOp>";
    ssF <<             "</BinOp>";
    ssF <<         "</BinOp>";
    ssF <<     "</BinOp>";
    ssF << "</Payment>";
    std::string sF(ssF.str());
    
    boost::shared_ptr<const TradeLeg> payoutLeg = GenPDEParser::parseTradeLeg(sF);

    double tenor = dateDifferenceInDays(dates.back(), dates[0]) / NbDaysPerYear;
	const double variance(volatility * volatility * tenor);
	const double stdDev(Double::sqrt(variance));
	const double lowerBoundary(Double::log(spot) - stdDevMultiple * stdDev);
	const double upperBoundary(Double::log(spot) + stdDevMultiple * stdDev);
	const double ds((upperBoundary - lowerBoundary) / static_cast<double>(nbSpaceNodes - 1));
    vector<double> logSpotGrid(nbSpaceNodes);
    vector<double> spotGrid(nbSpaceNodes);
    for (size_t i=0; i<nbSpaceNodes; ++i)
    {
    	logSpotGrid[i] = lowerBoundary + i * ds;
    	spotGrid[i]    = Double::exp(logSpotGrid[i]);
    }
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
    model->setupForTrade(dates, avc, TradeFixings::NoFixings);
    boost::shared_ptr<PDETradePricer> solverTradePricer(new PDETradePricer(model));
    
    avc->setAuxiliaryVariable(av1);
    solverTradePricer->pricerInit(1, payoutLeg);
    
    for(size_t i=dates.size()-2; i>0; --i)
    {
        solverTradePricer->timeStepToNextDate();
        solverTradePricer->removeAVDependency(1, (GenPDE::VariableUID) i);
    }
    solverTradePricer->timeStepToNextDate();
    double v = *(model->getPricer(1)->getDataPtr());
    boost::posix_time::ptime mst2 = boost::posix_time::microsec_clock::local_time();
    
    cerr << "Time: " << (mst2 - mst1).total_microseconds() << endl;
    cerr << "Value: " << v << endl;
    TEST_EQ_DBL(v, 0.016, 1e-3); // low precision, b/c we don't want to spend ages here!
}


REGISTER_TEST(ASR2)
{
    // Payout:
    // S_T * (1 / S_0 - 1 / A_T)
    // where A_T is the (daily) average
    // We can early redeem anytime!
    
    boost::posix_time::ptime mst1 = boost::posix_time::microsec_clock::local_time();
    
	const double spot(100.0);
	const double rate(0.01);
	const double volatility(0.25);
	const size_t nbSpaceNodes(500);
	const double stdDevMultiple(5.0);
    const size_t nbRannacher(4);
    const double avStdDevMultiple(4.0);
    const size_t nbPlanes(200);
    const size_t nbDaysTotal(181);
    const size_t nbDaysERStart(91);
    
    const double logSpot(Double::log(spot));
    
    Date d0 = dateFromString("20010101");
    vector<Date> dates(nbDaysTotal + 1);
    for(long i=0; i<=nbDaysTotal; ++i)
        dates[i] = dateAddDays(d0, i);
    Date firstERDate(GenPDE::dateAddDays(d0, nbDaysERStart));
    
    boost::shared_ptr<AVContext> avc(new AVContext());;
    std::string sAV1("<MORef uid=\"1\"/>");
    boost::shared_ptr<const PayoutExpression> av1Def = GenPDEParser::parsePayoutExpression(sAV1);
    boost::shared_ptr<AuxiliaryVariable> av1(new AuxiliaryVariable(1, dates[1], av1Def));
    vector<double> av1Values(nbPlanes);
    double stdDev1  = volatility * Double::sqrt(dateDifferenceInDays(dates[1], dates[0]) / NbDaysPerYear);
    double av1Lower = logSpot - avStdDevMultiple * stdDev1;
    double av1Step = 2 * avStdDevMultiple * stdDev1 / (nbPlanes - 1);
    for(size_t i=0; i<nbPlanes; ++i)
        av1Values[i] = Double::exp(av1Lower + i * av1Step);
    av1->setDiscretizationValues(av1Values);
    avc->setAuxiliaryVariable(av1);
    for(size_t i=2; i<dates.size()-1; ++i)
    {
        std::stringstream ss;
        ss << "<BinOp op=\"Add\"><MORef uid=\"1\"/><AVRef uid=\"";
        ss << i-1;
        ss << "\"/></BinOp>";
        std::string sAVi(ss.str());
        boost::shared_ptr<const PayoutExpression> aviDef = GenPDEParser::parsePayoutExpression(sAVi);
        boost::shared_ptr<AuxiliaryVariable> avi(new AuxiliaryVariable((unsigned int) i, dates[i], aviDef));
        vector<double> aviValues(nbPlanes);
        double stdDevi  = volatility * Double::sqrt(dateDifferenceInDays(dates[i], dates[0]) / NbDaysPerYear);
        double aviLower = logSpot - avStdDevMultiple * stdDevi;
        double aviStep = 2 * avStdDevMultiple * stdDevi / (nbPlanes - 1);
        for(size_t j=0; j<nbPlanes; ++j)
            aviValues[j] = ((double) i) * Double::exp(aviLower + j * aviStep);
        avi->setDiscretizationValues(aviValues);
        avc->setAuxiliaryVariable(avi);
    }
    
    /// The final payout
    std::stringstream ssF;
    ssF << "<Payment date=\"" << GenPDE::dateToString(dates.back()) << "\">";
    ssF <<     "<BinOp op=\"Mul\">";
    ssF <<         "<MORef uid=\"1\"/>";
    ssF <<         "<BinOp op=\"Sub\">";
    ssF <<             "<Const value=\"0.01\"/>";
    ssF <<             "<BinOp op=\"Div\">";
    ssF <<                 "<Const value=\"" << dates.size() - 1 << "\"/>";
    ssF <<                 "<BinOp op=\"Add\">";
    ssF <<                     "<MORef uid=\"1\"/>";
    ssF <<                     "<AVRef uid=\"" << dates.size() - 2 << "\"/>";
    ssF <<                 "</BinOp>";
    ssF <<             "</BinOp>";
    ssF <<         "</BinOp>";
    ssF <<     "</BinOp>";
    ssF << "</Payment>";
    std::string sF(ssF.str());
    boost::shared_ptr<const TradeLeg> payoutLeg = GenPDEParser::parseTradeLeg(sF);
    // this will be on pricer #365.
    
    std::map<size_t, boost::shared_ptr<const TradeLeg> > erLegsByDateIndex;
    for(size_t i=2; i<nbDaysTotal; ++i)
    {
        std::stringstream ssER;
        ssER << "<Payment date=\"" << GenPDE::dateToString(dates[i]) << "\">";
        ssER <<     "<BinOp op=\"Mul\">";
        ssER <<         "<MORef uid=\"1\"/>";
        ssER <<         "<BinOp op=\"Sub\">";
        ssER <<             "<Const value=\"0.01\"/>";
        ssER <<             "<BinOp op=\"Div\">";
        ssER <<                 "<Const value=\"" << i << "\"/>";
        ssER <<                 "<BinOp op=\"Add\">";
        ssER <<                     "<MORef uid=\"1\"/>";
        ssER <<                     "<AVRef uid=\"" << i - 1 << "\"/>";
        ssER <<                 "</BinOp>";
        ssER <<             "</BinOp>";
        ssER <<         "</BinOp>";
        ssER <<     "</BinOp>";
        ssER << "</Payment>";
        std::string sER(ssER.str());
        boost::shared_ptr<const TradeLeg> erLeg(GenPDEParser::parseTradeLeg(sER));
        erLegsByDateIndex[i] = erLeg;
    }
    std::stringstream ssER1;
    ssER1 << "<Payment date=\"" << GenPDE::dateToString(dates[1]) << "\">";
    ssER1 <<     "<BinOp op=\"Mul\">";
    ssER1 <<         "<MORef uid=\"1\"/>";
    ssER1 <<         "<BinOp op=\"Sub\">";
    ssER1 <<             "<Const value=\"0.01\"/>";
    ssER1 <<             "<BinOp op=\"Div\">";
    ssER1 <<                 "<Const value=\"2\"/>";
    ssER1 <<                 "<BinOp op=\"Add\">";
    ssER1 <<                     "<MORef uid=\"1\"/>";
    ssER1 <<                     "<Const value=\"100\"/>";
    ssER1 <<                 "</BinOp>";
    ssER1 <<             "</BinOp>";
    ssER1 <<         "</BinOp>";
    ssER1 <<     "</BinOp>";
    ssER1 << "</Payment>";
    std::string sER1(ssER1.str());
    boost::shared_ptr<const TradeLeg> erLeg1(GenPDEParser::parseTradeLeg(sER1));
    erLegsByDateIndex[1] = erLeg1;

    double tenor = dateDifferenceInDays(dates.back(), dates[0]) / NbDaysPerYear;
	const double variance(volatility * volatility * tenor);
	const double stdDev(Double::sqrt(variance));
	const double lowerBoundary(Double::log(spot) - stdDevMultiple * stdDev);
	const double upperBoundary(Double::log(spot) + stdDevMultiple * stdDev);
	const double ds((upperBoundary - lowerBoundary) / static_cast<double>(nbSpaceNodes - 1));
    vector<double> logSpotGrid(nbSpaceNodes);
    vector<double> spotGrid(nbSpaceNodes);
    for (size_t i=0; i<nbSpaceNodes; ++i)
    {
    	logSpotGrid[i] = lowerBoundary + i * ds;
    	spotGrid[i]    = Double::exp(logSpotGrid[i]);
    }
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
    model->setupForTrade(dates, avc);
    boost::shared_ptr<PDETradePricer> solverTradePricer(new PDETradePricer(model));
    
    avc->setAuxiliaryVariable(av1);
    solverTradePricer->pricerInit(nbDaysTotal, payoutLeg);
    size_t currentPricer(nbDaysTotal);
    
    for(size_t i=dates.size()-2; i>0; --i)
    {
        solverTradePricer->timeStepToNextDate();
        solverTradePricer->removeAVDependency(currentPricer, (GenPDE::VariableUID) i);
        if( dates[i] >= firstERDate )
        {
            boost::shared_ptr<TradeLeg> pricerLeg(new TLPricerRef(i+1));
            solverTradePricer->pricerChoice(currentPricer-1, Choice::Chooser_Client, pricerLeg, erLegsByDateIndex[i]);
            solverTradePricer->pricerRemove(currentPricer);
            --currentPricer;
        };
    }
    solverTradePricer->timeStepToNextDate();
    double v = *(model->getPricer(currentPricer)->getDataPtr());
    boost::posix_time::ptime mst2 = boost::posix_time::microsec_clock::local_time();
    
    cerr << "Time: " << (mst2 - mst1).total_microseconds() << endl;
    cerr << "Value: " << v << endl;
    TEST_EQ_DBL(v, 0.024657, 1e-3); // low precision, b/c we don't want to spend ages here!
}
