#include "Framework.h"
#include "TestSuite.h"

#include "MarkovFunctionalDistribution.h"

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_01.hpp>
#include <boost/random/variate_generator.hpp>


#include <iostream>
using namespace std;

REGISTER_TEST(MarkovFunctionalFlatVol1)
{
	double volatility(0.25);
	double rate(0.05);

	boost::shared_ptr<VolatilityParametrization> volParam(new VolatilityQuadraticForm(volatility, 0.0, 0.0));
	double tenor(1.0);
	size_t nbSamplingPoints(5000);
	double accuracy(1e-9);
	double boundsStepping(0.2);
	size_t boundsMaxIterations(500);
	MarkovFunctionalDistribution markovDistribution(
		tenor,
		volParam,
		nbSamplingPoints,
		accuracy,
		boundsStepping,
		boundsMaxIterations);

	size_t nbPaths(1000000);
	boost::mt19937 rng;
	boost::uniform_01<> uniform;
	boost::variate_generator<boost::mt19937&, boost::uniform_01<> > rndGen(rng, uniform);
	double fwdFactor(Double::exp(rate * tenor));
	double df(Double::exp(-rate * tenor));
	double spot(1.0);
	double strike(1.0);
	double acc(0.0);
	//double variance(volatility * volatility * tenor);
	//double stdDev(Double::sqrt(variance));
	for (size_t i=0; i<nbPaths; ++i)
	{
		double rnd(rndGen());
		//cerr << "Rnd = " << rnd << endl;
		double psiInverse(markovDistribution.evaluatePsiInverse(rnd));
		//cerr << "Psi^{-1} = " << psiInverse << endl;
		//double back(Double::normalCdf(psiInverse / stdDev + 0.5 * stdDev));
		//cerr << "Check = " << rnd - back << endl;
		//cerr << "Back = " << back << endl;
		double finalSpot(spot * fwdFactor * Double::exp(psiInverse));
		//cerr << "Spot = " << finalSpot << endl << endl;
		acc += Double::max(0.0, finalSpot - strike);
	}
	double mcResult(df * acc / static_cast<double>(nbPaths));
	TEST_EQ_DBL(mcResult, 0.1233, 1e-4);
}
