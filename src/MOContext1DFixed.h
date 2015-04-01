//
//  MOContext1DFixed.h
//  GenPDE
//
//  Created by Regis Dupont on 8/25/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef MOCONTEXT1DFIXED_H_
#define MOCONTEXT1DFIXED_H_

#include <vector>

#include "MOContext.h"

class MOContext1DFixed : public MOContext
{
public:
    MOContext1DFixed(MOUid uid, const std::vector<double>& values);
    
    virtual CEVConstPtr evalCE(MOUid uid) const
    {
        if( uid != mUid )
            Exception::raise("MOContext1DFixed::MOContext1DFixed", "Uid not supported");
        return mValues;
    }
    
protected:
    MOUid                       mUid;
    boost::shared_ptr<CEValues> mValues;
};

#endif // MOCONTEXT1DFIXED_H_
