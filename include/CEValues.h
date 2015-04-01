//
//  CEValues.h
//  GenPDE
//
//  Created by Regis Dupont on 8/21/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef CEVALUES_H_
#define CEVALUES_H_

#include <boost/shared_ptr.hpp>

#include "VariableDependencies.h"

#include "PEOperators.h"

/// \brief An abstract class to access conditional expectation values, i.e.,
/// values that potentially depend on StateVariable(s) and/or AuxiliaryVariable(s)
class CEValues
{
protected:
    typedef boost::shared_ptr<CEValues>       CEVPtr;
    typedef boost::shared_ptr<const CEValues> CEVConstPtr;
public:
    CEValues() {}
    
    CEValues(
        const VarDependencies& var_deps,
        bool                   is_av_inner = true
    )
    :mVarMemLayout(var_deps, is_av_inner)
    {}
    
    CEValues(
        const VarDependencies& var_deps1,
        const VarDependencies& var_deps2,
        bool                   is_av_inner = true
    )
    :mVarMemLayout(var_deps1, var_deps2, is_av_inner)
    {}
    
    CEValues(
        const VarDependencies& var_deps1,
        const VarDependencies& var_deps2,
        const VarDependencies& var_deps3,
        bool                   is_av_inner = true
    )
    :mVarMemLayout(var_deps1, var_deps2, var_deps3, is_av_inner)
    {}
    
    virtual ~CEValues() {}
    
    virtual const VarMemoryLayout& getVarMemoryLayout() const { return mVarMemLayout; }
    
    virtual size_t           getIndex(
        const VarBindings& var_bindings
    ) const
    {
        return mVarMemLayout.getIndex(var_bindings);
    }
    
    virtual double           getValue(
        const VarBindings& var_bindings
    ) const
    {
        size_t index      = getIndex(var_bindings);
        const double* ptr = getDataPtr();
        return ptr[index];
    }
    
    virtual void setValue(
        const VarBindings& var_bindings,
        double             value
    )
    {
        size_t index = getIndex(var_bindings);
        double* ptr  = getDataPtr();
        ptr[index]   = value;
    }
    
    virtual double*          getDataPtr() = 0;
    virtual const double*    getDataPtr() const = 0;
    
    void evalBinaryOperation(
        GenPDE::BinaryOperator  op,
        CEVConstPtr             arg1,
        CEVConstPtr             arg2
    );
    
    void evalComparisonOperation(
        GenPDE::ComparisonOperator  op,
        CEVConstPtr                 arg1,
        CEVConstPtr                 arg2
    );
    
    void evalUnaryOperation(
        GenPDE::UnaryOperator   op,
        CEVConstPtr             arg
    );
    
    void evalConditional(
        CEVConstPtr             cond,
        CEVConstPtr             arg1,
        CEVConstPtr             arg2
    );
    
    static void rmAVDependency(
        CEVPtr                  result,
        CEVConstPtr             arg,
        CEVConstPtr             av_values, // of the AV to remove
        const std::vector<double>& av_disc_values
    );
    
    /// The format is as follows:
    /// is_inner_av (0 or 1)
    /// nb_av_deps
    /// av_dep_id#0
    /// av_dep_nb_values#0
    /// ...
    /// nb_sv_deps
    /// sv_dep_id#0
    /// sv_dep_nb_values#0
    /// ...
    /// value#0
    /// ...
    friend std::ostream& operator<<(std::ostream& stream, const CEValues& ce_values);
    
protected:
    template <GenPDE::BinaryOperator OP>
    void evalBinaryOperationEx(
        CEVConstPtr             arg1,
        CEVConstPtr             arg2
    );
    
    template <GenPDE::ComparisonOperator OP>
    void evalComparisonOperationEx(
        CEVConstPtr             arg1,
        CEVConstPtr             arg2
    );
    
    template <GenPDE::UnaryOperator OP>
    void evalUnaryOperationEx(
        CEVConstPtr             arg
    );
    
    /// Handles the special case where result has no AVDep.
    static void rmAVDependency_noAVDepsResult(
        CEVPtr                  result,
        CEVConstPtr             arg,
        CEVConstPtr             av_values, // of the AV to remove
        const std::vector<double>& av_disc_values
    );

protected:
    VarMemoryLayout mVarMemLayout;
};

template <GenPDE::BinaryOperator OP>
void CEValues::evalBinaryOperationEx(
    CEVConstPtr             arg1,
    CEVConstPtr             arg2
)
{
    int loopingStrides1[MAX_NB_AV_DEPS + MAX_NB_SV_DEPS];
    int loopingStrides2[MAX_NB_AV_DEPS + MAX_NB_SV_DEPS];
    arg1->mVarMemLayout.getLoopStrides(mVarMemLayout, &loopingStrides1[0]);
    arg2->mVarMemLayout.getLoopStrides(mVarMemLayout, &loopingStrides2[0]);
    size_t        nbDeps  = mVarMemLayout.getNbDeps();
    const double* arg1Ptr = arg1->getDataPtr();
    const double* arg2Ptr = arg2->getDataPtr();
    double*       resPtr  = getDataPtr();
    
    // By definition, the strides of the result here are 1, 0, 0,...
    switch (nbDeps)
    {
    case 0:
    {
        *resPtr = GenPDE::BinaryOperation<OP>::apply(*arg1Ptr, *arg2Ptr);
        return;
    }
    case 1:
    {
        int arg1Stride = loopingStrides1[0];
        int arg2Stride = loopingStrides2[0];
        size_t size    = mVarMemLayout.getSize(0);
        for(size_t i=0; i<size; ++i)
        {
            *resPtr++ = GenPDE::BinaryOperation<OP>::apply(*arg1Ptr, *arg2Ptr);
            arg1Ptr += arg1Stride;
            arg2Ptr += arg2Stride;
        }
        return;
    }
    case 2:
    {
        int arg1Stride0 = loopingStrides1[0];
        int arg1Stride1 = loopingStrides1[1];
        int arg2Stride0 = loopingStrides2[0];
        int arg2Stride1 = loopingStrides2[1];
        size_t size0    = mVarMemLayout.getSize(0);
        size_t size1    = mVarMemLayout.getSize(1);
        for(size_t j=0; j<size1; ++j)
        {
            for(size_t i=0; i<size0; ++i)
            {
                *resPtr++ = GenPDE::BinaryOperation<OP>::apply(*arg1Ptr, *arg2Ptr);
                arg1Ptr += arg1Stride0;
                arg2Ptr += arg2Stride0;
            }
            arg1Ptr += arg1Stride1;
            arg2Ptr += arg2Stride1;
        }
        return;
    }
    case 3:
    {
        int arg1Stride0 = loopingStrides1[0];
        int arg1Stride1 = loopingStrides1[1];
        int arg1Stride2 = loopingStrides1[2];
        int arg2Stride0 = loopingStrides2[0];
        int arg2Stride1 = loopingStrides2[1];
        int arg2Stride2 = loopingStrides2[2];
        size_t size0    = mVarMemLayout.getSize(0);
        size_t size1    = mVarMemLayout.getSize(1);
        size_t size2    = mVarMemLayout.getSize(2);
        for(size_t k=0; k<size2; ++k)
        {
            for(size_t j=0; j<size1; ++j)
            {
                for(size_t i=0; i<size0; ++i)
                {
                    *resPtr++ = GenPDE::BinaryOperation<OP>::apply(*arg1Ptr, *arg2Ptr);
                    arg1Ptr += arg1Stride0;
                    arg2Ptr += arg2Stride0;
                }
                arg1Ptr += arg1Stride1;
                arg2Ptr += arg2Stride1;
            }
            arg1Ptr += arg1Stride2;
            arg2Ptr += arg2Stride2;
        }
        return;
    }
    case 4:
    {
        int arg1Stride0 = loopingStrides1[0];
        int arg1Stride1 = loopingStrides1[1];
        int arg1Stride2 = loopingStrides1[2];
        int arg1Stride3 = loopingStrides1[3];
        int arg2Stride0 = loopingStrides2[0];
        int arg2Stride1 = loopingStrides2[1];
        int arg2Stride2 = loopingStrides2[2];
        int arg2Stride3 = loopingStrides2[3];
        size_t size0    = mVarMemLayout.getSize(0);
        size_t size1    = mVarMemLayout.getSize(1);
        size_t size2    = mVarMemLayout.getSize(2);
        size_t size3    = mVarMemLayout.getSize(3);
        for(size_t l=0; l<size3; ++l)
        {
            for(size_t k=0; k<size2; ++k)
            {
                for(size_t j=0; j<size1; ++j)
                {
                    for(size_t i=0; i<size0; ++i)
                    {
                        *resPtr++ = GenPDE::BinaryOperation<OP>::apply(*arg1Ptr, *arg2Ptr);
                        arg1Ptr += arg1Stride0;
                        arg2Ptr += arg2Stride0;
                    }
                    arg1Ptr += arg1Stride1;
                    arg2Ptr += arg2Stride1;
                }
                arg1Ptr += arg1Stride2;
                arg2Ptr += arg2Stride2;
            }
            arg1Ptr += arg1Stride3;
            arg2Ptr += arg2Stride3;
        }
        return;
    }
    case 5:
    {
        int arg1Stride0 = loopingStrides1[0];
        int arg1Stride1 = loopingStrides1[1];
        int arg1Stride2 = loopingStrides1[2];
        int arg1Stride3 = loopingStrides1[3];
        int arg1Stride4 = loopingStrides1[4];
        int arg2Stride0 = loopingStrides2[0];
        int arg2Stride1 = loopingStrides2[1];
        int arg2Stride2 = loopingStrides2[2];
        int arg2Stride3 = loopingStrides2[3];
        int arg2Stride4 = loopingStrides2[4];
        size_t size0    = mVarMemLayout.getSize(0);
        size_t size1    = mVarMemLayout.getSize(1);
        size_t size2    = mVarMemLayout.getSize(2);
        size_t size3    = mVarMemLayout.getSize(3);
        size_t size4    = mVarMemLayout.getSize(4);
        for(size_t m=0; m<size4; ++m)
        {
            for(size_t l=0; l<size3; ++l)
            {
                for(size_t k=0; k<size2; ++k)
                {
                    for(size_t j=0; j<size1; ++j)
                    {
                        for(size_t i=0; i<size0; ++i)
                        {
                            *resPtr++ = GenPDE::BinaryOperation<OP>::apply(*arg1Ptr, *arg2Ptr);
                            arg1Ptr += arg1Stride0;
                            arg2Ptr += arg2Stride0;
                        }
                        arg1Ptr += arg1Stride1;
                        arg2Ptr += arg2Stride1;
                    }
                    arg1Ptr += arg1Stride2;
                    arg2Ptr += arg2Stride2;
                }
                arg1Ptr += arg1Stride3;
                arg2Ptr += arg2Stride3;
            }
            arg1Ptr += arg1Stride4;
            arg2Ptr += arg2Stride4;
        }
        return;
    }
    case 6:
    {
        int arg1Stride0 = loopingStrides1[0];
        int arg1Stride1 = loopingStrides1[1];
        int arg1Stride2 = loopingStrides1[2];
        int arg1Stride3 = loopingStrides1[3];
        int arg1Stride4 = loopingStrides1[4];
        int arg1Stride5 = loopingStrides1[5];
        int arg2Stride0 = loopingStrides2[0];
        int arg2Stride1 = loopingStrides2[1];
        int arg2Stride2 = loopingStrides2[2];
        int arg2Stride3 = loopingStrides2[3];
        int arg2Stride4 = loopingStrides2[4];
        int arg2Stride5 = loopingStrides2[5];
        size_t size0    = mVarMemLayout.getSize(0);
        size_t size1    = mVarMemLayout.getSize(1);
        size_t size2    = mVarMemLayout.getSize(2);
        size_t size3    = mVarMemLayout.getSize(3);
        size_t size4    = mVarMemLayout.getSize(4);
        size_t size5    = mVarMemLayout.getSize(5);
        for(size_t n=0; n<size5; ++n)
        {
            for(size_t m=0; m<size4; ++m)
            {
                for(size_t l=0; l<size3; ++l)
                {
                    for(size_t k=0; k<size2; ++k)
                    {
                        for(size_t j=0; j<size1; ++j)
                        {
                            for(size_t i=0; i<size0; ++i)
                            {
                                *resPtr++ = GenPDE::BinaryOperation<OP>::apply(*arg1Ptr, *arg2Ptr);
                                arg1Ptr += arg1Stride0;
                                arg2Ptr += arg2Stride0;
                            }
                            arg1Ptr += arg1Stride1;
                            arg2Ptr += arg2Stride1;
                        }
                        arg1Ptr += arg1Stride2;
                        arg2Ptr += arg2Stride2;
                    }
                    arg1Ptr += arg1Stride3;
                    arg2Ptr += arg2Stride3;
                }
                arg1Ptr += arg1Stride4;
                arg2Ptr += arg2Stride4;
            }
            arg1Ptr += arg1Stride5;
            arg2Ptr += arg2Stride5;
        }
        return;
    }
    default:
    {
        Exception::raise("CEValues::evalBinaryOperationEx", "Not implemented for more than 6 variable dependencies");
    }
    }
}

template <GenPDE::ComparisonOperator OP>
void CEValues::evalComparisonOperationEx(
    CEVConstPtr             arg1,
    CEVConstPtr             arg2
)
{
    int loopingStrides1[MAX_NB_AV_DEPS + MAX_NB_SV_DEPS];
    int loopingStrides2[MAX_NB_AV_DEPS + MAX_NB_SV_DEPS];
    arg1->mVarMemLayout.getLoopStrides(mVarMemLayout, &loopingStrides1[0]);
    arg2->mVarMemLayout.getLoopStrides(mVarMemLayout, &loopingStrides2[0]);
    size_t        nbDeps  = mVarMemLayout.getNbDeps();
    const double* arg1Ptr = arg1->getDataPtr();
    const double* arg2Ptr = arg2->getDataPtr();
    double*       resPtr  = getDataPtr();
    
    // By definition, the strides of the result here are 1, 0, 0,...
    switch (nbDeps)
    {
    case 0:
    {
        *resPtr = GenPDE::ComparisonOperation<OP>::apply(*arg1Ptr, *arg2Ptr);
        return;
    }
    case 1:
    {
        int arg1Stride = loopingStrides1[0];
        int arg2Stride = loopingStrides2[0];
        size_t size    = mVarMemLayout.getSize(0);
        for(size_t i=0; i<size; ++i)
        {
            *resPtr++ = GenPDE::ComparisonOperation<OP>::apply(*arg1Ptr, *arg2Ptr);
            arg1Ptr += arg1Stride;
            arg2Ptr += arg2Stride;
        }
        return;
    }
    case 2:
    {
        int arg1Stride0 = loopingStrides1[0];
        int arg1Stride1 = loopingStrides1[1];
        int arg2Stride0 = loopingStrides2[0];
        int arg2Stride1 = loopingStrides2[1];
        size_t size0    = mVarMemLayout.getSize(0);
        size_t size1    = mVarMemLayout.getSize(1);
        for(size_t j=0; j<size1; ++j)
        {
            for(size_t i=0; i<size0; ++i)
            {
                *resPtr++ = GenPDE::ComparisonOperation<OP>::apply(*arg1Ptr, *arg2Ptr);
                arg1Ptr += arg1Stride0;
                arg2Ptr += arg2Stride0;
            }
            arg1Ptr += arg1Stride1;
            arg2Ptr += arg2Stride1;
        }
        return;
    }
    case 3:
    {
        int arg1Stride0 = loopingStrides1[0];
        int arg1Stride1 = loopingStrides1[1];
        int arg1Stride2 = loopingStrides1[2];
        int arg2Stride0 = loopingStrides2[0];
        int arg2Stride1 = loopingStrides2[1];
        int arg2Stride2 = loopingStrides2[2];
        size_t size0    = mVarMemLayout.getSize(0);
        size_t size1    = mVarMemLayout.getSize(1);
        size_t size2    = mVarMemLayout.getSize(2);
        for(size_t k=0; k<size2; ++k)
        {
            for(size_t j=0; j<size1; ++j)
            {
                for(size_t i=0; i<size0; ++i)
                {
                    *resPtr++ = GenPDE::ComparisonOperation<OP>::apply(*arg1Ptr, *arg2Ptr);
                    arg1Ptr += arg1Stride0;
                    arg2Ptr += arg2Stride0;
                }
                arg1Ptr += arg1Stride1;
                arg2Ptr += arg2Stride1;
            }
            arg1Ptr += arg1Stride2;
            arg2Ptr += arg2Stride2;
        }
        return;
    }
    case 4:
    {
        int arg1Stride0 = loopingStrides1[0];
        int arg1Stride1 = loopingStrides1[1];
        int arg1Stride2 = loopingStrides1[2];
        int arg1Stride3 = loopingStrides1[3];
        int arg2Stride0 = loopingStrides2[0];
        int arg2Stride1 = loopingStrides2[1];
        int arg2Stride2 = loopingStrides2[2];
        int arg2Stride3 = loopingStrides2[3];
        size_t size0    = mVarMemLayout.getSize(0);
        size_t size1    = mVarMemLayout.getSize(1);
        size_t size2    = mVarMemLayout.getSize(2);
        size_t size3    = mVarMemLayout.getSize(3);
        for(size_t l=0; l<size3; ++l)
        {
            for(size_t k=0; k<size2; ++k)
            {
                for(size_t j=0; j<size1; ++j)
                {
                    for(size_t i=0; i<size0; ++i)
                    {
                        *resPtr++ = GenPDE::ComparisonOperation<OP>::apply(*arg1Ptr, *arg2Ptr);
                        arg1Ptr += arg1Stride0;
                        arg2Ptr += arg2Stride0;
                    }
                    arg1Ptr += arg1Stride1;
                    arg2Ptr += arg2Stride1;
                }
                arg1Ptr += arg1Stride2;
                arg2Ptr += arg2Stride2;
            }
            arg1Ptr += arg1Stride3;
            arg2Ptr += arg2Stride3;
        }
        return;
    }
    case 5:
    {
        int arg1Stride0 = loopingStrides1[0];
        int arg1Stride1 = loopingStrides1[1];
        int arg1Stride2 = loopingStrides1[2];
        int arg1Stride3 = loopingStrides1[3];
        int arg1Stride4 = loopingStrides1[4];
        int arg2Stride0 = loopingStrides2[0];
        int arg2Stride1 = loopingStrides2[1];
        int arg2Stride2 = loopingStrides2[2];
        int arg2Stride3 = loopingStrides2[3];
        int arg2Stride4 = loopingStrides2[4];
        size_t size0    = mVarMemLayout.getSize(0);
        size_t size1    = mVarMemLayout.getSize(1);
        size_t size2    = mVarMemLayout.getSize(2);
        size_t size3    = mVarMemLayout.getSize(3);
        size_t size4    = mVarMemLayout.getSize(4);
        for(size_t m=0; m<size4; ++m)
        {
            for(size_t l=0; l<size3; ++l)
            {
                for(size_t k=0; k<size2; ++k)
                {
                    for(size_t j=0; j<size1; ++j)
                    {
                        for(size_t i=0; i<size0; ++i)
                        {
                            *resPtr++ = GenPDE::ComparisonOperation<OP>::apply(*arg1Ptr, *arg2Ptr);
                            arg1Ptr += arg1Stride0;
                            arg2Ptr += arg2Stride0;
                        }
                        arg1Ptr += arg1Stride1;
                        arg2Ptr += arg2Stride1;
                    }
                    arg1Ptr += arg1Stride2;
                    arg2Ptr += arg2Stride2;
                }
                arg1Ptr += arg1Stride3;
                arg2Ptr += arg2Stride3;
            }
            arg1Ptr += arg1Stride4;
            arg2Ptr += arg2Stride4;
        }
        return;
    }
    case 6:
    {
        int arg1Stride0 = loopingStrides1[0];
        int arg1Stride1 = loopingStrides1[1];
        int arg1Stride2 = loopingStrides1[2];
        int arg1Stride3 = loopingStrides1[3];
        int arg1Stride4 = loopingStrides1[4];
        int arg1Stride5 = loopingStrides1[5];
        int arg2Stride0 = loopingStrides2[0];
        int arg2Stride1 = loopingStrides2[1];
        int arg2Stride2 = loopingStrides2[2];
        int arg2Stride3 = loopingStrides2[3];
        int arg2Stride4 = loopingStrides2[4];
        int arg2Stride5 = loopingStrides2[5];
        size_t size0    = mVarMemLayout.getSize(0);
        size_t size1    = mVarMemLayout.getSize(1);
        size_t size2    = mVarMemLayout.getSize(2);
        size_t size3    = mVarMemLayout.getSize(3);
        size_t size4    = mVarMemLayout.getSize(4);
        size_t size5    = mVarMemLayout.getSize(5);
        for(size_t n=0; n<size5; ++n)
        {
            for(size_t m=0; m<size4; ++m)
            {
                for(size_t l=0; l<size3; ++l)
                {
                    for(size_t k=0; k<size2; ++k)
                    {
                        for(size_t j=0; j<size1; ++j)
                        {
                            for(size_t i=0; i<size0; ++i)
                            {
                                *resPtr++ = GenPDE::ComparisonOperation<OP>::apply(*arg1Ptr, *arg2Ptr);
                                arg1Ptr += arg1Stride0;
                                arg2Ptr += arg2Stride0;
                            }
                            arg1Ptr += arg1Stride1;
                            arg2Ptr += arg2Stride1;
                        }
                        arg1Ptr += arg1Stride2;
                        arg2Ptr += arg2Stride2;
                    }
                    arg1Ptr += arg1Stride3;
                    arg2Ptr += arg2Stride3;
                }
                arg1Ptr += arg1Stride4;
                arg2Ptr += arg2Stride4;
            }
            arg1Ptr += arg1Stride5;
            arg2Ptr += arg2Stride5;
        }
        return;
    }
    default:
    {
        Exception::raise("CEValues::evalComparisonOperationEx", "Not implemented for more than 6 variable dependencies");
    }
    }
}

template <GenPDE::UnaryOperator OP>
void CEValues::evalUnaryOperationEx(
    CEVConstPtr             arg1
)
{
    int loopingStrides1[MAX_NB_AV_DEPS + MAX_NB_SV_DEPS];
    arg1->mVarMemLayout.getLoopStrides(mVarMemLayout, &loopingStrides1[0]);
    size_t        nbDeps  = mVarMemLayout.getNbDeps();
    const double* arg1Ptr = arg1->getDataPtr();
    double*       resPtr  = getDataPtr();
    
    // By definition, the strides of the result here are 1, 0, 0,...
    switch (nbDeps)
    {
    case 0:
    {
        *resPtr = GenPDE::UnaryOperation<OP>::apply(*arg1Ptr);
        return;
    }
    case 1:
    {
        int arg1Stride = loopingStrides1[0];
        size_t size    = mVarMemLayout.getSize(0);
        for(size_t i=0; i<size; ++i)
        {
            *resPtr++ = GenPDE::UnaryOperation<OP>::apply(*arg1Ptr);
            arg1Ptr += arg1Stride;
        }
        return;
    }
    case 2:
    {
        int arg1Stride0 = loopingStrides1[0];
        int arg1Stride1 = loopingStrides1[1];
        size_t size0    = mVarMemLayout.getSize(0);
        size_t size1    = mVarMemLayout.getSize(1);
        for(size_t j=0; j<size1; ++j)
        {
            for(size_t i=0; i<size0; ++i)
            {
                *resPtr++ = GenPDE::UnaryOperation<OP>::apply(*arg1Ptr);
                arg1Ptr += arg1Stride0;
            }
            arg1Ptr += arg1Stride1;
        }
        return;
    }
    case 3:
    {
        int arg1Stride0 = loopingStrides1[0];
        int arg1Stride1 = loopingStrides1[1];
        int arg1Stride2 = loopingStrides1[2];
        size_t size0    = mVarMemLayout.getSize(0);
        size_t size1    = mVarMemLayout.getSize(1);
        size_t size2    = mVarMemLayout.getSize(2);
        for(size_t k=0; k<size2; ++k)
        {
            for(size_t j=0; j<size1; ++j)
            {
                for(size_t i=0; i<size0; ++i)
                {
                    *resPtr++ = GenPDE::UnaryOperation<OP>::apply(*arg1Ptr);
                    arg1Ptr += arg1Stride0;
                }
                arg1Ptr += arg1Stride1;
            }
            arg1Ptr += arg1Stride2;
        }
        return;
    }
    case 4:
    {
        int arg1Stride0 = loopingStrides1[0];
        int arg1Stride1 = loopingStrides1[1];
        int arg1Stride2 = loopingStrides1[2];
        int arg1Stride3 = loopingStrides1[3];
        size_t size0    = mVarMemLayout.getSize(0);
        size_t size1    = mVarMemLayout.getSize(1);
        size_t size2    = mVarMemLayout.getSize(2);
        size_t size3    = mVarMemLayout.getSize(3);
        for(size_t l=0; l<size3; ++l)
        {
            for(size_t k=0; k<size2; ++k)
            {
                for(size_t j=0; j<size1; ++j)
                {
                    for(size_t i=0; i<size0; ++i)
                    {
                        *resPtr++ = GenPDE::UnaryOperation<OP>::apply(*arg1Ptr);
                        arg1Ptr += arg1Stride0;
                    }
                    arg1Ptr += arg1Stride1;
                }
                arg1Ptr += arg1Stride2;
            }
            arg1Ptr += arg1Stride3;
        }
        return;
    }
    case 5:
    {
        int arg1Stride0 = loopingStrides1[0];
        int arg1Stride1 = loopingStrides1[1];
        int arg1Stride2 = loopingStrides1[2];
        int arg1Stride3 = loopingStrides1[3];
        int arg1Stride4 = loopingStrides1[4];
        size_t size0    = mVarMemLayout.getSize(0);
        size_t size1    = mVarMemLayout.getSize(1);
        size_t size2    = mVarMemLayout.getSize(2);
        size_t size3    = mVarMemLayout.getSize(3);
        size_t size4    = mVarMemLayout.getSize(4);
        for(size_t m=0; m<size4; ++m)
        {
            for(size_t l=0; l<size3; ++l)
            {
                for(size_t k=0; k<size2; ++k)
                {
                    for(size_t j=0; j<size1; ++j)
                    {
                        for(size_t i=0; i<size0; ++i)
                        {
                            *resPtr++ = GenPDE::UnaryOperation<OP>::apply(*arg1Ptr);
                            arg1Ptr += arg1Stride0;
                        }
                        arg1Ptr += arg1Stride1;
                    }
                    arg1Ptr += arg1Stride2;
                }
                arg1Ptr += arg1Stride3;
            }
            arg1Ptr += arg1Stride4;
        }
        return;
    }
    case 6:
    {
        int arg1Stride0 = loopingStrides1[0];
        int arg1Stride1 = loopingStrides1[1];
        int arg1Stride2 = loopingStrides1[2];
        int arg1Stride3 = loopingStrides1[3];
        int arg1Stride4 = loopingStrides1[4];
        int arg1Stride5 = loopingStrides1[5];
        size_t size0    = mVarMemLayout.getSize(0);
        size_t size1    = mVarMemLayout.getSize(1);
        size_t size2    = mVarMemLayout.getSize(2);
        size_t size3    = mVarMemLayout.getSize(3);
        size_t size4    = mVarMemLayout.getSize(4);
        size_t size5    = mVarMemLayout.getSize(5);
        for(size_t n=0; n<size5; ++n)
        {
            for(size_t m=0; m<size4; ++m)
            {
                for(size_t l=0; l<size3; ++l)
                {
                    for(size_t k=0; k<size2; ++k)
                    {
                        for(size_t j=0; j<size1; ++j)
                        {
                            for(size_t i=0; i<size0; ++i)
                            {
                                *resPtr++ = GenPDE::UnaryOperation<OP>::apply(*arg1Ptr);
                                arg1Ptr += arg1Stride0;
                            }
                            arg1Ptr += arg1Stride1;
                        }
                        arg1Ptr += arg1Stride2;
                    }
                    arg1Ptr += arg1Stride3;
                }
                arg1Ptr += arg1Stride4;
            }
            arg1Ptr += arg1Stride5;
        }
        return;
    }
    default:
    {
        Exception::raise("CEValues::evalUnaryOperationEx", "Not implemented for more than 6 variable dependencies");
    }
    }
}

#endif // CEVALUES_H_