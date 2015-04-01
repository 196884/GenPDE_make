/*
 * PDECallPricingTests.h
 *
 *  Created on: Nov 1, 2009
 *      Author: dupontr
 */

#include "Framework.h"
#include "TestSuite.h"

#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "PDESolver1D.h"
#include "BlackScholesPDEUpdater.h"
#include "BlackScholesClosedForms.h"
#include "CubicSplineInterpolator.h"

using namespace std;

REGISTER_TEST(PDEForwardPricing1)
{
	const double spot(100.0);
	const double rate(0.05);
	const double volatility(0.25);
	const double tenor(1.0);
	const size_t nbTimeNodes(100);
	const size_t nbSpaceNodes(500);
	const double stdDevMultiple(5.0);

	const double variance(volatility * volatility * tenor);
	const double stdDev(Double::sqrt(variance));
	const double lowerBoundary(Double::log(spot) - 0.5 * variance - stdDevMultiple * stdDev);
	const double upperBoundary(Double::log(spot) - 0.5 * variance + stdDevMultiple * stdDev);
	const double ds((upperBoundary - lowerBoundary) / static_cast<double>(nbSpaceNodes - 1));
    vector<double> logSpotGrid(nbSpaceNodes);
    vector<double> spotGrid(nbSpaceNodes);
    for (size_t i=0; i<nbSpaceNodes; ++i)
    {
    	logSpotGrid[i] = lowerBoundary + i * ds;
    	spotGrid[i]    = Double::exp(logSpotGrid[i]);
    }
    vector<double> timeGrid(nbTimeNodes);
    const double dt(tenor / static_cast<double>(nbTimeNodes - 1));
    for (size_t i=0; i<nbTimeNodes; ++i)
    	timeGrid[i] = i * dt;
    boost::shared_ptr<PDEUpdater1D> updater(new BlackScholesPDEUpdater(rate, volatility));
    PDESolver1D solver(logSpotGrid, timeGrid, updater.get());
    const size_t solUid(solver.addSolutionSet());
    double* solStart(solver.getSolutionSetPtr(solUid));
    std::copy(solStart, solStart + nbSpaceNodes, spotGrid.begin());
    for (size_t i=1; i<nbTimeNodes; ++i)
    	solver.timeStepOnce();
    const double* finalSolStart = solver.getSolutionSetPtr(solUid);
    for (size_t i=0; i<nbSpaceNodes; ++i)
    	TEST_EQ_DBL(finalSolStart[i], spotGrid[i], 1e-6);
}

REGISTER_TEST(PDEForwardPricing2)
{
	const double spot(100.0);
	const double rate(0.05);
	const double volatility(0.25);
	const double tenor(1.0);
	const size_t nbTimeNodes(100);
	const size_t nbSpaceNodes(500);
	const double stdDevMultiple(5.0);

	const double variance(volatility * volatility * tenor);
	const double stdDev(Double::sqrt(variance));
	const double lowerBoundary(Double::log(spot) - 0.5 * variance - stdDevMultiple * stdDev);
	const double upperBoundary(Double::log(spot) - 0.5 * variance + stdDevMultiple * stdDev);
	const double ds((upperBoundary - lowerBoundary) / static_cast<double>(nbSpaceNodes - 1));
    vector<double> logSpotGrid(nbSpaceNodes);
    vector<double> spotGrid(nbSpaceNodes);
    for (size_t i=0; i<nbSpaceNodes; ++i)
    {
    	logSpotGrid[i] = lowerBoundary + i * ds;
    	spotGrid[i]    = Double::exp(logSpotGrid[i]);
    }
    vector<double> timeGrid(nbTimeNodes);
    const double dt(tenor / static_cast<double>(nbTimeNodes - 1));
    for (size_t i=0; i<nbTimeNodes; ++i)
    	timeGrid[i] = i * dt;
    boost::shared_ptr<PDEUpdater1D> updater(new BlackScholesPDEUpdater(rate, volatility));
    PDESolver1D solver(logSpotGrid, timeGrid, updater.get(), 1.0);
    
    const size_t solUid(solver.addSolutionSet());
    double* solStart(solver.getSolutionSetPtr(solUid));
    std::copy(solStart, solStart + nbSpaceNodes, spotGrid.begin());
    for (size_t i=1; i<nbTimeNodes; ++i)
    	solver.timeStepOnce();
    const double* finalSolStart = solver.getSolutionSetPtr(solUid);
    for (size_t i=0; i<nbSpaceNodes; ++i)
    	TEST_EQ_DBL(finalSolStart[i], spotGrid[i], 1e-6);
}

REGISTER_TEST(BlackScholesCFCall)
{
	const double pv(BlackScholes::callPV(1.0, 0.05, 0.25, 1.0, 1.0));
	TEST_EQ_DBL(pv, 0.123360, 1e-6);
}

REGISTER_TEST(PDECallPricing1)
{
	const double spot(1.0);
	const double rate(0.05);
	const double volatility(0.25);
	const double tenor(1.0);
	const double strike(Double::exp(-1.0 / 400.0));
	const size_t nbTimeNodes(101);
	const size_t nbSpaceNodes(501);
	const double stdDevMultiple(5.0);

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
    vector<double> timeGrid(nbTimeNodes);
    const double dt(tenor / static_cast<double>(nbTimeNodes - 1));
    for (size_t i=0; i<nbTimeNodes; ++i)
    	timeGrid[i] = i * dt;
    boost::shared_ptr<PDEUpdater1D> updater(new BlackScholesPDEUpdater(rate, volatility));
    PDESolver1D solver(logSpotGrid, timeGrid, updater.get());
    
    const size_t solUid(solver.addSolutionSet());
    double* solStart(solver.getSolutionSetPtr(solUid));
    for (size_t i=0; i<nbSpaceNodes; ++i)
    	solStart[i] = Double::max(spotGrid[i] - strike, 0.0);
    for (size_t i=1; i<nbTimeNodes; ++i)
    	solver.timeStepOnce();
    const double* finalSolStart = solver.getSolutionSetPtr(solUid);
    const double pdePv(finalSolStart[nbSpaceNodes / 2]);
    const double cfPv(BlackScholes::callPV(spotGrid[nbSpaceNodes / 2], rate, volatility, tenor, strike));
    TEST_EQ_DBL(pdePv, cfPv, 1e-6);
}
