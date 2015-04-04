#include "Framework.h"
#include "TestSuite.h"

#include "MOContextTests.h"

MOContextTest::CEVConstPtr MOContextTest::evalCE(MOUid uid) const
{
    std::map<MOUid, CEVPtr>::const_iterator it = mValues.find(uid);
    if( it == mValues.end() )
        Exception::raise("MOContextTest::evalCE", "Unhandled market observable");
    return it->second;
}

typedef boost::shared_ptr<const CEValues> CEVPtr;

REGISTER_TEST(MOContextTest)
{
    MOContextTest moContext;
    VarBindings   vb;
    
    
    CEVPtr cev0 = moContext.evalCE(0);
    //TEST( cev0->getAVContext().getNbConfigurations() == 1 );
    //TEST( cev0->getSVContext().getNbConfigurations() == 1 );
    TEST_EQ_DBL(cev0->getValue(vb), 3, 1e-10);
    
    CEVPtr cev1 = moContext.evalCE(1);
    //TEST( cev1->getAVContext().getNbConfigurations() == 1 );
    //TEST( cev1->getSVContext().getNbConfigurations() == 3 );
    //TEST( cev1->getSVContext().getNbConfigurations(1) == 3 );
    for(size_t i=0; i<3; ++i)
    {
        vb.setSVIndex(1, i);
        TEST_EQ_DBL(cev1->getValue(vb), (double) i, 1e-10);
    }
    
    CEVPtr cev2 = moContext.evalCE(2);
    //TEST( cev2->getAVContext().getNbConfigurations() == 1 );
    //TEST( cev2->getSVContext().getNbConfigurations() == 5 );
    //TEST( cev2->getSVContext().getNbConfigurations(2) == 5 );
    for(size_t i=0; i<5; ++i)
    {
        vb.setSVIndex(2, i);
        TEST_EQ_DBL(cev2->getValue(vb), (double) (2*i), 1e-10);
    }
    
    CEVPtr cev3 = moContext.evalCE(3);
    //TEST( cev3->getAVContext().getNbConfigurations() == 1 );
    //TEST( cev3->getSVContext().getNbConfigurations() == 7 );
    //TEST( cev3->getSVContext().getNbConfigurations(3) == 7 );
    for(size_t i=0; i<7; ++i)
    {
        vb.setSVIndex(3, i);
        TEST_EQ_DBL(cev3->getValue(vb), (double) (3*i), 1e-10);
    }
    
    CEVPtr cev12 = moContext.evalCE(12);
    //TEST( cev12->getAVContext().getNbConfigurations() == 1 );
    //TEST( cev12->getSVContext().getNbConfigurations() == 15 );
    //TEST( cev12->getSVContext().getNbConfigurations(1) == 3 );
    //TEST( cev12->getSVContext().getNbConfigurations(2) == 5 );
    for(size_t i=0; i<3; ++i)
    {
        vb.setSVIndex(1, i);
        for(size_t j=0; j<5; ++j)
        {
            vb.setSVIndex(2, j);
            TEST_EQ_DBL(cev12->getValue(vb), 7*((double) i)-3*((double) j), 1e-10);
        }
    }
    
    CEVPtr cev123 = moContext.evalCE(123);
    //TEST( cev123->getAVDependencies().getNbConfigurations() == 1 );
    //TEST( cev123->getSVDependencies().getNbConfigurations() == 105 );
    //TEST( cev123->getSVDependencies().getNbConfigurations(1) == 3 );
    //TEST( cev123->getSVDependencies().getNbConfigurations(2) == 5 );
    //TEST( cev123->getSVDependencies().getNbConfigurations(3) == 7 );
    for(size_t i=0; i<3; ++i)
    {
        vb.setSVIndex(1, i);
        for(size_t j=0; j<5; ++j)
        {
            vb.setSVIndex(2, j);
            for(size_t k=0; k<7; ++k)
            {
                vb.setSVIndex(3, k);
                double v = 3 * ((double) i) + 2 * ((double) j) - ((double) k);
                TEST_EQ_DBL(cev123->getValue(vb), v, 1e-10);
            }
        }
    }
}
