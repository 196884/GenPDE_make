/*
 * MarkovFunctionalDistribution.h
 *
 *  Created on: Nov 17, 2009
 *      Author: dupontr
 */

#ifndef MARKOVFUNCTIONALDISTRIBUTION_H_
#define MARKOVFUNCTIONALDISTRIBUTION_H_

#include <boost/shared_ptr.hpp>
#include "VolatilityParametrization.h"

class MarkovFunctionalDistribution
{
public:
	/// Constructor
	MarkovFunctionalDistribution(
		double tenor,
		const boost::shared_ptr<VolatilityParametrization>& vol_parametrization,
		size_t nb_sampling_points,
		double bounds_accuracy,
		double bounds_stepping,
		size_t bounds_max_iterations);

	double evaluatePsiInverse(double u) const;

	double evaluatePsi(double x) const;

private:
	void computeWorkingBoundaries(double& lower_boundary, double& upper_boundary) const;

private:
	double              mTenor;
	double              mSqrtTenor;
	boost::shared_ptr<VolatilityParametrization> mVolParametrization;

	std::vector<double> mBasePoints;
	std::vector<double> mAnchorValues;
	std::vector<double> mSlopes;
	double              mLowerValue;
	double              mUpperValue;
};

#endif /* MARKOVFUNCTIONALDISTRIBUTION_H_ */
