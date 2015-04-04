#include "Framework.h"

#include "CubicSplineInterpolator.h"

using std::vector;

CubicSplineInterpolator::CubicSplineInterpolator(
	const double* x,
	const double* y,
    size_t        n,
	ExtrapolationType          extrapolation_type)
	:mSize(n)
	,mX(x)
	,mY(y)
	,mExtrapolationType(extrapolation_type)
	,mY2(n)
    ,mAuxY2(n)
{
	computeY2();
    computeAB();
}

CubicSplineInterpolator::CubicSplineInterpolator(
	const double* x,
    size_t        n,
	ExtrapolationType          extrapolation_type)
	:mSize(n)
	,mX(x)
	,mExtrapolationType(extrapolation_type)
	,mY2(n)
    ,mAuxY2(n)
{}

void CubicSplineInterpolator::computeAB()
{
	if ( mExtrapolationType == ExtrapolateLinear )
	{
		mABegin = (mY[1] - mY[0]) / (mX[1] - mX[0]);
		mBBegin = mY[0] - mABegin * mX[0];

		mAEnd = (mY[mSize - 1] - mY[mSize - 2]) / (mX[mSize - 1] - mX[mSize - 2]);
		mBEnd = mY[mSize - 2] - mAEnd * mX[mSize - 2];
	}
}

void CubicSplineInterpolator::computeY2()
{
    double* auxY2Ptr = &mAuxY2[0];
    double* y2Ptr    = &mY2[0];
	auxY2Ptr[0] = 0.0;
	y2Ptr[0]    = 0.0;
	double dx(mX[1] - mX[0]);
	double sl((mY[1] - mY[0]) / dx);
	for (size_t i=1; i<mSize-1; ++i)
	{
		const double invD2(1.0 / (mX[i+1] - mX[i-1]));
		const double sig(dx * invD2);
		const double invP(1.0 / (sig * y2Ptr[i-1] + 2.0));
		y2Ptr[i]     = (sig - 1.0) * invP;
		dx           = mX[i+1] - mX[i];
		auxY2Ptr[i]  = - sl;
		sl           = (mY[i+1] - mY[i]) / dx;
		auxY2Ptr[i] += sl;
		auxY2Ptr[i]  = (6.0 * auxY2Ptr[i] * invD2) - sig * auxY2Ptr[i-1] * invP;
	}
	y2Ptr[mSize - 1] = 0;
	for (int k=((int) mSize)-2; k>=0; --k)
		y2Ptr[k] = y2Ptr[k] * y2Ptr[k+1] + auxY2Ptr[k];
}

void CubicSplineInterpolator::updateY(const double* y)
{
    mY = y;
    computeY2();
    computeAB();
}

double CubicSplineInterpolator::interpolate(double x) const
{
	if ( x <= mX[0] )
	{
		if ( mExtrapolationType == ExtrapolateFlat )
			return mY[0];
		else
			return mABegin * x + mBBegin;
	}
	if ( x >= mX[mSize-1] )
	{
		if ( mExtrapolationType == ExtrapolateFlat )
			return mY[mSize-1];
		else
			return mAEnd * x + mBEnd;
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
	double invH(1.0 / h);
	double a((mX[kHigh] - x) * invH);
	double b((x - mX[kLow]) * invH);
	return a * mY[kLow]  +
	       b * mY[kHigh] +
	       (a*(a*a - 1.0)*mY2[kLow] + b*(b*b-1.0)*mY2[kHigh])*(h*h) / 6.0;
}



CubicSplineInterpolatorCaching::CubicSplineInterpolatorCaching(
	const double* x,
	const double* y,
    size_t        n,
	ExtrapolationType          extrapolation_type)
	:mSize(n)
	,mX(x)
	,mY(y)
	,mExtrapolationType(extrapolation_type)
	,mY2(n)
    ,mAuxY2(n)
    ,mInvDX(n)
    ,mInvDX2_6(n)
    ,mSig(n)
    ,mXLower(x[0])
    ,mXUpper(x[n-1])
{
    computeXData();
	computeY2();
    computeAB();
}

CubicSplineInterpolatorCaching::CubicSplineInterpolatorCaching(
	const double* x,
    size_t        n,
	ExtrapolationType          extrapolation_type)
	:mSize(n)
	,mX(x)
	,mExtrapolationType(extrapolation_type)
	,mY2(n)
    ,mAuxY2(n)
    ,mInvDX(n)
    ,mInvDX2_6(n)
    ,mSig(n)
    ,mXLower(x[0])
    ,mXUpper(x[n-1])
{
    mY = 0;
    computeXData();
}

void CubicSplineInterpolatorCaching::computeAB()
{
    switch (mExtrapolationType)
    {
    case ExtrapolateFlat:
        break;
    case ExtrapolateLinear:
		mABegin = (mY[1] - mY[0]) / (mX[1] - mX[0]);
		mBBegin = mY[0] - mABegin * mX[0];
		mAEnd  = (mY[mSize - 1] - mY[mSize - 2]) / (mX[mSize - 1] - mX[mSize - 2]);
		mBEnd  = mY[mSize - 2] - mAEnd * mX[mSize - 2];
        break;
    case ExtrapolateNone:
        break;
    default:
        Exception::raise("", "Unknown extrapolation type");
    }
}

void CubicSplineInterpolatorCaching::computeXData()
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

void CubicSplineInterpolatorCaching::computeY2()
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

void CubicSplineInterpolatorCaching::updateY(const double* y)
{
    if( y == mY )
        return;
    mY = y;
    computeY2();
    computeAB();
}

double CubicSplineInterpolatorCaching::interpolate(double x) const
{
	if ( x <= mXLower )
	{
        switch (mExtrapolationType)
        {
        case ExtrapolateFlat:
            return mY[0];
        case ExtrapolateLinear:
            return mABegin * x + mBBegin;
        default:
            Exception::raise("CubicSplineInterpolatorCaching::interpolate", "Extrapolation not handled");
        }
	}
	if ( x >= mXUpper )
	{
        switch (mExtrapolationType)
        {
        case ExtrapolateFlat:
            return mY[mSize-1];
        case ExtrapolateLinear:
            return mAEnd * x + mBEnd;
        default:
            Exception::raise("CubicSplineInterpolatorCaching::interpolate", "Extrapolation not handled");
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
