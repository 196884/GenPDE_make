#ifndef PDE_PRICING_MODEL_BLACK_SCHOLES_H
#define PDE_PRICING_MODEL_BLACK_SCHOLES_H

#include "PDEPricingModelBase.h"
#include "PDEUpdater1D.h"
#include "PDESolver1D.h"
#include "AVContext.h"
#include "BSModelParameters.h"
#include "PDEParameters1D.h"

class PDEPricingModelBlackScholes : public PDEPricingModelBase
                                  , public PDEUpdater1D
                                  , public MOFixingsIfc
{
public:
    PDEPricingModelBlackScholes(
        const GenPDE::Date&              pricing_date,
        const BSModelParameters&         model_params,
        const PDEParameters1D&           pde_params,
        const AVDiscretizationPolicy*    av_disc_policy
    )
    :PDEPricingModelBase(
        pricing_date, 
        pde_params.getMaxTimestepLength(),
        pde_params.getNbRannacherSteps(),
        pde_params.getMaxRannacherStepLength(),
        av_disc_policy
    )
    ,mSpot(               model_params.getSpot()                                     )
    ,mRiskFreeRate(       model_params.getRiskFreeRate()                             )
    ,mVolatility(         model_params.getVolatility()                               )
    ,mSpaceGridSize(      pde_params.getSpaceGridSize()                            )
    ,mSpaceGridNbStdDevs( pde_params.getSpaceGridNbStdDevs()                         )
    ,mSVDeps(             GenPDE::VT_StateVariable, 0, pde_params.getSpaceGridSize() )
    ,mSpotGrid(           pde_params.getSpaceGridSize()                              )
    ,mLogSpotGrid(        pde_params.getSpaceGridSize()                              )
    ,mCoeffsSet(          false                                                      )
    {}
    
    virtual void setupForTrade(
        const std::vector<GenPDE::Date>& trade_dates,
        const AuxiliaryVariables&        auxiliary_variables,
        const MOFixingsIfc*              mo_fixings
    );

    virtual double        getVariance(const GenPDE::Date& to_date) const;
    virtual double        getSpot() const; // at the pricing date
    virtual CEValuesCPtr  getFixing( MOUid mo_uid, const GenPDE::Date& date ) const;
   
    virtual CEVConstPtr   discountFactorCE(const GenPDE::Date& to_date) const;
    virtual CEVConstPtr   marketObservableCE(MOUid uid) const;
    virtual double        getCollapsedPricerValue(PricerUid uid) const;
    
    void update(Solver1DInterface& solver);
    
protected:
    void setupSpaceGrid(const std::vector<double>& time_grid);
    
    const VarDependencies& getSVDependencies() const { return mSVDeps; }
    
    virtual boost::shared_ptr<PDESolverInterfaceBase> getSolver() { return mSolver; }
    
    virtual boost::shared_ptr<const PDESolverInterfaceBase> getSolver() const { return mSolver; }
    
protected:
    double              mSpot;
    double              mRiskFreeRate;
    double              mVolatility;
    
    size_t              mSpaceGridSize;
    double              mSpaceGridNbStdDevs;

    VarDependencies     mSVDeps;
    boost::shared_ptr<PDESolverInterfaceBase> mSolver;
    std::vector<double> mSpotGrid;
    std::vector<double> mLogSpotGrid;
    bool                mCoeffsSet;
};

#endif // PDE_PRICING_MODEL_BLACK_SCHOLES_H
