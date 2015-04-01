//
//  PDEPricingModelBlackScholes.h
//  GenPDE
//
//  Created by Regis Dupont on 8/30/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef PDEPRICINGMODELBLACKSCHOLES_H_
#define PDEPRICINGMODELBLACKSCHOLES_H_

#include "PDEPricingModelBase.h"
#include "PDEUpdater1D.h"
#include "PDESolver1D.h"
#include "AVContext.h"

class PDEPricingModelBlackScholes : public PDEPricingModelBase, public PDEUpdater1D
{
public:
    PDEPricingModelBlackScholes(
        const GenPDE::Date&              pricing_date,
        
        double                           spot,
        double                           risk_free_rate,
        double                           volatility,
        
        double                           max_timestep_length,
        double                           nb_rannacher_steps,
        double                           max_rannacher_step_length,
        
        size_t                           space_grid_size,
        double                           space_grid_nb_std_devs
    )
    :PDEPricingModelBase(pricing_date, max_timestep_length, nb_rannacher_steps, max_rannacher_step_length)
    ,mSpot(spot)
    ,mRiskFreeRate(risk_free_rate)
    ,mVolatility(volatility)
    ,mSpaceGridSize(space_grid_size)
    ,mSpaceGridNbStdDevs(space_grid_nb_std_devs)
    ,mSVDeps(GenPDE::VT_StateVariable, 0, space_grid_size)
    ,mSpotGrid(space_grid_size)
    ,mLogSpotGrid(space_grid_size)
    ,mCoeffsSet(false)
    {}
    
    void setupForTrade(
        const std::vector<GenPDE::Date>& trade_dates,
        const AVCPtr&                    av_context,
        const FixingsPtr&                fixings
    )
    {
        mFixings   = fixings;
        mAVContext = av_context;
        std::vector<double> timeGrid;
        setupTimeGrid(mPricingDate, trade_dates, timeGrid);
        setupSpaceGrid(timeGrid);
        mSolver                 = boost::shared_ptr<PDESolverInterfaceBase>(new PDESolver1D(
            mLogSpotGrid,
            timeGrid,
            this
        ));
    }
    
    virtual CEVConstPtr   discountFactorCE(const GenPDE::Date& to_date) const;
    virtual CEVConstPtr   marketObservableCE(MOUid uid) const;
    virtual CEVConstPtr   auxiliaryVariableCE(GenPDE::VariableUID av_uid) const;
    virtual AVConstPtr    getAuxiliaryVariable(GenPDE::VariableUID av_uid) const;
    virtual double        getCollapsedPricerValue(PricerUid uid) const;
    
    void update(Solver1DInterface& solver);
    
protected:
    void setupSpaceGrid(const std::vector<double>& time_grid);
    
    const VarDependencies& getSVDependencies() const { return mSVDeps; }
    
    virtual boost::shared_ptr<PDESolverInterfaceBase> getSolver() { return mSolver; }
    
    virtual boost::shared_ptr<const PDESolverInterfaceBase> getSolver() const { return mSolver; }
    
    virtual void discretizeAVs();
    
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

#endif // PDEPRICINGMODELBLACKSCHOLES_H_
