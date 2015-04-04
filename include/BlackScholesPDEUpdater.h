#ifndef BLACK_SCHOLES_PDE_UPDATER_H
#define BLACK_SCHOLES_PDE_UPDATER_H

#include "GpDate.h"
#include "PDEUpdater1D.h"
#include "PDESolver1DInterfaces.h"

/// \brief A naive Black-Scholes PDE updater, with normal ('gamma = 0')
/// boundary conditions
class BlackScholesPDEUpdater : public PDEUpdater1D
{
public:
	/// Constructor
	BlackScholesPDEUpdater(
		double risk_free_rate,
		double volatility)
	:mRiskFreeRate(risk_free_rate)
	,mVolatility(volatility)
	,mDone(false)
	{}

	/// Updater function
	void update(Solver1DInterface& solver)
	{
		if ( mDone )
			return;
		const double halfSqrVol(0.5 * mVolatility * mVolatility);
		const double diffCoef(-halfSqrVol);
		const double convCoef(halfSqrVol - mRiskFreeRate);
		const double varCoef(mRiskFreeRate);
		std::vector<double>& diffusionCoefs(solver.getDiffusionCoefs());
		std::fill(diffusionCoefs.begin(), diffusionCoefs.end(), diffCoef);
		std::vector<double>& convectionCoefs(solver.getConvectionCoefs());
		std::fill(convectionCoefs.begin(), convectionCoefs.end(), convCoef);
		std::vector<double>& variableCoefs(solver.getVariableCoefs());
		std::fill(variableCoefs.begin(), variableCoefs.end(), varCoef);
		// We take care of the 'gamma = 0' boundary condition:
		diffusionCoefs.front()  = 0;
		diffusionCoefs.back()   = 0;
		convectionCoefs.front() = -mRiskFreeRate;
		convectionCoefs.back()  = -mRiskFreeRate;
        // Since our coefs are / year:
        //solver.setTimeScaling(1.0 / ((double) GenPDE::NbDaysPerYear));
		mDone = true;
	}

private:
	const double mRiskFreeRate;
	const double mVolatility;
	bool         mDone;
};

#endif // BLACK_SCHOLES_PDE_UPDATER_H
