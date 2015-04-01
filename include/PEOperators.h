//
//  PEOperators.h
//  GenPDE
//
//  Created by Regis Dupont on 8/20/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef PEOPERATORS_H_
#define PEOPERATORS_H_

#include <iostream>

#include "Framework.h"

namespace GenPDE
{
    enum BinaryOperator
    {
        Add,
        Sub,
        Mul,
        Div,
        Min,
        Max
    };
    
    enum UnaryOperator
    {
        Id,
        Neg,
        Log
    };
    
    enum ComparisonOperator
    {
        LT,
        LE,
        GT,
        GE,
        EQ,
        NE
    };
    
    template <BinaryOperator OP>
    struct BinaryOperation
    {
        inline static double apply(double x, double y)
        {
            Exception::notImplemented("BinaryOperation::apply");
            return 0.0;
        }
    };
    
    template <>
    inline double BinaryOperation<Add>::apply(double x, double y) { return x + y; }
    
    template <>
    inline double BinaryOperation<Sub>::apply(double x, double y) { return x - y; }
    
    template <>
    inline double BinaryOperation<Mul>::apply(double x, double y) { return x * y; }
    
    template <>
    inline double BinaryOperation<Div>::apply(double x, double y) { return x / y; }
    
    template <>
    inline double BinaryOperation<Max>::apply(double x, double y) { return (x >= y) ? x : y; }
    
    template <>
    inline double BinaryOperation<Min>::apply(double x, double y) { return (x >= y) ? y : x; }
    
    template <UnaryOperator OP>
    struct UnaryOperation
    {
        inline static double apply(double x)
        {
            Exception::notImplemented("UnaryOperation::apply");
            return 0.0;
        }
    };
    
    template <>
    inline double UnaryOperation<Neg>::apply(double x) { return -x; }
    
    template <>
    inline double UnaryOperation<Id>::apply(double x) { return x; }
    
    template <>
    inline double UnaryOperation<Log>::apply(double x) { return Double::log(x); }
    
    template <ComparisonOperator OP>
    struct ComparisonOperation
    {
        inline static double apply(double x, double y)
        {
            Exception::notImplemented("ComparisonOperation::apply");
            return 0.0;
        }
    };
    
    template <>
    inline double ComparisonOperation<LT>::apply(double x, double y) { return (x <  y) ? 1.0 : 0.0; }
    
    template <>
    inline double ComparisonOperation<LE>::apply(double x, double y) { return (x <= y) ? 1.0 : 0.0; }
    
    template <>
    inline double ComparisonOperation<GT>::apply(double x, double y) { return (x >  y) ? 1.0 : 0.0; }
    
    template <>
    inline double ComparisonOperation<GE>::apply(double x, double y) { return (x >= y) ? 1.0 : 0.0; }
    
    template <>
    inline double ComparisonOperation<EQ>::apply(double x, double y) { return (x == y) ? 1.0 : 0.0; }
    
    template <>
    inline double ComparisonOperation<NE>::apply(double x, double y) { return (x != y) ? 1.0 : 0.0; }
};
    
std::ostream& operator<<(std::ostream& stream, const GenPDE::BinaryOperator& op);
    
std::ostream& operator<<(std::ostream& stream, const GenPDE::UnaryOperator& op);


#endif // PEOPERATORS_H_
