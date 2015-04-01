//
//  PDEPricingModelBase.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/30/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "PDEPricingModelBase.h"
#include "CEValuesReference.h"

void PDEPricingModelBase::setupTimeGrid(
    const GenPDE::Date&              pricing_date,
    const std::vector<GenPDE::Date>& trade_dates,
    std::vector<double>&             time_grid
)
{
    mIndexToDate.clear();
    mDateToIndex.clear();
    DateVec sortedDates(trade_dates);
    std::sort(sortedDates.begin(), sortedDates.end());
    mAllDates = trade_dates;
    mAllDates.push_back(pricing_date);
    std::sort(mAllDates.begin(), mAllDates.end());
    mAllDates.erase(std::unique(mAllDates.begin(), mAllDates.end()), mAllDates.end());
    mCurrentDateIndex = mAllDates.size() - 1;
    mCurrentDate      = mAllDates.back();
    
    // We do everything in days, anchored at pricing_date (the first point
    // in timeGrid, so by def. timeGrid[0] = 0
    time_grid.resize(1);
    time_grid[0]               = 0;
    double lastNode            = 0;
    GenPDE::Date lastDate      = pricing_date;
    mPricingDate               = pricing_date;
    mIndexToDate[0]            = pricing_date;
    mDateToIndex[pricing_date] = 0;

    // We skip all the dates in the past:
    for(GenPDE::Date tradeDate : trade_dates)
    {
        if( tradeDate <= lastDate )
            continue;
        
        double node       = (double) GenPDE::dateDifferenceInDays(tradeDate, pricing_date);
        double nodeDiff   = node - lastNode;
        if( mNbRannacherSteps * mMaxRannacherStepLength >= nodeDiff )
        {
            // Adding only Rannacher steps
            double stepLength = nodeDiff / mNbRannacherSteps;
            for(size_t i=1; i<mNbRannacherSteps; ++i)
                time_grid.push_back(time_grid.back() + stepLength);
        } else {
            size_t nbStepsAux = ceil((node - lastNode - mNbRannacherSteps * mMaxRannacherStepLength) / mMaxTimestepLength);
            double stepLength = (node - lastNode - mNbRannacherSteps * mMaxRannacherStepLength) / nbStepsAux;
            if( mNbRannacherSteps == 0 )
                --nbStepsAux;
            for(size_t i=0; i<nbStepsAux; ++i)
                time_grid.push_back(time_grid.back() + stepLength);
            for(size_t i=1; i<mNbRannacherSteps; ++i)
                time_grid.push_back(time_grid.back() + mMaxRannacherStepLength);
        }
        size_t dateIndex = time_grid.size();
        mIndexToDate[dateIndex] = tradeDate;
        mDateToIndex[tradeDate] = dateIndex;
        time_grid.push_back(node);
        lastDate                = tradeDate;
        lastNode                = node;
    }
    double dayInYears = 1.0 / ((double) GenPDE::NbDaysPerYear);
    // We normalize the time grid (to have year units...)
    for(double& time : time_grid)
        time *= dayInYears;
}

bool PDEPricingModelBase::timeStepToNextDate()
{
    if( 0 == mCurrentDateIndex )
        return false;
    if( mCurrentDate > mPricingDate )
    {
        // We actually need to timestep the solver
        std::map<GenPDE::Date, size_t>::const_iterator currIt = mDateToIndex.find(mCurrentDate);
        if( currIt == mDateToIndex.end() )
            Exception::raise("PDEPricingModelBase::timeStepToNextDate", "Unexpected: could not find current date in mDateToIndex");
        size_t currIdx = currIt->second;
        --currIt;
        size_t nbSteps = currIdx - currIt->second;
        if( nbSteps < mNbRannacherSteps )
            Exception::raise("PDEPricingModelBase::timeStepToNextDate", "Not enough steps to apply Rannacher, not expected");
        size_t i = 0;
        for(; i < mNbRannacherSteps; ++i)
        {
            //CEVPtr pricer = getPricer(1);
            //std::cout << *pricer << "******************************************" << std::endl;
            getSolver()->timeStepOnce(true);
            //CEVPtr pricer = getPricer(1);
            //std::cout << *pricer << "******************************************" << std::endl;
        }
        for(; i < nbSteps; ++i)
        {
            //CEVPtr pricer = getPricer(1);
            //std::cout << *pricer << "******************************************" << std::endl;
            getSolver()->timeStepOnce(false);
        }
    }
    mCurrentDate = mAllDates[--mCurrentDateIndex];
    if( mCurrentDate ==  mPricingDate )
    {
        //CEVPtr pricer = getPricer(1);
        //std::cout << *pricer << "******************************************" << std::endl;
        collapseAllPricerValues();
    }
    return true;
}

PDEPricingModelBase::CEVPtr PDEPricingModelBase::addPricer(PricerUid uid, const VarDependencies& av_deps)
{
    if( mCurrentDate <= mPricingDate )
    {
        if( mCollapsedPricerValues.find(uid) != mCollapsedPricerValues.end() )
            Exception::raise("PDEPricingModelBase::addPricer", "Cannot add pricer, uid already exists");
        mCollapsedPricerValues[uid] = 0;
        CEVPtr result(new CEValuesReference(&(mCollapsedPricerValues.find(uid)->second)));
        return result;
    }
    if( mPricerToDetails.find(uid) != mPricerToDetails.end() )
        Exception::raise("PDEPricingModelBase::addPricer", "Cannot add pricer, uid already exists");
    size_t  nbPlanes = av_deps.getNbAVConfigurations();
    SolutionSetUid solSetUid = getSolver()->addSolutionSet(nbPlanes);
    double* dataPtr  = getSolver()->getSolutionSetPtr(solSetUid);
    VarDependencies deps(getSVDependencies(), av_deps);
    CEVPtr  result(new CEValuesReference(deps, dataPtr));
    mPricerToDetails[uid] = std::make_pair(solSetUid, deps);
    return result;
}

PDEPricingModelBase::CEVPtr PDEPricingModelBase::getPricer(PricerUid uid)
{
    std::map<PricerUid, double>::iterator it(mCollapsedPricerValues.find(uid));
    if( it != mCollapsedPricerValues.end() )
    {
        CEVPtr result(new CEValuesReference(&it->second));
        return result;
    }
    std::map<PricerUid, PricerDetails>::const_iterator it2(mPricerToDetails.find(uid));
    if( it2 == mPricerToDetails.end() )
        Exception::raise("PDEPricingModelBase::getPricer", "Could not find details for specified pricer");
    double* dataPtr = getSolver()->getSolutionSetPtr(it2->second.first);
    CEVPtr result(new CEValuesReference(it2->second.second, dataPtr));
    return result;
}

PDEPricingModelBase::CEVConstPtr PDEPricingModelBase::getPricer(PricerUid uid) const
{
    std::map<PricerUid, double>::const_iterator it(mCollapsedPricerValues.find(uid));
    if( it != mCollapsedPricerValues.end() )
    {
        CEVPtr result(new CEValuesConstReference(&it->second));
        return result;
    }
    std::map<PricerUid, PricerDetails>::const_iterator it2(mPricerToDetails.find(uid));
    if( it2 == mPricerToDetails.end() )
        Exception::raise("PDEPricingModelBase::getPricer", "Could not find details for specified pricer");
    const double* dataPtr = getSolver()->getSolutionSetConstPtr(it2->second.first);
    CEVPtr result(new CEValuesConstReference(it2->second.second, dataPtr));
    return result;
}

void PDEPricingModelBase::removePricer(PricerUid uid)
{
    if( mCurrentDate <= mPricingDate )
    {
        std::map<PricerUid, double>::const_iterator it = mCollapsedPricerValues.find(uid);
        if( it == mCollapsedPricerValues.end() )
            Exception::raise("PDEPricingModelBase::removePricer", "Could not find details for specified pricer");
        mCollapsedPricerValues.erase(it);
        return;
    }
    
    std::map<PricerUid, PricerDetails>::const_iterator it = mPricerToDetails.find(uid);
    if( it == mPricerToDetails.end() )
        Exception::raise("PDEPricingModelBase::removePricer", "Could not find details for specified pricer");
    getSolver()->removeSolutionSet(it->second.first);
    mPricerToDetails.erase(it);
}

PDEPricingModelBase::CEVPtr PDEPricingModelBase::addTempPricer(const VarDependencies& av_deps, PricerUid& uid)
{
    Exception::check(
        mCurrentDate > mPricingDate,
        "PDEPricingModelBase::addTempPricer",
        "Only supposed to be called when the pricing date hasn't been reached"
    );
    PricerUid maxUid(mPricerToDetails.rbegin()->first);
    uid = maxUid+1;
    return addPricer(uid, av_deps);
}

void PDEPricingModelBase::renamePricer(PricerUid current_uid, PricerUid new_uid)
{
    Exception::check(
        mCurrentDate > mPricingDate,
        "PDEPricingModelBase::renamePricer",
        "Only supposed to be called when the pricing date hasn't been reached"
    );
    std::map<PricerUid, PricerDetails>::const_iterator itCurr = mPricerToDetails.find(current_uid);
    if( itCurr == mPricerToDetails.end() )
        Exception::raise("PDEPricingModelBase::renamePricer", "No existing pricer with current_uid");
    std::map<PricerUid, PricerDetails>::const_iterator itNew  = mPricerToDetails.find(new_uid);
    if( itNew != mPricerToDetails.end() )
        getSolver()->removeSolutionSet(itNew->second.first);
    mPricerToDetails[new_uid] = itCurr->second;
    mPricerToDetails.erase(current_uid);
}

void PDEPricingModelBase::collapseAllPricerValues()
{
    std::map<PricerUid, PricerDetails>::const_iterator itCurr(mPricerToDetails.begin());
    std::map<PricerUid, PricerDetails>::const_iterator itEnd(mPricerToDetails.end());
    for(; itCurr != itEnd; ++itCurr)
    {
        mCollapsedPricerValues[itCurr->first] = getCollapsedPricerValue(itCurr->first);
        getSolver()->removeSolutionSet(itCurr->second.first); // safety measure!
    }
    // To be sure the other values don't get used, we delete them:
    mPricerToDetails.clear();
}

