#include <iostream>
#include <fstream>
#include "Framework.h"
#include "TestSuite.h"

int main(int argc, const char * argv[]) {
    try {
        //TestSuite::runTest("PDETradePricer_AsianCall_1");
        TestSuite::runTest("ASROption1");
        //TestSuite::runAll();
        //TestSuite::printAllTestNames();
        TestSuite::printAllResults();
    } catch ( Exception e ) {
        e.print();
    }
    return 0;
}
