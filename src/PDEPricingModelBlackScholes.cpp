#include "Framework.h"
#include "PDEPricingModelBlackScholes.h"
#include "CubicSplineInterpolator.h"
#include "CEValuesStored.h"

void PDEPricingModelBlackScholes::setupSpaceGrid(const std::vector<double>& time_grid)
{
    const double tenor(time_grid.back() - time_grid.front());
	const double variance(mVolatility * mVolatility * tenor);
	const double stdDev(Double::sqrt(variance));
	const double lowerBoundary(Double::log(mSpot) - mSpaceGridNbStdDevs * stdDev);
	const double upperBoundary(Double::log(mSpot) + mSpaceGridNbStdDevs * stdDev);
	const double ds((upperBoundary - lowerBoundary) / static_cast<double>(mSpaceGridSize - 1));
    for (size_t i=0; i<mSpaceGridSize; ++i)
    {
    	mLogSpotGrid[i] = lowerBoundary + i * ds;
    	mSpotGrid[i]    = Double::exp(mLogSpotGrid[i]);
    }
}

void PDEPricingModelBlackScholes::setupForTrade(
    const std::vector<GenPDE::Date>& trade_dates,
    const AuxiliaryVariables&        auxiliary_variables,
    const MOFixingsIfc*              mo_fixings
)
{
    resetForTrade();
    m_moFixings = mo_fixings;
    setDeterministicAVs( *this, auxiliary_variables, m_avContext );
    m_moFixings = mo_fixings;
    switch( m_avDiscretizationPolicy->getType() )
    {
        case AVDiscretizationPolicy::Type_None:
            break;
        case AVDiscretizationPolicy::Type_Hardcoded:
            {
                AVDP_Hardcoded& avdp = static_cast<AVDP_Hardcoded&>( *m_avDiscretizationPolicy );
                avdp.setAVDiscretizations( auxiliary_variables, m_avContext );
            }
            break;
        case AVDiscretizationPolicy::Type_Sum:
            {
                const AVDP_Sum& avdp = static_cast<const AVDP_Sum&>( *m_avDiscretizationPolicy );
                AVDP::discretizeSum<PDEPricingModelBlackScholes>( avdp, auxiliary_variables, *this, m_avContext );
            }
            break;
        default:
            Exception::raise( "PDEPricingModelBlackScholes::setupForTrade", "unhandled AVDiscretizationPolicy" );
    };

    std::vector<double> timeGrid;
    setupTimeGrid(mPricingDate, trade_dates, timeGrid);
    setupSpaceGrid(timeGrid);
    mSolver                 = boost::shared_ptr<PDESolverInterfaceBase>(new PDESolver1D(
        mLogSpotGrid,
        timeGrid,
        this
    ));
}

double PDEPricingModelBlackScholes::getVariance(const GenPDE::Date& to_date) const
{
    if( to_date <= mPricingDate )
        return 0;
    double dfTenor(((double) GenPDE::dateDifferenceInDays(to_date, mCurrentDate)) / GenPDE::NbDaysPerYear);
    return mVolatility * mVolatility * dfTenor;
}

double PDEPricingModelBlackScholes::getSpot() const
{
    return mSpot;
}

PDEPricingModelBlackScholes::CEVConstPtr   PDEPricingModelBlackScholes::getFixing( MOUid mo_uid, const GenPDE::Date& date ) const
{
    CEVConstPtr result = m_moFixings->getFixing( mo_uid, date );
    if( result )
        return result;
    if( 1 == mo_uid && mPricingDate == date )
    {
        result = CEVConstPtr( new CEValuesStored( mSpot ) );
        return result;
    }
    return CEVConstPtr();
}
     
PDEPricingModelBlackScholes::CEVConstPtr   PDEPricingModelBlackScholes::discountFactorCE(const GenPDE::Date& to_date) const
{
    double df = 0;
    if( mCurrentDate >= mPricingDate )
    {
        double dfTenor(((double) GenPDE::dateDifferenceInDays(to_date, mCurrentDate)) / GenPDE::NbDaysPerYear);
        df = Double::exp(-mRiskFreeRate * dfTenor);
    } else if ( to_date >= mPricingDate )
    {
        double dfTenor(((double) GenPDE::dateDifferenceInDays(to_date, mPricingDate)) / GenPDE::NbDaysPerYear);
        df = Double::exp(-mRiskFreeRate * dfTenor);
    } else {
        df = 0;
    }
    CEVConstPtr result(new CEValuesStored(df));
    return result;
}

PDEPricingModelBlackScholes::CEVConstPtr   PDEPricingModelBlackScholes::marketObservableCE(MOUid uid) const
{
    if( uid != 1 )
        Exception::raise("PDEPricingModelBlackScholes::marketObservableCE", "Only MO with Uid 1 is supported by this model");
    
    CEVConstPtr fixing = m_moFixings->getFixing( uid, mCurrentDate );
    if( fixing )
        return fixing;

    if( mCurrentDate == mPricingDate )
        return boost::shared_ptr<CEValues>(new CEValuesStored(mSpot));
    
    Exception::check(
        mCurrentDate > mPricingDate,
        "PDEPricingModelBlackScholes::marketObservableCE",
        "Missing fixing"
    );
    
    CEVPtr result(boost::shared_ptr<CEValues>(new CEValuesStored(mSVDeps)));
    std::copy(mSpotGrid.begin(), mSpotGrid.end(), result->getDataPtr());
    return result;
}

double PDEPricingModelBlackScholes::getCollapsedPricerValue(PricerUid uid) const
{
    Exception::check(
        mCurrentDate == mPricingDate,
        "PDEPricingModelBlackScholes::getCollapsedPricerValue",
        "Can only be called once pricing date has been reached"
    );
    CEVConstPtr pricer(getPricer(uid));
    Exception::check(
        pricer->getVarMemoryLayout().getNbAVConfigurations() == 1,
        "PDEPricingModelBlackScholes::getCollapsedPricerValue",
        "Found a pricer with remaining AVDependencies, not expected"
    );
    CubicSplineInterpolatorCaching interp(&mLogSpotGrid[0], pricer->getDataPtr(), mSpaceGridSize, ExtrapolateNone);
    return interp.interpolate(Double::log(mSpot));
}

void PDEPricingModelBlackScholes::update(Solver1DInterface &solver)
{
    if ( mCoeffsSet )
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
    mCoeffsSet = true;
}
