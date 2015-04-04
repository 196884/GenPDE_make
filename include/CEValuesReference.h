#ifndef CE_VALUES_REFERENCE_H
#define CE_VALUES_REFERENCE_H

#include "CEValues.h"

class CEValuesReference : public CEValues
{  
public:
    CEValuesReference(
        const VarDependencies& var_deps,
        double*                data_ptr,
        bool                   is_inner_av = true
    )
    :CEValues(var_deps, is_inner_av)
    ,mDataPtr(data_ptr)
    {}
    
    CEValuesReference(
        double*               data_ptr
    )
    :mDataPtr(data_ptr)
    {}
    
    virtual ~CEValuesReference() {}
    
    virtual double*          getDataPtr()
    {
        return mDataPtr;
    }
    
    virtual const double*    getDataPtr() const
    {
        return mDataPtr;
    }
    
protected:
    double* mDataPtr;
};

class CEValuesConstReference : public CEValues
{  
public:
    CEValuesConstReference(
        const VarDependencies& var_deps,
        const double*          data_ptr,
        bool                   is_inner_av = true
    )
    :CEValues(var_deps, is_inner_av)
    ,mDataPtr(data_ptr)
    {}
    
    CEValuesConstReference(
        const double*               data_ptr
    )
    :mDataPtr(data_ptr)
    ,mInnerSVStride(0)
    {}
    
    virtual ~CEValuesConstReference() {}
    
    virtual double*          getDataPtr()
    {
        Exception::raise("CEValuesConstReference::getDataPtr", "Not available on this implementation");
        return 0;
    }
    
    virtual const double*    getDataPtr() const
    {
        return mDataPtr;
    }
    
protected:
    const double*       mDataPtr;
    size_t              mInnerSVStride;
};

#endif // CE_VALUES_REFERENCE_H
