//
//  PEBinaryOperationTests.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/24/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "Framework.h"
#include "TestSuite.h"
#include "GpDate.h"

#include "PEBinaryOperation.h"
#include "MOContextTests.h"
#include "GenPDEParser.h"

REGISTER_TEST(PEBinaryOperation1)
{
    MOContextTest moContext;
    AVContext     avContext;
    VarBindings   vb;
    GenPDE::Date  noDate = GenPDE::dateFromString("20000101");
    boost::shared_ptr<PayoutExpression> noPE;
    boost::shared_ptr<AuxiliaryVariable> av1(new AuxiliaryVariable(1, noDate, noPE));
    boost::shared_ptr<AuxiliaryVariable> av2(new AuxiliaryVariable(2, noDate, noPE));
    boost::shared_ptr<AuxiliaryVariable> av3(new AuxiliaryVariable(3, noDate, noPE));
    boost::shared_ptr<AuxiliaryVariable> av4(new AuxiliaryVariable(4, noDate, noPE));
    std::vector<double> v1 = { 2, 3, 5, 7, 11 };
    std::vector<double> v2 = { 0, 1 };
    std::vector<double> v3 = { 10, 11, 12, 13 };
    std::vector<double> v4 = { 42 };
    av1->setDiscretizationValues(v1);
    av2->setDiscretizationValues(v2);
    av3->setDiscretizationValues(v3);
    av4->setDiscretizationValues(v4);
    avContext.setAuxiliaryVariable(av1);
    avContext.setAuxiliaryVariable(av2);
    avContext.setAuxiliaryVariable(av3);
    avContext.setAuxiliaryVariable(av4);
    
    std::string s1("<BinOp op=\"Add\"><Const value=\"3\"/><Const value=\"5\"/></BinOp>");
    boost::shared_ptr<const PayoutExpression> pe1 = GenPDEParser::parsePayoutExpression(s1);
    boost::shared_ptr<const CEValues> cev1 = pe1->evalCE(moContext, avContext);
    //TEST_EQ(cev1->getInnerSVStride(), 1);
    //TEST_EQ(cev1->getAVDependencies().getNbConfigurations(), 1);
    //TEST_EQ(cev1->getSVDependencies().getNbConfigurations(), 1);
    TEST_EQ_DBL(cev1->getDataPtr()[0], 8, 1e-10);
    
    std::string s2("<BinOp op=\"Sub\"><MORef uid=\"1\"/><Const value=\"3\"/></BinOp>");
    boost::shared_ptr<const PayoutExpression> pe2 = GenPDEParser::parsePayoutExpression(s2);
    boost::shared_ptr<const CEValues> cev2 = pe2->evalCE(moContext, avContext);
    //TEST_EQ(cev2->getInnerSVStride(), 1);
    //TEST_EQ(cev2->getAVDependencies().getNbConfigurations(), 1);
    //TEST_EQ(cev2->getSVDependencies().getNbConfigurations(), 3);
    //TEST_EQ(cev2->getSVDependencies().getNbConfigurations(1), 3);
    const double* ptr2 = cev2->getDataPtr();
    for(size_t i=0; i<3; ++i)
        TEST_EQ_DBL(ptr2[i], (double) i-3, 1e-10);
    
    std::stringstream ss3;
    ss3 << "<BinOp op=\"Mul\">";
    ss3 <<     "<BinOp op=\"Add\">";
    ss3 <<         "<BinOp op=\"Max\">";
    ss3 <<             "<AVRef uid=\"3\"/>";
    ss3 <<             "<Const value=\"11\"/>";
    ss3 <<         "</BinOp>";
    ss3 <<         "<MORef uid=\"2\"/>";
    ss3 <<     "</BinOp>";
    ss3 <<     "<BinOp op=\"Add\">";
    ss3 <<         "<AVRef uid=\"1\"/>";
    ss3 <<         "<MORef uid=\"123\"/>";
    ss3 <<     "</BinOp>";
    ss3 << "</BinOp>";
    // (Max(AV3, 11) + MO2) * (AV1 + MO123)
    // AV1 = { 2, 3, 5, 7, 11 }
    // AV3 = { 10, 11, 12, 13 }
    // MO1 = { 0, 1, 2 }
    // MO2 = { 0, 2, 4, 6, 8 }
    // MO3 = { 0, 3, 6, 9, 12, 15, 18 }
    // MO123 = (3 * SV1 + 2 * SV2 - SV3)
    std::stringstream ss3_1;
    ss3_1 <<         "<BinOp op=\"Max\">";
    ss3_1 <<             "<AVRef uid=\"3\"/>";
    ss3_1 <<             "<Const value=\"11\"/>";
    ss3_1 <<         "</BinOp>";
    std::string s3_1(ss3_1.str());
    boost::shared_ptr<const PayoutExpression> pe3_1 = GenPDEParser::parsePayoutExpression(s3_1);
    boost::shared_ptr<const CEValues> cev3_1 = pe3_1->evalCE(moContext, avContext);
    //TEST_EQ(cev3_1->getInnerSVStride(), 4);
    //TEST_EQ(cev3_1->getAVDependencies().getNbConfigurations(), 4);
    //TEST_EQ(cev3_1->getAVDependencies().getNbConfigurations(3), 4);
    //TEST_EQ(cev3_1->getSVDependencies().getNbConfigurations(), 1);
    vb.clear();
    for(size_t i=0; i<4; ++i)
    {
        vb.setAVIndex(3, i);
        double v3_1 = v3[i];
        if( v3_1 < 11 )
            v3_1 = 11;
        TEST_EQ_DBL(cev3_1->getValue(vb), v3_1, 1e-10);
    }
    
    std::stringstream ss3_2;
    ss3_2 <<     "<BinOp op=\"Add\">";
    ss3_2 <<         "<AVRef uid=\"1\"/>";
    ss3_2 <<         "<MORef uid=\"123\"/>";
    ss3_2 <<     "</BinOp>";
    std::string s3_2(ss3_2.str());
    boost::shared_ptr<const PayoutExpression> pe3_2 = GenPDEParser::parsePayoutExpression(s3_2);
    boost::shared_ptr<const CEValues> cev3_2 = pe3_2->evalCE(moContext, avContext);
    //TEST_EQ(cev3_2->getInnerSVStride(), 5);
    //TEST_EQ(cev3_2->getAVDependencies().getNbConfigurations(), 5);
    //TEST_EQ(cev3_2->getAVDependencies().getNbConfigurations(1), 5);
    //TEST_EQ(cev3_2->getSVDependencies().getNbConfigurations(), 105);
    //TEST_EQ(cev3_2->getSVDependencies().getNbConfigurations(1), 3);
    //TEST_EQ(cev3_2->getSVDependencies().getNbConfigurations(2), 5);
    //TEST_EQ(cev3_2->getSVDependencies().getNbConfigurations(3), 7);
    vb.clear();
    for(size_t i=0; i<5; ++i)
    {
        vb.setAVIndex(1, i);
        for(size_t x=0; x<3; ++x)
        {
            vb.setSVIndex(1, x);
            for(size_t y=0; y<5; ++y)
            {
                vb.setSVIndex(2, y);
                for(size_t z=0; z<7; ++z)
                {
                    vb.setSVIndex(3, z);
                    size_t i3_2 = cev3_2->getIndex(vb);
                    TEST_EQ(i3_2, i + 5 * ( x + 3 * y + 15 * z));
                    double mo123 = 3 * ((double) x) + 2 * ((double) y) - ((double) z);
                    double v3_2  = mo123 + v1[i];
                    TEST_EQ_DBL(cev3_2->getValue(vb), v3_2, 1e-10);
                }
            }
        }
    }
    
    std::stringstream ss3_3;
    ss3_3 <<     "<BinOp op=\"Add\">";
    ss3_3 <<         "<BinOp op=\"Max\">";
    ss3_3 <<             "<AVRef uid=\"3\"/>";
    ss3_3 <<             "<Const value=\"11\"/>";
    ss3_3 <<         "</BinOp>";
    ss3_3 <<         "<MORef uid=\"2\"/>";
    ss3_3 <<     "</BinOp>";
    std::string s3_3(ss3_3.str());
    boost::shared_ptr<const PayoutExpression> pe3_3 = GenPDEParser::parsePayoutExpression(s3_3);
    boost::shared_ptr<const CEValues> cev3_3 = pe3_3->evalCE(moContext, avContext);
    //TEST_EQ(cev3_3->getInnerSVStride(), 4);
    //TEST_EQ(cev3_3->getAVDependencies().getNbConfigurations(), 4);
    //TEST_EQ(cev3_3->getAVDependencies().getNbConfigurations(3), 4);
    //TEST_EQ(cev3_3->getSVDependencies().getNbConfigurations(), 5);
    //TEST_EQ(cev3_3->getSVDependencies().getNbConfigurations(2), 5);
    vb.clear();
    for(size_t i=0; i<4; ++i)
    {
        vb.setAVIndex(3, i);
        for(size_t x=0; x<5; ++x)
        {
            vb.setSVIndex(2, x);
            double mo2 = (double) (2*x);
            double av3 = v3[i];
            double aux = ( av3 >= 11 ) ? av3 : 11;
            double v3_3 = aux + mo2;
            TEST_EQ_DBL(cev3_3->getValue(vb), v3_3, 1e-10);
        }
    }
    
    std::string s3(ss3.str());
    boost::shared_ptr<const PayoutExpression> pe3 = GenPDEParser::parsePayoutExpression(s3);
    boost::shared_ptr<const CEValues> cev3 = pe3->evalCE(moContext, avContext);
    //TEST_EQ(cev3->getInnerSVStride(), 20);
    //TEST_EQ(cev3->getAVDependencies().getNbConfigurations(), 20);
    //TEST_EQ(cev3->getAVDependencies().getNbConfigurations(1), 5);
    //TEST_EQ(cev3->getAVDependencies().getNbConfigurations(3), 4);
    //TEST_EQ(cev3->getSVDependencies().getNbConfigurations(), 105);
    //TEST_EQ(cev3->getSVDependencies().getNbConfigurations(1), 3);
    //TEST_EQ(cev3->getSVDependencies().getNbConfigurations(2), 5);
    //TEST_EQ(cev3->getSVDependencies().getNbConfigurations(3), 7);
 
    for(size_t i=0; i<5; ++i)
    {
        vb.setAVIndex(1, i);
        for(size_t j=0; j<4; ++j)
        {
            vb.setAVIndex(3, j);
            for(size_t x=0; x<3; ++x)
            {
                vb.setSVIndex(1, x);
                for(size_t y=0; y<5; ++y)
                {
                    vb.setSVIndex(2, y);
                    for(size_t z=0; z<7; ++z)
                    {
                        vb.setSVIndex(3, z);
                        double mo2   = (double) (2 * y);
                        double mo123 = 3 * ((double) x) + 2 * ((double) y) - ((double) z);
                        double av1   = v1[i];
                        double av3   = v3[j];
                        double maxv  = (av3 >= 11) ? av3 : 11;
                        double v3_   = (maxv + mo2) * (av1 + mo123);
                        TEST_EQ(cev3->getIndex(vb), i + 5 * (j + 4 * (x + 3 * (y + 5 * z))));
                        TEST_EQ_DBL(cev3->getValue(vb), v3_, 1e-10);
                    }
                }
            }
        }
    }
}
