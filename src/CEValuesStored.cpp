//
//  CEValuesStored.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/24/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "CEValuesStored.h"

CEValuesStored::CEValuesStored(
    const CEVConstPtr& cevs
)
:CEValues(cevs->getVarMemoryLayout())
,mData(mVarMemLayout.getSize())
{}

CEValuesStored::CEValuesStored(
    const CEVConstPtr& cevs1,
    const CEVConstPtr& cevs2
)
:CEValues(cevs1->getVarMemoryLayout(), cevs2->getVarMemoryLayout())
,mData(mVarMemLayout.getSize())
{}

CEValuesStored::CEValuesStored(
    const CEVConstPtr& cevs1,
    const CEVConstPtr& cevs2,
    const CEVConstPtr& cevs3
)
:CEValues(cevs1->getVarMemoryLayout(), cevs2->getVarMemoryLayout(), cevs3->getVarMemoryLayout())
,mData(mVarMemLayout.getSize())
{}