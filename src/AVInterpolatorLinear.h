//
//  AVInterpolatorLinear.h
//  GenPDE
//
//  Created by Regis Dupont on 8/30/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef AVINTERPOLATORLINEAR_H_
#define AVINTERPOLATORLINEAR_H_

#include "AVInterpolatorInterface.h"
#include "InterpolationEnums.h"
#include "PEOperators.h"

class AVInterpolatorLinear : public AVInterpolatorInterface
{
public:
    AVInterpolatorLinear(
        const double*             x_begin,
        size_t                    nb_points,
        GenPDE::ExtrapolationType extrapolation_type
    )
    :mX(x_begin)
    ,mSize(nb_points)
    ,mExtrapolationType(extrapolation_type)
    ,mInvDX(nb_points)
    {
        Exception::check(nb_points > 1, "AVInterpolatorLinear::AVInterpolatorLinear", "Needs at least 2 points");
        for(size_t i=0; i<nb_points-1; ++i)
            mInvDX[i] = 1.0 / (mX[i+1] - mX[i]);
    }
    
    void updateY(const double* y_begin)
    {
        mY = y_begin;
    }
    
    inline double interpolate(double x) const
    {
        if( x <= mX[0] )
        {
            switch (mExtrapolationType)
            {
            case GenPDE::ExtrapolationType_None:
                Exception::raise("AVInterpolatorLinear", "No extrapolation allowed");
            case GenPDE::ExtrapolationType_Flat:
                return mY[0];
            case GenPDE::ExtrapolationType_Linear:
                return mInvDX[0] * ((x - mX[0]) * mY[1] + (mX[1] - x) * mY[0]);
            default:
                Exception::raise("AVInterpolatorLinear", "Unknown extrapolation type");
            }
        }
        if( x >= mX[mSize-1] )
        {
            switch (mExtrapolationType)
            {
            case GenPDE::ExtrapolationType_None:
                Exception::raise("AVInterpolatorLinear", "No extrapolation allowed");
            case GenPDE::ExtrapolationType_Flat:
                return mY[mSize-1];
            case GenPDE::ExtrapolationType_Linear:
                return mInvDX[mSize-2] * ((x - mX[mSize-2]) * mY[mSize-1] + (mX[mSize-1] - x) * mY[mSize-2]);
            default:
                Exception::raise("AVInterpolatorLinear", "Unknown extrapolation type");
            }
        }
        
        size_t kLow(0);
        size_t kHigh(mSize - 1);
        while ( kHigh - kLow > 1 )
        {
            size_t k = (kHigh + kLow) >> 1;
            if ( mX[k] > x )
                kHigh = k;
            else
                kLow = k;
        }
        
        return mInvDX[kLow] * ((x - mX[kLow]) * mY[kHigh] + (mX[kHigh] - x) * mY[kLow]);
    }
    
protected:
    const double*             mX;
    const double*             mY;
    size_t                    mSize;
    GenPDE::ExtrapolationType mExtrapolationType;
    std::vector<double>       mInvDX;
};

template <GenPDE::UnaryOperator OP = GenPDE::Id>
class AVInterpolatorLinearUniform : public AVInterpolatorInterface
{
public:
    AVInterpolatorLinearUniform(
        const double*             x_begin,
        size_t                    nb_points,
        GenPDE::ExtrapolationType extrapolation_type
    )
    :mSize(nb_points)
    ,mExtrapolationType(extrapolation_type)
    {
        Exception::check(nb_points > 1, "AVInterpolatorLinear::AVInterpolatorLinear", "Needs at least 2 points");
        mX0     = GenPDE::UnaryOperation<OP>::apply(x_begin[0]);
        mXLast  = GenPDE::UnaryOperation<OP>::apply(x_begin[mSize-1]);
        mInvDX  = ((double) (mSize - 1)) / (mXLast - mX0);
    }
    
    void updateY(const double* y_begin)
    {
        mY = y_begin;
    }
    
    inline double interpolate(double x) const
    {
        x = GenPDE::UnaryOperation<OP>::apply(x);
        if( x <= mX0 )
        {
            switch (mExtrapolationType)
            {
            case GenPDE::ExtrapolationType_None:
                Exception::raise("AVInterpolatorLinear", "No extrapolation allowed");
            case GenPDE::ExtrapolationType_Flat:
                return mY[0];
            case GenPDE::ExtrapolationType_Linear:
            {
                double w = mInvDX * (x - mX0);
                return w * mY[1] + (1-w) * mY[0];
            }
            default:
                Exception::raise("AVInterpolatorLinear", "Unknown extrapolation type");
            }
        }
        if( x >= mXLast )
        {
            switch (mExtrapolationType)
            {
            case GenPDE::ExtrapolationType_None:
                Exception::raise("AVInterpolatorLinear", "No extrapolation allowed");
            case GenPDE::ExtrapolationType_Flat:
                return mY[mSize-1];
            case GenPDE::ExtrapolationType_Linear:
            {
                double w = mInvDX * (mXLast - x);
                return w * mY[mSize-2] + (1-w) * mY[mSize-1];
            }
            default:
                Exception::raise("AVInterpolatorLinear", "Unknown extrapolation type");
            }
        }
        
        size_t kLow = (size_t) ((x - mX0) * mInvDX);
        double w    = mInvDX * (x - mX0) - ((size_t) kLow);
        return w * mY[kLow+1] + (1-w) * mY[kLow];
    }
    
protected:
    size_t                    mSize;
    GenPDE::ExtrapolationType mExtrapolationType;
    double                    mX0;
    double                    mXLast;
    double                    mInvDX;
    const double*             mY;
};

#endif // AVINTERPOLATORLINEAR_H_