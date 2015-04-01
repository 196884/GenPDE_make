//
//  AVContext.h
//  GenPDE
//
//  Created by Regis Dupont on 8/24/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef AVCONTEX_H_
#define AVCONTEX_H_

#include "Framework.h"

#include "AuxiliaryVariable.h"

class AVContext
{
protected:
    typedef boost::shared_ptr<AuxiliaryVariable>       AVPtr;
    typedef boost::shared_ptr<const AuxiliaryVariable> AVConstPtr;

public:
    typedef std::map<GenPDE::VariableUID, AVPtr>::iterator           AVIterator;
    
public:
    AVContext() {}
    
    AVContext(const std::vector<AVPtr>& avs)
    {
        for(const AVPtr& av : avs)
            mAVs[av->getUid()] = av;
    }
    
    virtual ~AVContext() {}
    
    void setAuxiliaryVariable(const AVPtr& av)
    {
        mAVs[av->getUid()] = av;
    }
    
    AVConstPtr getAuxiliaryVariable(GenPDE::VariableUID uid) const
    {
        std::map<GenPDE::VariableUID, AVPtr>::const_iterator it = mAVs.find(uid);
        if( it == mAVs.end() )
            Exception::raise("AVContext::getAuxiliaryVariable", "Could not find specified Auxiliary Variable");
        return it->second;
    }
    
    AVPtr getAuxiliaryVariable(GenPDE::VariableUID uid)
    {
        std::map<GenPDE::VariableUID, AVPtr>::const_iterator it = mAVs.find(uid);
        if( it == mAVs.end() )
            Exception::raise("AVContext::getAuxiliaryVariable", "Could not find specified Auxiliary Variable");
        return it->second;
    }
    
    AVIterator AVBegin() { return mAVs.begin(); }
    AVIterator AVEnd()   { return mAVs.end();   }
    
protected:
    std::map<GenPDE::VariableUID, AVPtr> mAVs;
};

#endif // AVCONTEX_H_