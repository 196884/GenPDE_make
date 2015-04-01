//
//  VariableEnums.h
//  GenPDE
//
//  Created by Regis Dupont on 9/9/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef VARIABLEENUMS_H_
#define VARIABLEENUMS_H_

namespace GenPDE
{
    enum VariableType {
        VT_StateVariable,
        VT_AuxiliaryVariable
    };
    
    typedef size_t VariableUID;
}

#endif // VARIABLEENUMS_H_