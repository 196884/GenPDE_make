//
//  AVInterpolatorNaturalCubic.h
//  GenPDE
//
//  Created by Regis Dupont on 8/30/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef AVINTERPOLATORNATURALCUBIC_H_
#define AVINTERPOLATORNATURALCUBIC_H_

#include "InterpolationEnums.h"
#include "AVInterpolatorInterface.h"
#include "PEOperators.h"

class AVInterpolatorNaturalCubic : public AVInterpolatorInterface
{
public:
    AVInterpolatorNaturalCubic(
        const double*             x,
        size_t                    n,
        GenPDE::ExtrapolationType extrapolation_type)
        :mSize(n)
        ,mX(x)
        ,mExtrapolationType(extrapolation_type)
        ,mY2(n)
        ,mAuxY2(n)
        ,mInvDX(n)
        ,mInvDX2_6(n)
        ,mSig(n)
    {
        Exception::check(n > 1, "AVInterpolatorNaturalCubic::AVInterpolatorNaturalCubic", "Needs at least 2 points");
        mY = 0;
        computeXData();
    }
    
    void updateY(const double* y)
    {
        if( y == mY )
            return;
        mY = y;
        computeY2();
        computeAB();
    }

    inline double interpolate(double x) const
    {
        if ( x <= mX[0] )
        {
            switch (mExtrapolationType)
            {
            case GenPDE::ExtrapolationType_Flat:
                return mY[0];
            case GenPDE::ExtrapolationType_Linear:
                return mABegin * x + mBBegin;
            default:
                Exception::raise("AVInterpolatorNaturalCubic::interpolate", "Extrapolation not handled");
            }
        }
        if ( x >= mX[mSize-1] )
        {
            switch (mExtrapolationType)
            {
            case GenPDE::ExtrapolationType_Flat:
                return mY[mSize-1];
            case GenPDE::ExtrapolationType_Linear:
                return mAEnd * x + mBEnd;
            default:
                Exception::raise("AVInterpolatorNaturalCubic::interpolate", "Extrapolation not handled");
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
        double h(mX[kHigh] - mX[kLow]);
        double invH(mInvDX[kLow]);
        double a((mX[kHigh] - x) * invH);
        double b((x - mX[kLow]) * invH);
        return a * mY[kLow]  +
               b * mY[kHigh] +
               (a*(a*a-1.0)*mY2[kLow] + b*(b*b-1.0)*mY2[kHigh])*(h*h) / 6.0;
    }

protected:
    void computeAB()
    {
        switch (mExtrapolationType)
        {
        case GenPDE::ExtrapolationType_Flat:
            break;
        case GenPDE::ExtrapolationType_Linear:
            mABegin = (mY[1] - mY[0]) / (mX[1] - mX[0]);
            mBBegin = mY[0] - mABegin * mX[0];
            mAEnd  = (mY[mSize - 1] - mY[mSize - 2]) / (mX[mSize - 1] - mX[mSize - 2]);
            mBEnd  = mY[mSize - 2] - mAEnd * mX[mSize - 2];
            break;
        case GenPDE::ExtrapolationType_None:
            break;
        default:
            Exception::raise("AVInterpolatorNaturalCubic::computeAB", "Unknown extrapolation type");
        }
    }

    void computeXData()
    {
        // mDX[i]     = mX[i+1] - mX[i];
        // mInvDX[i]  = 1 / mDX[i];
        // mInvDX2[i] = 1 / (mX[i+2] - mX[i]);
        const double* xPtr        = &mX[0];
        double*       invDxPtr    = &mInvDX[0];
        double*       invDx2_6Ptr = &mInvDX2_6[0];
        double*       sigPtr      = &mSig[0];
        for(size_t i=0; i<mSize-1; ++i)
        {
            double dx       = xPtr[1] - *xPtr;
            *invDxPtr++     = 1.0 / dx;
            *invDx2_6Ptr    = 1.0 / (xPtr[2] - *xPtr);
            *sigPtr++       = dx * *invDx2_6Ptr;
            *invDx2_6Ptr++ *= 6.0;
            ++xPtr;
        }
    }

    void computeY2()
    {
        double*       auxY2Ptr    = &mAuxY2[0];
        double*       y2Ptr       = &mY2[0];
        auxY2Ptr[0]               = 0.0;
        y2Ptr[0]                  = 0.0;
        const double* invDxPtr    = &mInvDX[0];
        const double* invDx2_6Ptr = &mInvDX2_6[0];
        const double* sigPtr      = &mSig[0];
        double sl((mY[1] - mY[0]) * *invDxPtr++);
        for (size_t i=1; i<mSize-1; ++i)
        {
            const double invP(1.0 / (*sigPtr * y2Ptr[i-1] + 2.0));
            y2Ptr[i]     = (*sigPtr - 1.0) * invP;
            auxY2Ptr[i]  = - sl;
            sl           = (mY[i+1] - mY[i]) * *invDxPtr++;
            auxY2Ptr[i] += sl;
            auxY2Ptr[i]  = (auxY2Ptr[i] * *invDx2_6Ptr++) - *sigPtr++ * auxY2Ptr[i-1] * invP;
        }
        y2Ptr[mSize - 1] = 0;
        for (int k=((int) mSize)-2; k>=0; --k)
            y2Ptr[k] = y2Ptr[k] * y2Ptr[k+1] + auxY2Ptr[k];
    }

protected:
	size_t                    mSize;
	const double*             mX;
	const double*             mY;
	GenPDE::ExtrapolationType mExtrapolationType;

	std::vector<double>       mY2;
    std::vector<double>       mAuxY2;
    std::vector<double>       mInvDX;    // mInvDX[i]    = 1 / mDX[i];
    std::vector<double>       mInvDX2_6; // mInvDX2_6[i] = 6 / (mX[i+2] - mX[i]);
    std::vector<double>       mSig;      // mSig[i]      = mDX[i] * mInvDX2[i];
	double                    mABegin;
	double                    mBBegin;
	double                    mAEnd;
	double                    mBEnd;
};

template <GenPDE::UnaryOperator OP = GenPDE::Id>
class AVInterpolatorNaturalCubicUniform : public AVInterpolatorInterface
{
public:
    AVInterpolatorNaturalCubicUniform(
        const double*             x,
        size_t                    n,
        GenPDE::ExtrapolationType extrapolation_type)
        :mSize(n)
        ,mExtrapolationType(extrapolation_type)
        ,mY2(n)
        ,mAuxY2(n)
    {
        Exception::check(n > 1, "AVInterpolatorNaturalCubicUniform::AVInterpolatorNaturalCubicUniform", "Needs at least 2 points");
        mX0       = GenPDE::UnaryOperation<OP>::apply(*x);
        mXLast    = GenPDE::UnaryOperation<OP>::apply(x[n-1]);
        mDX       = (mXLast - mX0) / ((double) (n-1));
        mDX2      = mDX * mDX;
        mInvDX    = 1.0 / mDX;
        mInv6     = 1.0 / 6.0;
        
        mY = 0;
    }
    
    void updateY(const double* y)
    {
        if( y == mY )
            return;
        mY = y;
        computeY2();
    }

    inline double interpolate(double x) const
    {
        x = GenPDE::UnaryOperation<OP>::apply(x);
        if ( x <= mX0 )
        {
            switch (mExtrapolationType)
            {
            case GenPDE::ExtrapolationType_Flat:
                return mY[0];
            case GenPDE::ExtrapolationType_Linear:
            {
                double w = mInvDX * (x - mX0);
                return w * mY[1] + (1-w) * mY[0];
            }
            default:
                Exception::raise("AVInterpolatorNaturalCubicUniform::interpolate", "Extrapolation not handled");
            }
        }
        if ( x >= mXLast )
        {
            switch (mExtrapolationType)
            {
            case GenPDE::ExtrapolationType_Flat:
                return mY[mSize-1];
            case GenPDE::ExtrapolationType_Linear:
            {
                double w = mInvDX * (mXLast - x);
                return w * mY[mSize-2] + (1-w) * mY[mSize-1];
            }
            default:
                Exception::raise("AVInterpolatorNaturalCubicUniform::interpolate", "Extrapolation not handled");
            }
        }
        
        size_t kLow = (size_t) (mInvDX * (x - mX0));
        double dL   = x - mX0 - kLow * mDX;
        double dH   = mDX - dL;
        return mInvDX * ( dH * mY[kLow] + dL * mY[kLow+1] +
                          mInv6 * ( dH * (dH * dH - mDX2) * mY2[kLow] + dL * (dL * dL - mDX2) * mY2[kLow+1] ) );
    }

protected:
    void computeY2()
    {
        double*       auxY2Ptr    = &mAuxY2[0];
        double*       y2Ptr       = &mY2[0];
        auxY2Ptr[0]               = 0.0;
        y2Ptr[0]                  = 0.0;
        double sl((mY[1] - mY[0]) * mInvDX);
        for (size_t i=1; i<mSize-1; ++i)
        {
            const double aux(-1.0 / (y2Ptr[i-1] + 4.0));
            y2Ptr[i]     = aux;
            auxY2Ptr[i]  = - sl;
            sl           = (mY[i+1] - mY[i]) * mInvDX;
            auxY2Ptr[i] += sl;
            auxY2Ptr[i]  = (3 * auxY2Ptr[i] * mInvDX) + aux * auxY2Ptr[i-1];
        }
        y2Ptr[mSize - 1] = 0;
        for (int k=((int) mSize)-2; k>=0; --k)
            y2Ptr[k] = y2Ptr[k] * y2Ptr[k+1] + auxY2Ptr[k];
    }

protected:
	size_t                    mSize;
    double                    mX0;
    double                    mXLast;
    double                    mDX;
    double                    mDX2;
    double                    mInvDX;
    double                    mInvDX2;
    double                    mInv6;
    
	const double*             mY;
	GenPDE::ExtrapolationType mExtrapolationType;

	std::vector<double>       mY2;
    std::vector<double>       mAuxY2;
};

#endif // AVINTERPOLATORNATURALCUBIC_H_