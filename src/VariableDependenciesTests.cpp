//
//  VariableDependenciesTests.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/24/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//



#include <boost/shared_ptr.hpp>

#include "Framework.h"
#include "TestSuite.h"

#include "VariableDependencies.h"

REGISTER_TEST(VarDependencies1)
{
    int ls[MAX_NB_SV_DEPS + MAX_NB_AV_DEPS];
    std::stringstream ss;
    
    VarDependencies vd0;
    ss << vd0;
    TEST_EQ(ss.str(), "Deps[]");
    ss.str(std::string());
    
    VarDependencies avd1(GenPDE::VT_AuxiliaryVariable, 1, 2);
    ss << avd1;
    TEST_EQ(ss.str(), "Deps[AV_1(2)]");
    ss.str(std::string());
    VarMemoryLayout vml1(avd1, true);
    vml1.getLoopStrides(vml1, &ls[0]);
    TEST_EQ(ls[0], 1);
    
    VarDependencies avd2(GenPDE::VT_AuxiliaryVariable, 2, 3);
    ss << avd2;
    TEST_EQ(ss.str(), "Deps[AV_2(3)]");
    ss.str(std::string());
    VarMemoryLayout vml2(avd2, true);
    vml2.getLoopStrides(vml2, &ls[0]);
    TEST_EQ(ls[0], 1);
    
    VarDependencies avd3(GenPDE::VT_AuxiliaryVariable, 3, 5);
    ss << avd3;
    TEST_EQ(ss.str(), "Deps[AV_3(5)]");
    ss.str(std::string());
    VarMemoryLayout vml3(avd3, true);
    vml3.getLoopStrides(vml3, &ls[0]);
    TEST_EQ(ls[0], 1);
    
    VarDependencies avd4(GenPDE::VT_AuxiliaryVariable, 4, 7);
    ss << avd4;
    TEST_EQ(ss.str(), "Deps[AV_4(7)]");
    ss.str(std::string());
    VarMemoryLayout vml4(avd4, true);
    vml4.getLoopStrides(vml4, &ls[0]);
    TEST_EQ(ls[0], 1);
    
    VarDependencies svd0(GenPDE::VT_StateVariable, 0, 10);
    ss << svd0;
    TEST_EQ(ss.str(), "Deps[SV_0(10)]");
    ss.str(std::string());
    VarMemoryLayout vml5(svd0, true);
    vml5.getLoopStrides(vml5, &ls[0]);
    TEST_EQ(ls[0], 1);
    
    VarDependencies svd1(GenPDE::VT_StateVariable, 1, 20);
    ss << svd1;
    TEST_EQ(ss.str(), "Deps[SV_1(20)]");
    ss.str(std::string());
    VarMemoryLayout vml6(svd1, true);
    vml6.getLoopStrides(vml6, &ls[0]);
    TEST_EQ(ls[0], 1);
    
    VarDependencies svd2(GenPDE::VT_StateVariable, 2, 30);
    ss << svd2;
    TEST_EQ(ss.str(), "Deps[SV_2(30)]");
    ss.str(std::string());
    VarMemoryLayout vml7(svd2, true);
    vml7.getLoopStrides(vml7, &ls[0]);
    TEST_EQ(ls[0], 1);
    
    VarDependencies av12d(avd1, avd2);
    ss << av12d;
    TEST_EQ(ss.str(), "Deps[AV_1(2), AV_2(3)]");
    ss.str(std::string());
    VarMemoryLayout vml8(av12d, true);
    vml8.getLoopStrides(vml8, &ls[0]);
    TEST_EQ(ls[0], 1);
    TEST_EQ(ls[1], 0);
    
    VarDependencies av134d(avd1, avd3, avd4);
    ss << av134d;
    TEST_EQ(ss.str(), "Deps[AV_1(2), AV_3(5), AV_4(7)]");
    ss.str(std::string());
    VarMemoryLayout vml9(av134d, true);
    vml9.getLoopStrides(av134d, &ls[0]);
    TEST_EQ(ls[0], 1);
    TEST_EQ(ls[1], 0);
    TEST_EQ(ls[2], 0);
    
    VarDependencies av1sv0d(avd1, svd0);
    ss << av1sv0d;
    TEST_EQ(ss.str(), "Deps[SV_0(10), AV_1(2)]");
    ss.str(std::string());
    VarMemoryLayout vml10(av1sv0d, true);
    vml10.getLoopStrides(vml10, &ls[0]);
    TEST_EQ(ls[0], 1);
    TEST_EQ(ls[1], 0);
    
    VarDependencies av3sv2d(avd3, svd2);
    ss << av3sv2d;
    TEST_EQ(ss.str(), "Deps[SV_2(30), AV_3(5)]");
    ss.str(std::string());
    VarMemoryLayout vml11(av3sv2d, true);
    vml11.getLoopStrides(vml11, &ls[0]);
    TEST_EQ(ls[0], 1);
    TEST_EQ(ls[1], 0);
    
    VarDependencies av134sv02d(av134d, av1sv0d, av3sv2d);
    ss << av134sv02d;
    TEST_EQ(ss.str(), "Deps[SV_0(10), SV_2(30), AV_1(2), AV_3(5), AV_4(7)]");
    ss.str(std::string());
    VarMemoryLayout vml12(av134sv02d, true);
    vml12.getLoopStrides(vml12, &ls[0]);
    TEST_EQ(ls[0], 1);
    TEST_EQ(ls[1], 0);
    TEST_EQ(ls[2], 0);
    TEST_EQ(ls[3], 0);
    TEST_EQ(ls[4], 0);
    
    VarDependencies av134sv02dv(av134sv02d);
    ss << av134sv02dv;
    TEST_EQ(ss.str(), "Deps[SV_0(10), SV_2(30), AV_1(2), AV_3(5), AV_4(7)]");
    ss.str(std::string());
    
    VarDependencies avd1db(avd1);
    ss << avd1db;
    TEST_EQ(ss.str(), "Deps[AV_1(2)]");
    ss.str(std::string());
    
    VarDependencies av14sv02d = av134sv02d.removeAVDependency(3);
    ss << av14sv02d;
    TEST_EQ(ss.str(), "Deps[SV_0(10), SV_2(30), AV_1(2), AV_4(7)]");
    ss.str(std::string());
    
    VarDependencies av14sv02db = av14sv02d.removeAVDependency(3);
    ss << av14sv02db;
    TEST_EQ(ss.str(), "Deps[SV_0(10), SV_2(30), AV_1(2), AV_4(7)]");
    ss.str(std::string());
    
    VarDependencies av1234sv012d(av134sv02d, avd2, svd1);
    ss << av1234sv012d;
    TEST_EQ(ss.str(), "Deps[SV_0(10), SV_1(20), SV_2(30), AV_1(2), AV_2(3), AV_3(5), AV_4(7)]");
    ss.str(std::string());
    VarMemoryLayout vml13(av1234sv012d, true);
    vml13.getLoopStrides(vml13, &ls[0]);
    TEST_EQ(ls[0], 1);
    TEST_EQ(ls[1], 0);
    TEST_EQ(ls[2], 0);
    TEST_EQ(ls[3], 0);
    TEST_EQ(ls[4], 0);
    TEST_EQ(ls[5], 0);
    TEST_EQ(ls[6], 0);
    
    VarMemoryLayout vml14(av1234sv012d, false);
    vml14.getLoopStrides(vml13, &ls[0]);
    TEST_EQ(ls[0], 6000);
    TEST_EQ(ls[1], 0);
    TEST_EQ(ls[2], 0);
    TEST_EQ(ls[3], 0);
    TEST_EQ(ls[4], 1 - 2 * 3 * 5 * 7 * 6000);
    TEST_EQ(ls[5], 0);
    TEST_EQ(ls[6], 0);
    
    vml12.getLoopStrides(vml13, &ls[0]);
    TEST_EQ(ls[0], 1);
    TEST_EQ(ls[1], -2);
    TEST_EQ(ls[2], 2);
    TEST_EQ(ls[3], 0);
    TEST_EQ(ls[4], 0);
    TEST_EQ(ls[5], -700);
    TEST_EQ(ls[6], 700);
    
    vml12.getLoopStrides(vml14, &ls[0]);
    TEST_EQ(ls[0], 70);
    TEST_EQ(ls[1], -700);
    TEST_EQ(ls[2], 700);
    TEST_EQ(ls[3], 1 - 30 * 700);
    TEST_EQ(ls[4], -2);
    TEST_EQ(ls[5], 2);
    TEST_EQ(ls[6], 0);
}