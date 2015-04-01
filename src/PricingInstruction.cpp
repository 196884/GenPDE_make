//
//  PricingInstruction.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/26/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "PricingInstruction.h"

std::ostream& operator<<(std::ostream& stream, const PricingInstruction& pi)
{
    pi.print(stream);
    return stream;
}
