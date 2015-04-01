//
//  InterpolationEnums.h
//  GenPDE
//
//  Created by Regis Dupont on 8/30/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef INTERPOLATIONENUMS_H_
#define INTERPOLATIONENUMS_H_

namespace GenPDE
{
    enum ExtrapolationType
    {
        ExtrapolationType_None, // throws if trying to extrapolate
        ExtrapolationType_Flat,
        ExtrapolationType_Linear
    };
    
    enum InterpolatorType
    {
        InterpolatorType_Linear,
        InterpolatorType_NaturalCubic,
        InterpolatorType_LocalCubic
    };
}

#endif // INTERPOLATIONENUMS_H_