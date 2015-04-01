//
//  MOContext.h
//  GenPDE
//
//  Created by Regis Dupont on 8/22/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef MOCONTEXT_H_
#define MOCONTEXT_H_

#include <boost/shared_ptr.hpp>

#include "CEValuesStored.h"
#include "MarketObservable.h"

class MOContext
{
protected:
    typedef boost::shared_ptr<const CEValues> CEVConstPtr;
    
public:
    virtual CEVConstPtr evalCE(MOUid uid) const = 0;
};

#endif // MOCONTEXT_H_