/*
 * RngTests.cpp
 *
 *  Created on: Nov 17, 2009
 *      Author: dupontr
 */

#include "Framework.h"
#include "TestSuite.h"

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_01.hpp>
#include <boost/random/variate_generator.hpp>

REGISTER_TEST(RngTest1)
{
	boost::mt19937 rng;
	boost::uniform_01<> uniform;
	boost::variate_generator<boost::mt19937&, boost::uniform_01<> > rndGen(rng, uniform);
	double acc1(0.0);
	double acc2(0.0);
	size_t nbSamples(10000000);
	for (size_t i=0; i<nbSamples; ++i)
	{
		double rnd(rndGen());
		acc1 += rnd;
		acc2 += rnd * rnd;
	}
	double mean1(acc1 / static_cast<double>(nbSamples));
	double mean2(acc2 / static_cast<double>(nbSamples));
	double stdDev(Double::sqrt(mean2 - mean1 * mean1));
	TEST_EQ_DBL(mean1, 0.5, 1e-4);
	TEST_EQ_DBL(stdDev, Double::sqrt(1.0 / 12.0), 1e-4);
}

