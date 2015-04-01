//
//  PricingInstructions.h
//  GenPDE
//
//  Created by Regis Dupont on 8/27/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef PRICINGINSTRUCTIONS_H_
#define PRICINGINSTRUCTIONS_H_

#include <boost/shared_ptr.hpp>
#include <vector>
#include <map>
#include <iostream> 

#include "GpDate.h"
#include "PricingInstruction.h"

class PricingInstructions
{
protected:
    typedef boost::shared_ptr<const PricingInstruction> PIPtr;
    
public:
    virtual ~PricingInstructions() {}
    
    PricingInstructions(
        const GenPDE::Date&       date,
        const std::vector<PIPtr>& pis
    )
    :mDate(date)
    ,mPIs(pis)
    {}
    
    GenPDE::Date getDate() const { return mDate; }
    
    std::vector<PIPtr> getPricingInstructions() const { return mPIs; }
    
protected:
    GenPDE::Date       mDate;
    std::vector<PIPtr> mPIs;
};

class DatedPricingInstructions
{
protected:
    typedef boost::shared_ptr<const PricingInstruction>  PIPtr;
    typedef boost::shared_ptr<const PricingInstructions> PISPtr;
    typedef std::map<GenPDE::Date, std::vector<PIPtr> >::const_iterator MapConstIt;
    
public:
    virtual ~DatedPricingInstructions() {}
    
    DatedPricingInstructions(const std::vector<PISPtr>& instructions)
    {
        for(const PISPtr& pis : instructions)
            mInstructions[pis->getDate()] = pis->getPricingInstructions();
    }
    
    std::vector<PIPtr> getPricingInstructions(const GenPDE::Date& date) const
    {
        MapConstIt it = mInstructions.find(date);
        if( it == mInstructions.end() )
            return std::vector<PIPtr>();
        return it->second;
    }
    
    /// Note that the result is sorted in ascending order
    std::vector<GenPDE::Date> getInstructionDates() const
    {
        std::vector<GenPDE::Date> result;
        result.reserve(mInstructions.size());
        MapConstIt itCurr = mInstructions.begin();
        MapConstIt itEnd  = mInstructions.end();
        for(; itCurr != itEnd; ++itCurr)
            result.push_back(itCurr->first);
        return result;
    }

protected:
    std::map<GenPDE::Date, std::vector<PIPtr> > mInstructions;
};


#endif // PRICINGINSTRUCTIONS_H_
