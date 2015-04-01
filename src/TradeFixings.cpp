//
//  TradeFixings.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/31/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "TradeFixings.h"

boost::shared_ptr<const TradeFixings> TradeFixings::NoFixings = boost::shared_ptr<TradeFixings>(new TradeFixings());
