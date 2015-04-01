//
//  DateTests.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/22/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "TestSuite.h"
#include "Date.h"

REGISTER_TEST(Date1)
{
    GenPDE::Date d1 = GenPDE::dateFromString("20140701");
    GenPDE::Date d2 = GenPDE::dateFromString("20140702");
    GenPDE::Date d3 = GenPDE::dateFromString("20140701");
    TEST(d1 != d2);
    TEST(d1 == d3);
}
