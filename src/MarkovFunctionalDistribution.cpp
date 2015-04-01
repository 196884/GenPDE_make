/*
 * MarkovFunctionalDistribution.cpp
 *
 *  Created on: Nov 17, 2009
 *      Author: dupontr
 */

#include "Framework.h"

#include "MarkovFunctionalDistribution.h"

#include <iostream>
using namespace std;

using std::vector;

MarkovFunctionalDistribution::MarkovFunctionalDistribution(
	double tenor,
	const boost::shared_ptr<VolatilityParametrization>& vol_parametrization,
	size_t nb_sampling_points,
	double bounds_accuracy,
	double bounds_stepping,
	size_t bounds_max_iterations)
:mTenor(tenor)
,mSqrtTenor(Double::sqrt(tenor))
,mVolParametrization(vol_parametrization)
{
	// We determine working boundaries xLow and xHigh:
	double xLow(0.0);
	size_t nbIts(0);
	double halfAccuracy(0.5 * bounds_accuracy);
	for (nbIts=0;
		 nbIts<bounds_max_iterations && evaluatePsi(xLow) > halfAccuracy;
		 ++nbIts, xLow-=bounds_stepping);
	if ( nbIts == bounds_max_iterations )
		Exception::raise("MarkovFunctionalDistribution::CTOR", "could not find a lower working bound");
	double xHigh(0.0);
	for (nbIts=0;
	     nbIts<bounds_max_iterations && evaluatePsi(xHigh) < (1 - halfAccuracy);
	     ++nbIts, xHigh+=bounds_stepping);
	if ( nbIts == bounds_max_iterations )
		Exception::raise("MarkovFunctionalDistribution::CTOR", "could not find a higher working bound");
	if ( xLow == xHigh )
		Exception::raise("MarkovFunctionalDistribution::CTOR", "xLow found equal to xHigh, not possible to continue");
	vector<double> xPoints(nb_sampling_points);
	mBasePoints.resize(nb_sampling_points);
	double dx((xHigh - xLow) / static_cast<double>(nb_sampling_points - 1));
	for (size_t i=0; i<nb_sampling_points; ++i)
	{
		xPoints[i] = xLow + i * dx;
		mBasePoints[i] = evaluatePsi(xPoints[i]);
	}
	for (size_t i=1; i<nb_sampling_points; ++i)
	{
		if ( mBasePoints[i] <= mBasePoints[i-1] )
			Exception::raise("MarkovFunctionalDistribution::CTOR", "the Psi function is not strictly increasing");
	}
	mAnchorValues.resize(nb_sampling_points - 1);
	mSlopes.resize(nb_sampling_points - 1);
	for (size_t i=0; i<nb_sampling_points-1; ++i)
	{
		mSlopes[i] = (xPoints[i+1] - xPoints[i]) / (mBasePoints[i+1] - mBasePoints[i]);
		mAnchorValues[i] = xPoints[i] - mSlopes[i] * mBasePoints[i];
	}
	mLowerValue = xPoints.front();
	mUpperValue = xPoints.back();
	/*
	for (size_t i=0; i<nb_sampling_points; ++i)
		cerr << i << " -> " << mBasePoints[i] << endl;
		*/
}

double MarkovFunctionalDistribution::evaluatePsi(double x) const
{
	double stdDev(mSqrtTenor * mVolParametrization->evaluateVolatility(x));
	double d1(-x / stdDev + 0.5 * stdDev);
	double minusD2(stdDev - d1);
	double volDiff(mVolParametrization->evaluateVolatilityDerivative(x));
	return Double::normalCdf(minusD2) + mSqrtTenor * Double::normalPdf(d1) * Double::exp(-x) * volDiff;
}

double MarkovFunctionalDistribution::evaluatePsiInverse(double y) const
{
	if ( y <= mBasePoints.front() )
		return mLowerValue;
	if ( y >= mBasePoints.back() )
		return mUpperValue;

	size_t kLow(0);
	size_t kHigh(mBasePoints.size() - 1);
	while ( kHigh - kLow > 1 )
	{
		size_t k = (kHigh + kLow) >> 1;
		if ( mBasePoints[k] > y )
			kHigh = k;
		else
			kLow = k;
	}
	return mAnchorValues[kLow] + mSlopes[kLow] * y;
}
