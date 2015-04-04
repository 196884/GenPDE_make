#include "Framework.h"
#include "TestSuite.h"

#include "AVInterpolatorLinear.h"
#include "AVInterpolatorNaturalCubic.h"
#include "CubicSplineInterpolator.h"

REGISTER_TEST(AVInterpolatorLinear)
{
    std::vector<double> xs = { 2.4, 3.7, 3.9, 4.2, 5.6, 8.9, 22.3 };
    AVInterpolatorLinear interp1(&xs[0], xs.size(), GenPDE::ExtrapolationType_Flat);
    
    std::vector<double> ys1;
    double a = -3.5;
    double b = 1.7;
    for(double x : xs)
        ys1.push_back(a * x + b);
    interp1.updateY(&ys1[0]);
    TEST_EQ_DBL(interp1.interpolate(0), a * 2.4 + b, 1e-10);
    TEST_EQ_DBL(interp1.interpolate(30), a * 22.3 + b, 1e-10);
    for(double x=xs.front(); x<=xs.back(); x += 0.01)
        TEST_EQ_DBL(interp1.interpolate(x), a * x + b, 1e-10);
    
    std::vector<double> ys2;
    a = 2.4;
    b = -0.9;
    for(double x : xs)
        ys2.push_back(a * x + b);
    interp1.updateY(&ys2[0]);
    TEST_EQ_DBL(interp1.interpolate(0), a * 2.4 + b, 1e-10);
    TEST_EQ_DBL(interp1.interpolate(30), a * 22.3 + b, 1e-10);
    for(double x=xs.front(); x<=xs.back(); x += 0.01)
        TEST_EQ_DBL(interp1.interpolate(x), a * x + b, 1e-10);
    
    AVInterpolatorLinear interp2(&xs[0], xs.size(), GenPDE::ExtrapolationType_Linear);
    interp2.updateY(&ys2[0]);
    for(double x=0; x<=24; x+=0.5)
        TEST_EQ_DBL(interp2.interpolate(x), a * x + b, 1e-10);
}

REGISTER_TEST(AVInterpolatorNaturalCubic)
{
    std::vector<double> xs = { 2.4, 3.7, 3.9, 4.2, 5.6, 8.9, 22.3 };
    AVInterpolatorNaturalCubic interp1(&xs[0], xs.size(), GenPDE::ExtrapolationType_Flat);
    
    std::vector<double> ys1;
    double a = -3.5;
    double b = 1.7;
    double c = 0.9;
    double d = -2.1;
    for(double x : xs)
        ys1.push_back(a + x * (b + x * (c + x * d)));
    interp1.updateY(&ys1[0]);
    std::vector<double> xTest = { 2, 3, 4, 5, 6, 15, 30 };
    std::vector<double> yTest = {
        -23.2664,
        -42.8324142113794,
        -116.147391111805,
        -228.393396201595,
        -353.531937186963,
        -6130.68425103026,
        -22806.1197
    };
    for(size_t i=0; i<xTest.size(); ++i)
        TEST_EQ_DBL(interp1.interpolate(xTest[i]), yTest[i], 1e-8);
    
    std::vector<double> ys2;
    a = 2.4;
    b = -0.9;
    c = 5.3;
    d = -7.3;
    for(double x : xs)
        ys2.push_back(a + x * (b + x * (c + x * d)));
    interp1.updateY(&ys2[0]);
    std::vector<double> yTest2 = {
        -70.1472,
        -135.581979919229,
        -381.718853031089,
        -759.987896250283,
        -1179.53821644215,
        -20950.0418654796,
        -78335.8721
    };
    for(size_t i=0; i<xTest.size(); ++i)
        TEST_EQ_DBL(interp1.interpolate(xTest[i]), yTest2[i], 1e-8);
    
    AVInterpolatorNaturalCubic interp2(&xs[0], xs.size(), GenPDE::ExtrapolationType_Linear);
    interp2.updateY(&ys2[0]);
    std::vector<double> yTest3 = {
        0.00440000000003238,
        -135.581979919229,
        -381.718853031089,
        -759.987896250283,
        -1179.53821644215,
        -20950.0418654796,
        -120630.5938
    };
    for(size_t i=0; i<xTest.size(); ++i)
        TEST_EQ_DBL(interp2.interpolate(xTest[i]), yTest3[i], 1e-8);
}

REGISTER_TEST(AVInterpolatorNaturalCubicUniform)
{
    size_t N(10);
    double x0 = 1.1;
    double dx = 0.7;
    std::vector<double> xs(N);
    std::vector<double> expXs(N);
    for(size_t i=0; i<N; ++i)
    {
        xs[i]    = x0 + i * dx;
        expXs[i] = Double::exp(xs[i]);
    }
    std::vector<double> ys = { 2.1, 4.2, 5.1, 5.9, 6.3, 7.0, 6.8, 6.5, 5.3, 2.1 };
    AVInterpolatorNaturalCubic interp1(&xs[0], N, GenPDE::ExtrapolationType_Linear);
    AVInterpolatorNaturalCubicUniform<> interp2(&xs[0], N, GenPDE::ExtrapolationType_Linear);
    AVInterpolatorLinear interp3(&xs[0], N, GenPDE::ExtrapolationType_Linear);
    AVInterpolatorLinearUniform<> interp4(&xs[0], N, GenPDE::ExtrapolationType_Linear);
    interp1.updateY(&ys[0]);
    interp2.updateY(&ys[0]);
    interp3.updateY(&ys[0]);
    interp4.updateY(&ys[0]);
    std::vector<double> c = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    for(size_t i=0; i<N; ++i)
    {
        TEST_EQ_DBL(interp1.interpolate(c[i]), interp2.interpolate(c[i]), 1e-10);
        TEST_EQ_DBL(interp3.interpolate(c[i]), interp4.interpolate(c[i]), 1e-10);
    }
}
