//
//  VariableBindings.h
//  GenPDE
//
//  Created by Regis Dupont on 8/21/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef VARIABLEBINDINGS_H_
#define VARIABLEBINDINGS_H_

#include <map> 

#include "AuxiliaryVariable.h"
#include "StateVariable.h"
#include "VariableEnums.h"

class VarBindings
{
public:
    VarBindings() {}
    
    void setSVIndex(GenPDE::VariableUID svUid, size_t index) { mSVBindings[svUid] = index; }
    
    void setAVIndex(GenPDE::VariableUID avUid, size_t index) { mAVBindings[avUid] = index; }
    
    size_t getSVIndex(GenPDE::VariableUID svUid) const
    {
        std::map<GenPDE::VariableUID, size_t>::const_iterator it(mSVBindings.find(svUid));
        if( it == mSVBindings.end() )
            return 0;
        return it->second;
    }
    
    size_t getAVIndex(GenPDE::VariableUID avUid) const
    {
        std::map<GenPDE::VariableUID, size_t>::const_iterator it(mAVBindings.find(avUid));
        if( it == mAVBindings.end() )
            return 0;
        return it->second;
    }
    
    void clear()
    {
        mSVBindings.clear();
        mAVBindings.clear();
    }
private:
    /// Using std::map is quite inefficient, but this class it a convenience
    /// class and shouldn't be used in performance-sensitive situations
    std::map<GenPDE::VariableUID, size_t> mSVBindings;
    std::map<GenPDE::VariableUID, size_t> mAVBindings;
};

#endif // VARIABLEBINDINGS_H_