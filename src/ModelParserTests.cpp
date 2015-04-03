#include <iostream>
#include <fstream>

#include "Framework.h"
#include "TestSuite.h"

#include "GenPDEParser.h"

REGISTER_TEST(BSModelParsing1)
{
    std::string file("../resources/PDE_BS_Model_1.xml");
    boost::shared_ptr<PDEPricingModelInterface> pe(GenPDEParser::parsePDEModel(file, true));
    TEST(true);
}


