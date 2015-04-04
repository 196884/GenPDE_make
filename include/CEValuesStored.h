#ifndef CE_VALUES_STORED_H
#define CE_VALUES_STORED_H

#include "CEValues.h"

class CEValuesStored : public CEValues
{
protected:
    typedef boost::shared_ptr<const CEValues> CEVConstPtr;
    
public:
    CEValuesStored(
        const VarDependencies& var_deps,
        bool                   is_inner_av = true
    )
    :CEValues(var_deps, is_inner_av)
    ,mData(mVarMemLayout.getSize())
    {}
    
    CEValuesStored(double value)
    :CEValues()
    ,mData(1, value)
    {}
    
    CEValuesStored(
        const CEVConstPtr& cevs
    );
    
    CEValuesStored(
        const CEVConstPtr& cevs1,
        const CEVConstPtr& cevs2
    );
    
    CEValuesStored(
        const CEVConstPtr& cevs1,
        const CEVConstPtr& cevs2,
        const CEVConstPtr& cevs3
    );
    
    virtual ~CEValuesStored() {}
    
    virtual double*          getDataPtr()
    {
        return &mData[0];
    }
    
    virtual const double*    getDataPtr() const
    {
        return &mData[0];
    }
    
protected:
    std::vector<double> mData;
};

#endif // CE_VALUES_STORED_H
