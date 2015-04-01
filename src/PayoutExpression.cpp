//
//  PayoutExpression.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/20/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "PayoutExpression.h"

std::ostream& operator<<(std::ostream& stream, const PayoutExpression& pe)
{
    pe.print(stream);
    return stream;
}