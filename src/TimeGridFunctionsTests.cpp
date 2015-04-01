//
//  TimeGridFunctionsTests.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/25/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "Framework.h"
#include "TestSuite.h"

#include "TimeGridFunctions.h"

REGISTER_TEST(TimeGrid1)
{
    GenPDE::Date d0 = GenPDE::dateFromString("20010101");
    GenPDE::Date d1 = GenPDE::dateFromString("20020101");
    std::vector<GenPDE::Date> dates = {d0, d1};
    std::vector<double> timeGrid;
    std::vector<size_t> dateIndices;
    GenPDE::buildTimeGrid(
        dates,
        1,
        0,
        1,
        timeGrid,
        dateIndices
    );
    TEST_EQ(timeGrid.size(), 366);
    TEST_EQ(dateIndices.size(), 2);
    double ds = 1 / GenPDE::NbDaysPerYear;
    for(size_t i=0; i<366; ++i)
        TEST_EQ_DBL(timeGrid[i], i * ds, 1e-10);
    TEST_EQ(dateIndices[0], 0);
    TEST_EQ(dateIndices[1], 365);
}

REGISTER_TEST(TimeGrid2)
{
    GenPDE::Date d0 = GenPDE::dateFromString("20010101");
    GenPDE::Date d1 = GenPDE::dateFromString("20020101");
    std::vector<GenPDE::Date> dates = {d0, d1};
    std::vector<double> timeGrid;
    std::vector<size_t> dateIndices;
    GenPDE::buildTimeGrid(
        dates,
        1,
        4,
        0.25,
        timeGrid,
        dateIndices
    );
    TEST_EQ(timeGrid.size(), 369);
    TEST_EQ(dateIndices.size(), 2);
    double ds = 1 / GenPDE::NbDaysPerYear;
    for(size_t i=0; i<365; ++i)
        TEST_EQ_DBL(timeGrid[i], i * ds, 1e-10);
    double ds2 = 0.25 / GenPDE::NbDaysPerYear;
    for(size_t i=1; i<=4; ++i)
        TEST_EQ_DBL(timeGrid[364+i], timeGrid[364] + i * ds2, 1e-10);
    TEST_EQ(dateIndices[0], 0);
    TEST_EQ(dateIndices[1], 368);
}
