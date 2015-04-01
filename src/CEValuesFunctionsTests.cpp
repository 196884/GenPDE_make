//
//  CEValuesFunctionsTests.cpp
//  GenPDE
//
//  Created by Regis Dupont on 9/1/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include <vector>

#include "Framework.h"
#include "TestSuite.h"

#include <boost/shared_ptr.hpp>

#include "CEValues.h"
#include "CEValuesStored.h"

REGISTER_TEST(CEValues_Functions_1)
{
    using namespace std;
    typedef boost::shared_ptr<CEValues> CEVPtr;
    
    // Defining the SVs, MOs and AVs:
    vector<double> mo1 = { -0.3, -0.1, 3.4, 7.9, 10.11, 17.9, -9.4 };
    vector<double> mo2 = { -2.2, 6.4, -0.3, 5.3, 2.1, 1.4 };
    vector<double> mo3 = { 1.1, 2.3, 3.7, 5.3, 7.1, 9.1, 11.3, 13.5 };
    vector<double> av1 = { 0.7, 0.9, 4.3, 5.9 };
    vector<double> av2 = { 5.1, 8.3, 9.7, 15.3, 19.5 };
    vector<double> av3 = { 0.4, 0.5, 0.6, 0.8, 0.9, 1.1, 1.4, 1.7, 2.1, 3.8, 4.5 };
    double         c1  = 1.0;
    double         c2  = -3.14;
    double         c3  = 666;
    
    VarDependencies svDep1(GenPDE::VT_StateVariable, 1, mo1.size());
    VarDependencies svDep2(GenPDE::VT_StateVariable, 2, mo2.size());
    VarDependencies svDep3(GenPDE::VT_StateVariable, 3, mo3.size());
    
    VarDependencies avDep1(GenPDE::VT_AuxiliaryVariable, 1, av1.size());
    VarDependencies avDep2(GenPDE::VT_AuxiliaryVariable, 2, av2.size());
    VarDependencies avDep3(GenPDE::VT_AuxiliaryVariable, 3, av3.size());
    
    CEVPtr mo1V(new CEValuesStored(svDep1));
    std::copy(mo1.begin(), mo1.end(), mo1V->getDataPtr());
    CEVPtr mo2V(new CEValuesStored(svDep2));
    std::copy(mo2.begin(), mo2.end(), mo2V->getDataPtr());
    CEVPtr mo3V(new CEValuesStored(svDep3));
    std::copy(mo3.begin(), mo3.end(), mo3V->getDataPtr());
    CEVPtr av1V(new CEValuesStored(avDep1));
    std::copy(av1.begin(), av1.end(), av1V->getDataPtr());
    CEVPtr av2V(new CEValuesStored(avDep2));
    std::copy(av2.begin(), av2.end(), av2V->getDataPtr());
    CEVPtr av3V(new CEValuesStored(avDep3));
    std::copy(av3.begin(), av3.end(), av3V->getDataPtr());
    CEVPtr c1V(new CEValuesStored(c1));
    CEVPtr c2V(new CEValuesStored(c2));
    CEVPtr c3V(new CEValuesStored(c3));
    
    VarBindings vb;
    
    // c1 + c2:
    CEVPtr c1_Add_c2(new CEValuesStored(0));
    c1_Add_c2->evalBinaryOperation(GenPDE::Add, c1V, c2V);
    //TEST_EQ(c1_Add_c2->getAVDependencies().getNbDependencies(), 0);
    //TEST_EQ(c1_Add_c2->getSVDependencies().getNbDependencies(), 0);
    TEST_EQ_DBL(*c1_Add_c2->getDataPtr(), c1 + c2, 1e-10);
    
    // -mo1:
    CEVPtr Neg_mo1(new CEValuesStored(svDep1));
    Neg_mo1->evalUnaryOperation(GenPDE::Neg, mo1V);
    //TEST_EQ(Neg_mo1->getAVDependencies().getNbDependencies(), 0);
    //TEST_EQ(Neg_mo1->getSVDependencies().getNbDependencies(), 1);
    //TEST_EQ(Neg_mo1->getSVDependencies().getNbConfigurations(1), mo1.size());
    for(size_t i=0; i<mo1.size(); ++i)
    {
        vb.setSVIndex(1, i);
        TEST_EQ_DBL(Neg_mo1->getValue(vb), -mo1[i], 1e-10);
    }
    
    // av1 * c1
    CEVPtr av1_Mul_c1(new CEValuesStored(avDep1));
    av1_Mul_c1->evalBinaryOperation(GenPDE::Mul, av1V, c1V);
    //TEST_EQ(av1_Mul_c1->getSVDependencies().getNbDependencies(), 0);
    //TEST_EQ(av1_Mul_c1->getAVDependencies().getNbDependencies(), 1);
    //TEST_EQ(av1_Mul_c1->getAVDependencies().getNbConfigurations(1), av1.size());
    vb.clear();
    for(size_t i=0; i<av1.size(); ++i)
    {
        vb.setAVIndex(1, i);
        TEST_EQ_DBL(av1_Mul_c1->getValue(vb), av1[i] * c1, 1e-10);
    }
    
    // mo2 - c3
    CEVPtr mo2_Sub_c3(new CEValuesStored(svDep2));
    mo2_Sub_c3->evalBinaryOperation(GenPDE::Sub, mo2V, c3V);
    //TEST_EQ(mo2_Sub_c3->getSVDependencies().getNbDependencies(), 1);
    //TEST_EQ(mo2_Sub_c3->getSVDependencies().getNbConfigurations(2), mo2.size());
    //TEST_EQ(mo2_Sub_c3->getAVDependencies().getNbDependencies(), 0);
    vb.clear();
    for(size_t i=0; i<mo2.size(); ++i)
    {
        vb.setSVIndex(2, i);
        TEST_EQ_DBL(mo2_Sub_c3->getValue(vb), mo2[i] - c3, 1e-10);
    }
    
    // mo3 / ( av1 * c1 )
    VarDependencies av3av1Dep(svDep3, avDep1);
    CEVPtr res1(new CEValuesStored(av3av1Dep));
    res1->evalBinaryOperation(GenPDE::Div, mo3V, av1_Mul_c1);
    //TEST_EQ(res1->getSVDependencies().getNbDependencies(), 1);
    //TEST_EQ(res1->getSVDependencies().getNbConfigurations(3), mo3.size());
    //TEST_EQ(res1->getAVDependencies().getNbDependencies(), 1);
    //TEST_EQ(res1->getAVDependencies().getNbConfigurations(1), av1.size());
    vb.clear();
    for(size_t i=0; i<mo3.size(); ++i)
    {
        vb.setSVIndex(3, i);
        for(size_t j=0; j<av1.size(); ++j)
        {
            vb.setAVIndex(1, j);
            TEST_EQ_DBL(res1->getValue(vb), mo3[i] / (av1[j] * c1), 1e-10);
        }
    }
    
    // Max(mo1, mo2)
    VarDependencies sv12Deps(svDep1, svDep2);
    CEVPtr mo1_Max_mo2(new CEValuesStored(sv12Deps));
    mo1_Max_mo2->evalBinaryOperation(GenPDE::Max, mo1V, mo2V);
    //TEST_EQ(mo1_Max_mo2->getSVDependencies().getNbDependencies(), 2);
    //TEST_EQ(mo1_Max_mo2->getSVDependencies().getNbConfigurations(1), mo1.size());
    //TEST_EQ(mo1_Max_mo2->getSVDependencies().getNbConfigurations(2), mo2.size());
    //TEST_EQ(mo1_Max_mo2->getAVDependencies().getNbDependencies(), 0);
    vb.clear();
    for(size_t i=0; i<mo1.size(); ++i)
    {
        vb.setSVIndex(1, i);
        for(size_t j=0; j<mo2.size(); ++j)
        {
            vb.setSVIndex(2, j);
            TEST_EQ_DBL(mo1_Max_mo2->getValue(vb), Double::max(mo1[i], mo2[j]), 1e-10);
        }
    }
    
    // av1 + av3
    VarDependencies av13Deps(avDep1, avDep3);
    CEVPtr av1_Add_av3(new CEValuesStored(av13Deps));
    av1_Add_av3->evalBinaryOperation(GenPDE::Add, av1V, av3V);
    //TEST_EQ(av1_Add_av3->getSVDependencies().getNbDependencies(), 0);
    //TEST_EQ(av1_Add_av3->getAVDependencies().getNbDependencies(), 2);
    //TEST_EQ(av1_Add_av3->getAVDependencies().getNbConfigurations(1), av1.size());
    //TEST_EQ(av1_Add_av3->getAVDependencies().getNbConfigurations(3), av3.size());
    vb.clear();
    vb.setSVIndex(1, 1);
    vb.setSVIndex(2, 2);
    vb.setSVIndex(3, 3);
    for(size_t i=0; i<av1.size(); ++i)
    {
        vb.setAVIndex(1, i);
        for(size_t j=0; j<av3.size(); ++j)
        {
            vb.setAVIndex(3, j);
            TEST_EQ_DBL(av1_Add_av3->getValue(vb), av1[i] + av3[j], 1e-10);
        }
    }
    
    // Max(mo1, mo2) + av1 + av3
    VarDependencies sv12av13Deps(sv12Deps, av13Deps);
    CEVPtr res2(new CEValuesStored(sv12av13Deps));
    res2->evalBinaryOperation(GenPDE::Add, mo1_Max_mo2, av1_Add_av3);
    //TEST_EQ(res2->getSVDependencies().getNbDependencies(), 2);
    //TEST_EQ(res2->getSVDependencies().getNbConfigurations(1), mo1.size());
    //TEST_EQ(res2->getSVDependencies().getNbConfigurations(2), mo2.size());
    //TEST_EQ(res2->getAVDependencies().getNbDependencies(), 2);
    //TEST_EQ(res2->getAVDependencies().getNbConfigurations(1), av1.size());
    //TEST_EQ(res2->getAVDependencies().getNbConfigurations(3), av3.size());
    for(size_t i=0; i<mo1.size(); ++i)
    {
        vb.setSVIndex(1, i);
        for(size_t j=0; j<mo2.size(); ++j)
        {
            vb.setSVIndex(2, j);
            for(size_t k=0; k<av1.size(); ++k)
            {
                vb.setAVIndex(1, k);
                for(size_t l=0; l<av3.size(); ++l)
                {
                    vb.setAVIndex(3, l);
                    TEST_EQ_DBL(res2->getValue(vb), Double::max(mo1[i], mo2[j]) + av1[k] + av3[l], 1e-10);
                }
            }
        }
    }
    
    // mo1 + mo2
    CEVPtr mo12(new CEValuesStored(sv12Deps));
    mo12->evalBinaryOperation(GenPDE::Add, mo1V, mo2V);
    //TEST_EQ(mo12->getSVDependencies().getNbDependencies(), 2);
    //TEST_EQ(mo12->getSVDependencies().getNbConfigurations(1), mo1.size());
    //TEST_EQ(mo12->getSVDependencies().getNbConfigurations(2), mo2.size());
    //TEST_EQ(mo12->getAVDependencies().getNbDependencies(), 0);
    for(size_t i=0; i<mo1.size(); ++i)
    {
        vb.setSVIndex(1, i);
        for(size_t j=0; j<mo2.size(); ++j)
        {
            vb.setSVIndex(2, j);
            TEST_EQ_DBL(mo12->getValue(vb), mo1[i] + mo2[j], 1e-10);
        }
    }
    
    // mo2 + mo3
    VarDependencies sv23Deps(svDep2, svDep3);
    CEVPtr mo23(new CEValuesStored(sv23Deps));
    mo23->evalBinaryOperation(GenPDE::Add, mo2V, mo3V);
    //TEST_EQ(mo23->getSVDependencies().getNbDependencies(), 2);
    //TEST_EQ(mo23->getSVDependencies().getNbConfigurations(3), mo3.size());
    //TEST_EQ(mo23->getSVDependencies().getNbConfigurations(2), mo2.size());
    //TEST_EQ(mo23->getAVDependencies().getNbDependencies(), 0);
    for(size_t i=0; i<mo2.size(); ++i)
    {
        vb.setSVIndex(2, i);
        for(size_t j=0; j<mo3.size(); ++j)
        {
            vb.setSVIndex(3, j);
            TEST_EQ_DBL(mo23->getValue(vb), mo2[i] + mo3[j], 1e-10);
        }
    }
    
    // mo1 + 2mo2 + mo3
    VarDependencies sv123Deps(sv12Deps, svDep3);
    CEVPtr res3(new CEValuesStored(sv123Deps));
    res3->evalBinaryOperation(GenPDE::Add, mo12, mo23);
    //TEST_EQ(res3->getSVDependencies().getNbDependencies(), 3);
    //TEST_EQ(res3->getSVDependencies().getNbConfigurations(1), mo1.size());
    //TEST_EQ(res3->getSVDependencies().getNbConfigurations(2), mo2.size());
    //TEST_EQ(res3->getSVDependencies().getNbConfigurations(3), mo3.size());
    //TEST_EQ(res3->getAVDependencies().getNbDependencies(), 0);
    for(size_t i=0; i<mo1.size(); ++i)
    {
        vb.setSVIndex(1, i);
        for(size_t j=0; j<mo2.size(); ++j)
        {
            vb.setSVIndex(2, j);
            for(size_t k=0; k<mo3.size(); ++k)
            {
                vb.setSVIndex(3, k);
                TEST_EQ_DBL(res3->getValue(vb), mo1[i] + 2 * mo2[j] + mo3[k], 1e-10);
            }
        }
    }
    
    // av1 + av2
    VarDependencies av12Deps(avDep1, avDep2);
    CEVPtr av12(new CEValuesStored(av12Deps));
    av12->evalBinaryOperation(GenPDE::Add, av1V, av2V);
    //TEST_EQ(av12->getAVDependencies().getNbDependencies(), 2);
    //TEST_EQ(av12->getSVDependencies().getNbDependencies(), 0);
    //TEST_EQ(av12->getAVDependencies().getNbConfigurations(1), av1.size());
    //TEST_EQ(av12->getAVDependencies().getNbConfigurations(2), av2.size());
    for(size_t i=0; i<av1.size(); ++i)
    {
        vb.setAVIndex(1, i);
        for(size_t j=0; j<av2.size(); ++j)
        {
            vb.setAVIndex(2, j);
            TEST_EQ_DBL(av12->getValue(vb), av1[i] + av2[j], 1e-10);
        }
    }
    
    // av1 + av3
    CEVPtr av13(new CEValuesStored(av13Deps));
    av13->evalBinaryOperation(GenPDE::Add, av1V, av3V);
    //TEST_EQ(av13->getAVDependencies().getNbDependencies(), 2);
    //TEST_EQ(av13->getSVDependencies().getNbDependencies(), 0);
    //TEST_EQ(av13->getAVDependencies().getNbConfigurations(1), av1.size());
    //TEST_EQ(av13->getAVDependencies().getNbConfigurations(3), av3.size());
    for(size_t i=0; i<av1.size(); ++i)
    {
        vb.setAVIndex(1, i);
        for(size_t j=0; j<av3.size(); ++j)
        {
            vb.setAVIndex(3, j);
            TEST_EQ_DBL(av13->getValue(vb), av1[i] + av3[j], 1e-10);
        }
    }
    
    // mo1 + av1 + av3
    VarDependencies sv1av13Deps(svDep1, av13Deps);
    CEVPtr res4(new CEValuesStored(sv1av13Deps));
    res4->evalBinaryOperation(GenPDE::Add, mo1V, av13);
    
    // mo2 + mo3 + av1 + av2
    VarDependencies sv23av12Deps(sv23Deps, av12Deps);
    CEVPtr res5(new CEValuesStored(sv23av12Deps));
    res5->evalBinaryOperation(GenPDE::Add, av12, mo23);
    
    // mo2 + av2
    VarDependencies sv2av2Deps(svDep2, avDep2);
    CEVPtr res6(new CEValuesStored(sv2av2Deps));
    res6->evalBinaryOperation(GenPDE::Add, mo2V, av2V);
    
    // 2 * mo2 + mo3 + av1 + 2 * av2
    CEVPtr res7(new CEValuesStored(sv23av12Deps));
    res7->evalBinaryOperation(GenPDE::Add, res6, res5);
    
    // 3 * mo2 + mo3 + av1 + 3 * av2
    CEVPtr res8(new CEValuesStored(sv23av12Deps));
    res8->evalBinaryOperation(GenPDE::Add, res7, res6);
    
    // mo1 + av1
    VarDependencies sv1av1Deps(svDep1, avDep1);
    CEVPtr res9(new CEValuesStored(sv1av1Deps));
    res9->evalBinaryOperation(GenPDE::Add, av1V, mo1V);
    
    // mo1 + 3 * mo2 + mo3 + 2 * av1 + 3 * av2
    VarDependencies sv123av12Deps(sv123Deps, av12Deps);
    CEVPtr res10(new CEValuesStored(sv123av12Deps));
    res10->evalBinaryOperation(GenPDE::Add, res8, res9);
    
    // mo2 + av3
    VarDependencies sv2av3Deps(svDep2, avDep3);
    CEVPtr res11(new CEValuesStored(sv2av3Deps));
    res11->evalBinaryOperation(GenPDE::Add, mo2V, av3V);
    
    // mo1 + 4 * mo2 + mo3 + 2 * av1 + 3 * av2 + av3
    VarDependencies sv123av123Deps(sv123Deps, av13Deps, avDep2);
    CEVPtr res12(new CEValuesStored(sv123av123Deps));
    res12->evalBinaryOperation(GenPDE::Add, res11, res10);
    
    for(size_t i=0; i<mo1.size(); ++i)
    {
        vb.setSVIndex(1, i);
        for(size_t j=0; j<mo2.size(); ++j)
        {
            vb.setSVIndex(2, j);
            for(size_t k=0; k<mo3.size(); ++k)
            {
                vb.setSVIndex(3, k);
                for(size_t l=0; l<av1.size(); ++l)
                {
                    vb.setAVIndex(1, l);
                    for(size_t m=0; m<av2.size(); ++m)
                    {
                        vb.setAVIndex(2, m);
                        for(size_t n=0; n<av3.size(); ++n)
                        {
                            vb.setAVIndex(3, n);
                            TEST_EQ_DBL(res4->getValue(vb), mo1[i] + av1[l] + av3[n], 1e-10);
                            TEST_EQ_DBL(res5->getValue(vb), mo2[j] + mo3[k] + av1[l] + av2[m], 1e-10);
                            TEST_EQ_DBL(res6->getValue(vb), mo2[j] + av2[m], 1e-10);
                            TEST_EQ_DBL(res7->getValue(vb), 2 * mo2[j] + mo3[k] + av1[l] + 2 * av2[m], 1e-10);
                            TEST_EQ_DBL(res8->getValue(vb), 3 * mo2[j] + mo3[k] + av1[l] + 3 * av2[m], 1e-10);
                            TEST_EQ_DBL(res9->getValue(vb), mo1[i] + av1[l], 1e-10);
                            TEST_EQ_DBL(res10->getValue(vb), mo1[i] + 3 * mo2[j] + mo3[k] + 2 * av1[l] + 3 * av2[m], 1e-10);
                            TEST_EQ_DBL(res11->getValue(vb), mo2[j] + av3[n], 1e-10);
                            TEST_EQ_DBL(res12->getValue(vb), mo1[i] + 4 * mo2[j] + mo3[k] + 2 * av1[l] + 3 * av2[m] + av3[n], 1e-10);
                        }
                    }
                }
            }
        }
    }
    
}