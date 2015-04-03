//
//  CEValuesStoredTests.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/21/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include <boost/shared_ptr.hpp>

#include "Framework.h"
#include "TestSuite.h"

#include "VariableDependencies.h"
#include "CEValuesStored.h"

REGISTER_TEST(CEValuesStored1)
{
    VarDependencies svDepsNone;
    CEValuesStored v0(svDepsNone);
    VarBindings vb;
    v0.setValue(vb, 3.14);
    TEST_EQ_DBL(v0.getValue(vb), 3.14, 1e-10);
}

// TODO: fix CEValuesStored2
REGISTER_TEST(CEValuesStored2)
{
    VarDependencies avd1(GenPDE::VT_AuxiliaryVariable, 1, 3);
    VarDependencies avd2(GenPDE::VT_AuxiliaryVariable, 2, 5);
    VarDependencies avd3(GenPDE::VT_AuxiliaryVariable, 3, 2);
    VarDependencies sv1(GenPDE::VT_StateVariable, 1, 10);
    VarDependencies sv2(GenPDE::VT_StateVariable, 2, 7);
    VarDependencies av123(avd1, avd2, avd3);
    VarDependencies sv12(sv1, sv2);
    VarDependencies sv12av123(sv12, av123);
    CEValuesStored v(sv12av123);
    
    double* ptr = v.getDataPtr();
    for(size_t i=0; i<2100; ++i)
        ptr[i] = i;
    VarBindings vb;
    double cmp = 0;
    for(size_t y=0; y<7; ++y)
    {
        vb.setSVIndex(2, y);
        for(size_t x=0; x<10; ++x)
        {
            vb.setSVIndex(1, x);
            for(size_t k=0; k<2; ++k)
            {
                vb.setAVIndex(3, k);
                for(size_t j=0; j<5; ++j)
                {
                    vb.setAVIndex(2, j);
                    for(size_t i=0; i<3; ++i)
                    {
                        vb.setAVIndex(1, i);
                        TEST_EQ_DBL(v.getValue(vb), cmp, 1e-10);
                        ++cmp;
                    }
                }
            }
        }
    }
}
