//
//  TradeLeg.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/26/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "TradeLeg.h"

std::ostream& operator<<(std::ostream& stream, const TradeLeg& trade_leg)
{
    trade_leg.print(stream);
    return stream;
}