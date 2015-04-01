//
//  MCPricingChecks.cpp
//  GenPDE
//
//  Created by Regis Dupont on 9/14/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "Framework.h"
#include "TestSuite.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>


#include <iostream>
using namespace std;

REGISTER_TEST(MCPricingCheck_1)
{
	double volatility(0.25);
	double rate(0.05);
    double spot(100.0);
    double tenor(1.0);
    size_t nbDays(365);
    double k(100.0);
    
    double dt(tenor / ((double) nbDays));
    double variance(volatility * volatility * dt);
    double stdDev(volatility * Double::sqrt(dt));
    double factor(Double::exp(rate * dt - 0.5 * variance));
    double df(Double::exp(-rate * tenor));
    
    size_t nbPaths(10000000);
    
	boost::mt19937 rng;
	boost::normal_distribution<> normal;
	boost::variate_generator<boost::mt19937&, boost::normal_distribution<> > rndGen(rng, normal);
    
    double acc_1       = 0;
    double acc_2       = 0;
    double pctComplete = 0;
    double pctStep     = 0.1;
    
    boost::posix_time::ptime mst1 = boost::posix_time::microsec_clock::local_time();
    for(size_t i=0; i<nbPaths; ++i)
    {
        double s = spot;
        double a = 0;
        for(size_t t=0; t<nbDays; ++t)
        {
            s *= factor * Double::exp(stdDev * rndGen());
            a += s;
        }
        a /= (double) nbDays;
        if( a >= k )
        {
            acc_1 += df * (a - k);
            acc_2 += df * df * (a - k) * (a - k);
        }
        double done = 100.0 * ((double) (i+1)) / ((double) nbPaths);
        if( done > pctComplete + pctStep )
        {
            pctComplete += pctStep;
            boost::posix_time::ptime mstL = boost::posix_time::microsec_clock::local_time();
            cout << pctComplete << "% done (" << (mstL - mst1).total_microseconds() << " us)" << endl;
        }
    }
    
    double avg_1 = acc_1 / ((double) nbPaths);
    double avg_2 = acc_2 / ((double) nbPaths);
    double error = Double::sqrt((avg_2 - avg_1 * avg_1) / ((double) (nbPaths - 1)));
    cout << "Mean:  " << avg_1 << std::endl;
    cout << "Error: " << error << std::endl;
}
