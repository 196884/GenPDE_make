//
//  CubicSplineInterpolatorTests.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/24/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "Framework.h"
#include "TestSuite.h"

#include "CubicSplineInterpolator.h"

/*
REGISTER_TEST(CubicSpline1)
{
    std::vector<double> x = { -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5 };
    std::vector<double> y;
    for(double xe : x)
        y.push_back(xe * xe);
    CubicSplineInterpolator interp(&x[0], &y[0], 11, ExtrapolateLinear);
    for(double d=-6; d<=6; d+=0.05)
    {
        double f = interp.interpolate(d);
        std::cout << "d: " << d << ", f: " << f << ", d^2:" << d * d << std::endl;
    }
}
*/
