//
//  PDEPricingModelInterface.h
//  GenPDE
//
//  Created by Regis Dupont on 8/27/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef PDEPRICINGMODELINTERFACE_H_
#define PDEPRICINGMODELINTERFACE_H_

#include <boost/shared_ptr.hpp>

#include "GpDate.h"
#include "CEValues.h"
#include "PricerUid.h"
#include "MarketObservable.h"
#include "TradeFixings.h"

class PDEPricingModelInterface
{
protected:
    typedef boost::shared_ptr<const CEValues>          CEVConstPtr;
    typedef boost::shared_ptr<CEValues>                CEVPtr;
    typedef boost::shared_ptr<const AuxiliaryVariable> AVConstPtr;
    typedef boost::shared_ptr<const TradeFixings>      FixingsPtr;
    
public:
    virtual ~PDEPricingModelInterface() {}
    
    virtual void setupForTrade(
        const std::vector<GenPDE::Date>& trade_dates,
        const AuxiliaryVariables&        auxiliary_variables,
        const FixingsPtr&                fixings = TradeFixings::NoFixings
    ) = 0;
    
    virtual GenPDE::Date  getPricingDate() const = 0;
    virtual GenPDE::Date  getCurrentDate() const = 0;
    
    /// Goes to the next date (of the union of trade_dates and pricing_date)
    /// Note that this might not involve PDE solving (if we've reached the
    /// pricing date already).
    ///
    /// Returns True while it actually timesteps, False if it's reached the
    /// minimal date
    virtual bool          timeStepToNextDate() = 0;
    
    virtual CEVConstPtr   discountFactorCE(const GenPDE::Date& to_date) const = 0;
    virtual CEVConstPtr   auxiliaryVariableCE(GenPDE::VariableUID av_uid) const = 0;
    virtual CEVConstPtr   marketObservableCE(MOUid uid) const = 0;    
    
    virtual CEVPtr        addPricer(PricerUid uid, const VarDependencies& av_deps) = 0;
    virtual CEVPtr        getPricer(PricerUid uid) = 0;
    virtual CEVConstPtr   getPricer(PricerUid uid) const = 0;
    virtual void          removePricer(PricerUid uid) = 0;
    
    /// Returns the 'collapsed' value of a pricer at pricing date (by definition, all
    /// dependencies can be removed at that point).
    virtual double        getCollapsedPricerValue(PricerUid uid) const = 0;
    
    /// The following two methods are specifically designed to be used when removing
    /// AVDependencies, b/c we don't want to change the PricerUid in that case, so
    /// we use a temporary PricerUid.
    virtual CEVPtr        addTempPricer(const VarDependencies& av_deps, PricerUid& uid) = 0;
    /// The following obliterates the existing PricerUid, if any.
    virtual void          renamePricer(PricerUid current_uid, PricerUid new_uid) = 0;
};

#endif // PDEPRICINGMODELINTERFACE_H_
