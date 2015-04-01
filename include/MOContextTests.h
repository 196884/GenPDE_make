//
//  MOContextTests.h
//  GenPDE
//
//  Created by Regis Dupont on 8/22/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef MOCONTEXTTESTS_H_
#define MOCONTEXTTESTS_H_

#include <map>
#include <utility>
#include <vector>

#include "MOContext.h"

class MOContextTest : public MOContext
{
    typedef boost::shared_ptr<CEValues>       CEVPtr;
    typedef boost::shared_ptr<const CEValues> CEVConstPtr;
public:
    MOContextTest()
    {
        // Hardcoded:
        // SV1: 3 values
        // SV2: 5 values
        // SV3: 7 values
        // 0 depends on no SV (value: 3)
        // 1 depends on SV1 (values: SV1    )
        // 2 depends on SV2 (values: 2 * SV2)
        // 3 depends on SV3 (values: 3 * SV3)
        // 12 depends on SV1 and SV2 (7 * SV1 - 3 * SV2)
        // 123 depends on SV1, SV2 and SV3 (3 * SV1 + 2 * SV2 - SV3)
        VarDependencies sv1(GenPDE::VT_StateVariable, 1, 3);
        VarDependencies sv2(GenPDE::VT_StateVariable, 2, 5);
        VarDependencies sv3(GenPDE::VT_StateVariable, 3, 7);
        VarDependencies sv12(sv1, sv2);
        VarDependencies sv123(sv1, sv2, sv3);
        
        boost::shared_ptr<CEValues> cev0(new CEValuesStored(3));
        mValues[0] = cev0;
      
        boost::shared_ptr<CEValues> cev1(new CEValuesStored(sv1));
        double* cev1Ptr = cev1->getDataPtr();
        for(size_t i=0; i<3; ++i)
            cev1Ptr[i] = (double) i;
        mValues[1] = cev1;
        
        boost::shared_ptr<CEValues> cev2(new CEValuesStored(sv2));
        double* cev2Ptr = cev2->getDataPtr();
        for(size_t i=0; i<5; ++i)
            cev2Ptr[i] = 2 * ((double) i);
        mValues[2] = cev2;
        
        boost::shared_ptr<CEValues> cev3(new CEValuesStored(sv3));
        double* cev3Ptr = cev3->getDataPtr();
        for(size_t i=0; i<7; ++i)
            cev3Ptr[i] = 3 * ((double) i);
        mValues[3] = cev3;
        
        boost::shared_ptr<CEValues> cev12(new CEValuesStored(sv12));
        double* cev12Ptr = cev12->getDataPtr();
        for(size_t i=0; i<3; ++i)
            for(size_t j=0; j<5; ++j)
                cev12Ptr[i + 3 * j] = 7 * ((double) i) - 3 * ((double) j);
        mValues[12] = cev12;
        
        boost::shared_ptr<CEValues> cev123(new CEValuesStored(sv123));
        double* cev123Ptr = cev123->getDataPtr();
        for(size_t i=0; i<3; ++i)
            for(size_t j=0; j<5; ++j)
                for(size_t k=0; k<7; ++k)
                    cev123Ptr[i + 3*j + 15*k] = 3 * ((double) i) + 2 * ((double) j) - ((double) k);
        mValues[123] = cev123;
    }
    
    CEVConstPtr evalCE(MOUid uid) const;
    
protected:
    std::map<MOUid, CEVPtr> mValues;
};

#endif // MOCONTEXTTESTS_H_
