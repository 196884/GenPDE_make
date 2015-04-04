#ifndef INTERPOLATION_ENUMS_H
#define INTERPOLATION_ENUMS_H

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

#endif // INTERPOLATION_ENUMS_H
