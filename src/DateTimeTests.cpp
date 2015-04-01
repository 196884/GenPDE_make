//
//  DateTimeTests.cpp
//  GenPDE
//
//  Created by Regis Dupont on 9/17/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "TestSuite.h"
#include "GpDate.h"
#include "GpTime.h"

REGISTER_TEST(Time1)
{
    GenPDE::Date d1 = GenPDE::dateFromString("20140701");
    //GenPDE::Date d2 = GenPDE::dateFromString("20140702");
    //GenPDE::Date d3 = GenPDE::dateFromString("20140703");
    double yfd      = 1.0 / 365.0;
    double yfh      = yfd / 24.0;
    double yfm      = yfh / 60.0;
    double yfs      = yfm / 60.0;
    GenPDE::Time t1 = GenPDE::timeFromDateAndYearFraction(d1, yfd);
    TEST_EQ(GenPDE::timeToString(t1), "20140702T000000");
    GenPDE::Time t2 = GenPDE::timeFromDateAndYearFraction(d1, -yfd);
    TEST_EQ(GenPDE::timeToString(t2), "20140630T000000");
    GenPDE::Time t3 = GenPDE::timeFromDateAndYearFraction(d1, yfd + yfh);
    TEST_EQ(GenPDE::timeToString(t3), "20140702T010000");
    GenPDE::Time t4 = GenPDE::timeFromDateAndYearFraction(d1, yfd + yfm);
    TEST_EQ(GenPDE::timeToString(t4), "20140702T000100");
    GenPDE::Time t5 = GenPDE::timeFromDateAndYearFraction(d1, yfd + yfs);
    TEST_EQ(GenPDE::timeToString(t5), "20140702T000001");
    GenPDE::Time t6 = GenPDE::timeFromDateAndYearFraction(d1, yfd - yfh);
    TEST_EQ(GenPDE::timeToString(t6), "20140701T230000");
    GenPDE::Time t7 = GenPDE::timeFromDateAndYearFraction(d1, yfd - yfm);
    TEST_EQ(GenPDE::timeToString(t7), "20140701T235900");
    GenPDE::Time t8 = GenPDE::timeFromDateAndYearFraction(d1, yfd - yfs);
    TEST_EQ(GenPDE::timeToString(t8), "20140701T235959");
}
