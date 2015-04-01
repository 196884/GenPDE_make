//
//  AuxiliaryVariable.h
//  GenPDE
//
//  Created by Regis Dupont on 8/20/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef AUXILIARYVARIABLE_H_
#define AUXILIARYVARIABLE_H_

#include <vector>
#include <boost/shared_ptr.hpp>

#include "Date.h"
#include "VariableEnums.h"

class PayoutExpression;

class AuxiliaryVariable
{
public:
    AuxiliaryVariable(
        GenPDE::VariableUID                              uid,
        GenPDE::Date                                     date,
        const boost::shared_ptr<const PayoutExpression>& definition
    )
    :mUid(uid)
    ,mDefinition(definition)
    ,mDate(date)
    {}
    
    void setDiscretizationValues(const std::vector<double>& values)
    {
        mDiscretizationValues = values;
    }
    
    const std::vector<double>& getDiscretizationValues() const
    {
        return mDiscretizationValues;
    }
    
    size_t getNbDiscretizationValues() const { return mDiscretizationValues.size(); }
    
    boost::shared_ptr<const PayoutExpression> getDefinition() const { return mDefinition; }
    
    GenPDE::Date getDate() const { return mDate; }
    
    GenPDE::VariableUID getUid() const { return mUid; }
    
protected:
    GenPDE::VariableUID                       mUid;
    std::vector<double>                       mDiscretizationValues;
    boost::shared_ptr<const PayoutExpression> mDefinition;
    GenPDE::Date                              mDate;
};


#endif // AUXILIARYVARIABLE_H_
