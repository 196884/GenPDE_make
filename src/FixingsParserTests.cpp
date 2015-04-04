#include <iostream>
#include <fstream>

#include "Framework.h"
#include "TestSuite.h"

#include "GenPDEParser.h"

REGISTER_TEST(FixingsParsing1)
{
    std::string file("../resources/Fixings_1.xml");
    boost::shared_ptr<const TradeFixings> pe(GenPDEParser::parseFixings(file, true));
    std::cout << "Parsed fixings:" << std::endl;
    std::cout << *pe << std::endl;
    TEST(true);
}


