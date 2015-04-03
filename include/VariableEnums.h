#ifndef VARIABLE_ENUMS_H
#define VARIABLE_ENUMS_H

namespace GenPDE
{
    enum VariableType {
        VT_StateVariable,
        VT_AuxiliaryVariable
    };
    
    typedef size_t VariableUID;
}

#endif // VARIABLE_ENUMS_H
