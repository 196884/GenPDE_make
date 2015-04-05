#ifndef PDE_PRICING_MODEL_BASE_H
#define PDE_PRICING_MODEL_BASE_H

#include <map> 
#include <boost/shared_ptr.hpp>

#include "PDEPricingModelInterface.h"
#include "PDESolverInterfaceBase.h"
#include "AVContext.h"
#include "AVDiscretizationPolicy.h"
#include "VariableDependencies.h"

class PDEPricingModelBase : public PDEPricingModelInterface
{
protected:
    typedef std::vector<GenPDE::Date> DateVec;
    typedef std::vector<double>       DblVec;
    typedef std::pair<SolutionSetUid, VarDependencies> PricerDetails;
    
public:
    PDEPricingModelBase(
        const GenPDE::Date&                              pricing_date,
        double                                           max_timestep_length,
        size_t                                           nb_rannacher_steps,
        double                                           max_rannacher_step_length,
        const AVDiscretizationPolicy*                    av_disc_policy
    )
    :mMaxTimestepLength(       max_timestep_length       )
    ,mNbRannacherSteps(        nb_rannacher_steps        )
    ,mMaxRannacherStepLength(  max_rannacher_step_length )
    ,m_avDiscretizationPolicy( av_disc_policy            )
    ,m_moFixings(              NULL                      )
    ,mPricingDate(             pricing_date              )
    {}
    
    virtual ~PDEPricingModelBase();
    
    virtual GenPDE::Date  getPricingDate() const { return mPricingDate; }
    virtual GenPDE::Date  getCurrentDate() const { return mCurrentDate; }
    virtual bool          timeStepToNextDate();
    
    virtual CEVPtr        addPricer(    PricerUid uid , const VarDependencies& av_deps);
    virtual CEVPtr        getPricer(    PricerUid uid );
    virtual CEVConstPtr   getPricer(    PricerUid uid ) const;
    virtual void          removePricer( PricerUid uid );
    /// The following two methods are specifically designed to be used when removing
    /// AVDependencies, b/c we don't want to change the PricerUid in that case, so
    /// we use a temporary PricerUid.
    virtual CEVPtr        addTempPricer(const VarDependencies& av_deps, PricerUid& uid);
    /// The following obliterates the existing PricerUid, if any.
    virtual void          renamePricer(PricerUid current_uid, PricerUid new_uid);
    virtual CEVConstPtr   auxiliaryVariableCE(GenPDE::VariableUID av_uid) const;

protected:
    /// Helper routine, also sets the members used to keep track of date...
    virtual void setupTimeGrid(
        const GenPDE::Date&              pricing_date,
        const std::vector<GenPDE::Date>& trade_dates,
        std::vector<double>&             time_grid // to set!
    );
    
    virtual const VarDependencies& getSVDependencies() const = 0;
    
    virtual boost::shared_ptr<      PDESolverInterfaceBase> getSolver()       = 0;
    virtual boost::shared_ptr<const PDESolverInterfaceBase> getSolver() const = 0;
    
    virtual void collapseAllPricerValues();

    /// Routine to evaluate 'past' AVs based on fixings:
    virtual void setDeterministicAVs(
        const MOFixingsIfc&       mo_fixings,
        const AuxiliaryVariables& av_defs,
        AVContext&                av_context // set during the call
    );

    virtual void resetForTrade();
    
protected:
    double                                          mMaxTimestepLength;
    size_t                                          mNbRannacherSteps;
    double                                          mMaxRannacherStepLength;

    const AVDiscretizationPolicy*                   m_avDiscretizationPolicy;
    
    const MOFixingsIfc*                             m_moFixings;
    AVContext                                       m_avContext;
    
    std::vector<GenPDE::Date>                       mAllDates; // trade dates and pricing date
    size_t                                          mCurrentDateIndex; // in mAllDates;
    std::map<size_t, GenPDE::Date>                  mIndexToDate;
    std::map<GenPDE::Date, size_t>                  mDateToIndex;
    GenPDE::Date                                    mPricingDate;
    GenPDE::Date                                    mCurrentDate;

    std::map<PricerUid, PricerDetails>              mPricerToDetails;
    
    std::map<PricerUid, double>                     mCollapsedPricerValues;
};

#endif // PDE_PRICING_MODEL_BASE_H
