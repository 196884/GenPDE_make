//
//  AVDiscretization.h
//  GenPDE
//
//  Created by Regis Dupont on 8/31/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef AVDISCRETIZATION_H_
#define AVDISCRETIZATION_H_

namespace GenPDE
{
    enum AVDiscretizationType {
        AVDiscretizationType_FwdSpot,
        AVDiscretizationType_SpotSum,
        AVDiscretizationType_Boolean
    };
}

#endif AVDISCRETIZATION_H_