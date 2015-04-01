//
//  TestSuite_main.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/31/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "Framework.h"
#include "TestSuite.h"

int main(int argc, const char * argv[]) {
    try {
        TestSuite::runAll();
        TestSuite::printRecapWithFailures();
    } catch ( Exception e ) {
        e.print();
    }
    return 0;
}
