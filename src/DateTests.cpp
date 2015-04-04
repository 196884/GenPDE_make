#include "TestSuite.h"
#include "GpDate.h"

REGISTER_TEST(Date1)
{
    GenPDE::Date d1 = GenPDE::dateFromString("20140701");
    GenPDE::Date d2 = GenPDE::dateFromString("20140702");
    GenPDE::Date d3 = GenPDE::dateFromString("20140701");
    TEST(d1 != d2);
    TEST(d1 == d3);
}
