#ifndef AUXILIARY_VARIABLE_H
#define AUXILIARY_VARIABLE_H

#include <vector>
#include <boost/shared_ptr.hpp>

#include "GpDate.h"
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
    
    boost::shared_ptr<const PayoutExpression> getDefinition() const { return mDefinition; }
    
    GenPDE::Date getDate() const { return mDate; }
    
    GenPDE::VariableUID getUid() const { return mUid; }
    
protected:
    GenPDE::VariableUID                       mUid;
    boost::shared_ptr<const PayoutExpression> mDefinition;
    GenPDE::Date                              mDate;
};

class AuxiliaryVariables
{
public:
    typedef boost::shared_ptr< const AuxiliaryVariable > AVConstPtr;
    typedef std::map< GenPDE::VariableUID, AVConstPtr >  AVMap;

public:
    AuxiliaryVariables() {}
    AuxiliaryVariables(const std::vector<AVConstPtr>& avs);

    AVConstPtr getAuxiliaryVariable(GenPDE::VariableUID uid) const;

    // Returns the vector of all UIDs, ordered such that if AV_a appears before AV_b,
    // Then AV_a cannot depend (even indirectly) on AV_b
    void getUids(std::vector<GenPDE::VariableUID>& uids) const;

    size_t getNbAVs() const;

protected:
    AVMap m_avMap;
};

#endif // AUXILIARY_VARIABLE_H
