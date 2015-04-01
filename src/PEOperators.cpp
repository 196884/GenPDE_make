//
//  PEOperators.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/20/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "PEOperators.h"

#include "Framework.h"

std::ostream& operator<<(std::ostream& stream, const GenPDE::BinaryOperator& op)
{
    switch (op)
    {
    case GenPDE::Add:
        stream << "Add";
        break;
    case GenPDE::Sub:
        stream << "Sub";
        break;
    case GenPDE::Mul:
        stream << "Mul";
        break;
    case GenPDE::Div:
        stream << "Div";
        break;
    case GenPDE::Min:
        stream << "Min";
        break;
    case GenPDE::Max:
        stream << "Max";
        break;
    default:
        Exception::raise("operator<<", "Unknown GenPDE::BinaryOperator");
    }
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const GenPDE::UnaryOperator& op)
{
    switch (op)
    {
    case GenPDE::Id:
        stream << "Id";
        break;
    case GenPDE::Neg:
        stream << "Neg";
        break;
    case GenPDE::Log:
        stream << "Log";
        break;
    default:
        Exception::raise("operator<<", "Unknown GenPDE::UnaryOperator");
    }
    return stream;
}
