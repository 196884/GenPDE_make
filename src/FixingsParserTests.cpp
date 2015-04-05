#include <iostream>
#include <fstream>

#include "Framework.h"
#include "TestSuite.h"

#include "GenPDEParser.h"

REGISTER_TEST(FixingsParsing1)
{
    std::string file("../resources/MO_Fixings_1.xml");
    boost::shared_ptr<const MOFixingsIfc> moFixings( GenPDEParser::parseMOFixings( file, true ) );
    TEST( moFixings );
}


