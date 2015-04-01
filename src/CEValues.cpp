//
//  CEValues.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/24/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "CEValues.h"
#include "AVInterpolatorNaturalCubic.h"
#include "AVInterpolatorLinear.h"

void CEValues::evalBinaryOperation(
    GenPDE::BinaryOperator  op,
    CEVConstPtr             arg1,
    CEVConstPtr             arg2
)
{
    switch (op)
    {
    case GenPDE::Add:
        evalBinaryOperationEx<GenPDE::Add>(arg1, arg2);
        break;
    case GenPDE::Sub:
        evalBinaryOperationEx<GenPDE::Sub>(arg1, arg2);
        break;
    case GenPDE::Mul:
        evalBinaryOperationEx<GenPDE::Mul>(arg1, arg2);
        break;
    case GenPDE::Div:
        evalBinaryOperationEx<GenPDE::Div>(arg1, arg2);
        break;
    case GenPDE::Max:
        evalBinaryOperationEx<GenPDE::Max>(arg1, arg2);
        break;
    case GenPDE::Min:
        evalBinaryOperationEx<GenPDE::Min>(arg1, arg2);
        break;
    default:
        Exception::raise("CEValues::evalBinaryOperation", "Unknown operator");
    }
}

void CEValues::evalComparisonOperation(
    GenPDE::ComparisonOperator  op,
    CEVConstPtr                 arg1,
    CEVConstPtr                 arg2
)
{
    switch (op)
    {
    case GenPDE::LT:
        evalComparisonOperationEx<GenPDE::LT>(arg1, arg2);
        break;
    case GenPDE::LE:
        evalComparisonOperationEx<GenPDE::LE>(arg1, arg2);
        break;
    case GenPDE::GT:
        evalComparisonOperationEx<GenPDE::GT>(arg1, arg2);
        break;
    case GenPDE::GE:
        evalComparisonOperationEx<GenPDE::GE>(arg1, arg2);
        break;
    case GenPDE::EQ:
        evalComparisonOperationEx<GenPDE::EQ>(arg1, arg2);
        break;
    case GenPDE::NE:
        evalComparisonOperationEx<GenPDE::NE>(arg1, arg2);
        break;
    default:
        Exception::raise("CEValues::evalComparisonOperation", "Unknown operator");
    }
}

void CEValues::evalUnaryOperation(
    GenPDE::UnaryOperator   op,
    CEVConstPtr             arg1
)
{
    switch (op)
    {
    case GenPDE::Neg:
        evalUnaryOperationEx<GenPDE::Neg>(arg1);
        break;
    case GenPDE::Id:
        evalUnaryOperationEx<GenPDE::Id>(arg1);
        break;
    default:
        Exception::raise("CEValues::evalUnaryOperation", "Unknown operator");
    }
}

void CEValues::rmAVDependency(
    CEVPtr                     res,
    CEVConstPtr                arg,
    CEVConstPtr                av_values, // of the AV to remove
    const std::vector<double>& av_disc_values
)
{
    const VarMemoryLayout& varMemLayoutNew(res->getVarMemoryLayout());
    
    //const AVDependencies&             avDepsNew       = res->getAVDependencies();
    size_t                 avDepsNewNb = varMemLayoutNew.getNbAVDeps();
    //const SVDependencies&             svDepsNew       = res->getSVDependencies();
    //size_t                            nbSVCfgsNew     = svDepsNew.getNbConfigurations();
    Exception::check(
        arg->getVarMemoryLayout().hasSameSVDeps(av_values->getVarMemoryLayout()),
        "CEValues::rmAVDependency",
        "some arguments do not have the same SVDependencies, case not yet handled"
    );
    // For now (performance reasons), we only handle the case where:
    // * #avDepsNew <= 1
    // * avDepsOld doesn't intersect avDepsNew
    Exception::check(avDepsNewNb <= 1, "CEValues::rmAVDependency", "avDepsNewNb > 1, case not yet handled");
    if( 0 == avDepsNewNb )
        return CEValues::rmAVDependency_noAVDepsResult(res, arg, av_values, av_disc_values);
    
    GenPDE::VariableUID newAVUid = varMemLayoutNew.getInnerAV();
    // So av_values also depends on (exactly) newAVUid
    const VarMemoryLayout&             varMemLayoutOld = arg->getVarMemoryLayout();
    Exception::check(
        varMemLayoutOld.isInnerAV(),
        "CEValues::rmAVDependency",
        "Argument must have an inner AV layout"
    );
    //const AVDependencies&             avDepsOld       = arg->getAVDependencies();
    Exception::check(
        !varMemLayoutOld.dependsOn(GenPDE::VT_AuxiliaryVariable, newAVUid),
        "CEValues::rmAVDependency",
        "avDepsNew and avDepsOld intersect, case not yet handled"
    );
    
    // WARNING: the code below is specialized when there's exactly one (new) AV dependency in res.
    size_t nbSVDeps = varMemLayoutNew.getNbDeps() - 1; // the 1 is for the AV...
    const VarMemoryLayout& varMemLayoutAV = av_values->getVarMemoryLayout();
    int newLoopStrides[MAX_NB_SV_DEPS + MAX_NB_AV_DEPS];
    int oldLoopStrides[MAX_NB_SV_DEPS + MAX_NB_AV_DEPS];
    int avLoopStrides[MAX_NB_SV_DEPS + MAX_NB_AV_DEPS];
    varMemLayoutNew.getLoopStrides(varMemLayoutNew, newLoopStrides);
    varMemLayoutOld.getLoopStrides(varMemLayoutNew, oldLoopStrides);
    varMemLayoutAV.getLoopStrides(varMemLayoutNew, avLoopStrides);
    Exception::check(
        1 == newLoopStrides[0] && 1 == avLoopStrides[0] && 0 == oldLoopStrides[0],
        "CEValues::rmAVDependency",
        "Inconsistent AV strides"
    );
    size_t nbNewAVValues = varMemLayoutNew.getSize(0);
    double*       newPtr = res->getDataPtr();
    const double* oldPtr = arg->getDataPtr();
    const double* avPtr  = av_values->getDataPtr();
        
    //AVInterpolatorNaturalCubic                     interp(&av_disc_values[0], av_disc_values.size(), GenPDE::ExtrapolationType_Linear);
    //AVInterpolatorLinear                           interp(&av_disc_values[0], av_disc_values.size(), GenPDE::ExtrapolationType_Linear);
    //AVInterpolatorLinearUniform<GenPDE::Log>       interp(&av_disc_values[0], av_disc_values.size(), GenPDE::ExtrapolationType_Linear);
    AVInterpolatorNaturalCubicUniform<GenPDE::Log> interp(&av_disc_values[0], av_disc_values.size(), GenPDE::ExtrapolationType_Linear);
    switch (nbSVDeps)
    {
    case 1:
    {
        size_t size0 = varMemLayoutNew.getSize(1); // 0 is for the AV...
        int newLoopStride0 = newLoopStrides[1];
        int avLoopStride0  = avLoopStrides[1];
        int oldLoopStride0 = oldLoopStrides[1] + oldLoopStrides[0] * ((int) nbNewAVValues); // b/c we don't increment it in the inner loop
        for(size_t i=0; i<size0; ++i)
        {
            interp.updateY(oldPtr);
            for(size_t a=0; a<nbNewAVValues; ++a)
                *newPtr++ = interp.interpolate(*avPtr++);
            newPtr += newLoopStride0;
            oldPtr += oldLoopStride0;
            avPtr  += avLoopStride0;
        }
        return;
    }
    case 2:
    {
        Exception::notImplemented("CEValues::rmAVDependency");
    }
    case 3:
    {
        Exception::notImplemented("CEValues::rmAVDependency");
    }
    default:
        Exception::raise("CEValues::rmAVDependency", "Expected nb of SV dependencies between 1 and 3");
    }
}

void CEValues::rmAVDependency_noAVDepsResult(
    CEVPtr                     res,
    CEVConstPtr                arg,
    CEVConstPtr                av_values, // of the AV to remove
    const std::vector<double>& av_disc_values
)
{
    /// We know that:
    /// res and av_values have no AVDependencies
    /// arg has exactly one (being removed)
    /// all have exactly the same SVDependencies
    
    // WARNING: the code below is specialized when there's exactly one (new) AV dependency in res.
    const VarMemoryLayout& varMemLayoutNew = res->getVarMemoryLayout();
    const VarMemoryLayout& varMemLayoutOld = arg->getVarMemoryLayout();
    const VarMemoryLayout& varMemLayoutAV  = av_values->getVarMemoryLayout();
    int newLoopStrides[MAX_NB_SV_DEPS + MAX_NB_AV_DEPS];
    int oldLoopStrides[MAX_NB_SV_DEPS + MAX_NB_AV_DEPS];
    int avLoopStrides[MAX_NB_SV_DEPS + MAX_NB_AV_DEPS];
    varMemLayoutNew.getLoopStrides(varMemLayoutNew, newLoopStrides);
    varMemLayoutOld.getLoopStrides(varMemLayoutNew, oldLoopStrides);
    varMemLayoutAV.getLoopStrides(varMemLayoutNew, avLoopStrides);
    size_t nbSVDeps = varMemLayoutNew.getNbDeps();
    double*       newPtr = res->getDataPtr();
    const double* oldPtr = arg->getDataPtr();
    const double* avPtr  = av_values->getDataPtr();
        
    //AVInterpolatorNaturalCubic                   interp(&av_disc_values[0], av_disc_values.size(), GenPDE::ExtrapolationType_Linear);
    //AVInterpolatorLinear                           interp(&av_disc_values[0], av_disc_values.size(), GenPDE::ExtrapolationType_Linear);
    //AVInterpolatorLinearUniform<GenPDE::Log>       interp(&av_disc_values[0], av_disc_values.size(), GenPDE::ExtrapolationType_Linear);
    AVInterpolatorNaturalCubicUniform<GenPDE::Log> interp(&av_disc_values[0], av_disc_values.size(), GenPDE::ExtrapolationType_Linear);
    switch (nbSVDeps)
    {
    case 1:
    {
        size_t size0 = varMemLayoutNew.getSize(0);
        int newLoopStride0 = newLoopStrides[0];
        int oldLoopStride0 = oldLoopStrides[0];
        int avLoopStride0  = avLoopStrides[0];
        Exception::check(
            1 == newLoopStride0 && 1 == avLoopStride0,
            "CEValues::rmAVDependency_noAVDepsResult",
            "Inconsistent strides"
        );
        for(size_t i=0; i<size0; ++i)
        {
            interp.updateY(oldPtr);
            *newPtr++ = interp.interpolate(*avPtr++);
            oldPtr += oldLoopStride0;
        }
        return;
    }
    case 2:
    {
        Exception::notImplemented("CEValues::rmAVDependency_noAVDepsResult");
    }
    case 3:
    {
        Exception::notImplemented("CEValues::rmAVDependency_noAVDepsResult");
    }
    default:
        Exception::raise("CEValues::rmAVDependency_noAVDepsResult", "Expected nb of SV dependencies between 1 and 3");
    }
}

std::ostream& operator<<(std::ostream& stream, const CEValues& ce_values)
{
    stream << ce_values.mVarMemLayout;
    const double* dataPtr = ce_values.getDataPtr();
    size_t size = ce_values.mVarMemLayout.getSize();
    stream << std::setprecision(10);
    for(size_t i=0; i<size; ++i)
        stream << *dataPtr++ << std::endl;
    stream << std::setprecision(6);
    return stream;
}

void CEValues::evalConditional(
    CEVConstPtr             cond,
    CEVConstPtr             arg1,
    CEVConstPtr             arg2
)
{
    int loopingStridesC[MAX_NB_AV_DEPS + MAX_NB_SV_DEPS];
    int loopingStrides1[MAX_NB_AV_DEPS + MAX_NB_SV_DEPS];
    int loopingStrides2[MAX_NB_AV_DEPS + MAX_NB_SV_DEPS];
    cond->mVarMemLayout.getLoopStrides(mVarMemLayout, &loopingStridesC[0]);
    arg1->mVarMemLayout.getLoopStrides(mVarMemLayout, &loopingStrides1[0]);
    arg2->mVarMemLayout.getLoopStrides(mVarMemLayout, &loopingStrides2[0]);
    size_t        nbDeps  = mVarMemLayout.getNbDeps();
    const double* condPtr = cond->getDataPtr();
    const double* arg1Ptr = arg1->getDataPtr();
    const double* arg2Ptr = arg2->getDataPtr();
    double*       resPtr  = getDataPtr();
    
    // By definition, the strides of the result here are 1, 0, 0,...
    switch (nbDeps)
    {
    case 0:
    {
        *resPtr = (*condPtr > 0.5) ? *arg1Ptr : *arg2Ptr;
        return;
    }
    case 1:
    {
        int condStride = loopingStridesC[0];
        int arg1Stride = loopingStrides1[0];
        int arg2Stride = loopingStrides2[0];
        size_t size    = mVarMemLayout.getSize(0);
        for(size_t i=0; i<size; ++i)
        {
            *resPtr++ = (*condPtr > 0.5) ? *arg1Ptr : *arg2Ptr;
            condPtr += condStride;
            arg1Ptr += arg1Stride;
            arg2Ptr += arg2Stride;
        }
        return;
    }
    case 2:
    {
        int condStride0 = loopingStridesC[0];
        int condStride1 = loopingStridesC[1];
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
                *resPtr++ = (*condPtr > 0.5) ? *arg1Ptr : *arg2Ptr;
                condPtr += condStride0;
                arg1Ptr += arg1Stride0;
                arg2Ptr += arg2Stride0;
            }
            condPtr += condStride1;
            arg1Ptr += arg1Stride1;
            arg2Ptr += arg2Stride1;
        }
        return;
    }
    case 3:
    {
        int condStride0 = loopingStridesC[0];
        int condStride1 = loopingStridesC[1];
        int condStride2 = loopingStridesC[2];
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
                    *resPtr++ = (*condPtr > 0.5) ? *arg1Ptr : *arg2Ptr;
                    condPtr += condStride0;
                    arg1Ptr += arg1Stride0;
                    arg2Ptr += arg2Stride0;
                }
                condPtr += condStride1;
                arg1Ptr += arg1Stride1;
                arg2Ptr += arg2Stride1;
            }
            condPtr += condStride2;
            arg1Ptr += arg1Stride2;
            arg2Ptr += arg2Stride2;
        }
        return;
    }
    case 4:
    {
        int condStride0 = loopingStridesC[0];
        int condStride1 = loopingStridesC[1];
        int condStride2 = loopingStridesC[2];
        int condStride3 = loopingStridesC[3];
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
                        *resPtr++ = (*condPtr > 0.5) ? *arg1Ptr : *arg2Ptr;
                        condPtr += condStride0;
                        arg1Ptr += arg1Stride0;
                        arg2Ptr += arg2Stride0;
                    }
                    condPtr += condStride1;
                    arg1Ptr += arg1Stride1;
                    arg2Ptr += arg2Stride1;
                }
                condPtr += condStride2;
                arg1Ptr += arg1Stride2;
                arg2Ptr += arg2Stride2;
            }
            condPtr += condStride3;
            arg1Ptr += arg1Stride3;
            arg2Ptr += arg2Stride3;
        }
        return;
    }
    case 5:
    {
        int condStride0 = loopingStridesC[0];
        int condStride1 = loopingStridesC[1];
        int condStride2 = loopingStridesC[2];
        int condStride3 = loopingStridesC[3];
        int condStride4 = loopingStridesC[4];
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
                            *resPtr++ = (*condPtr > 0.5) ? *arg1Ptr : *arg2Ptr;
                            condPtr += condStride0;
                            arg1Ptr += arg1Stride0;
                            arg2Ptr += arg2Stride0;
                        }
                        condPtr += condStride1;
                        arg1Ptr += arg1Stride1;
                        arg2Ptr += arg2Stride1;
                    }
                    condPtr += condStride2;
                    arg1Ptr += arg1Stride2;
                    arg2Ptr += arg2Stride2;
                }
                condPtr += condStride3;
                arg1Ptr += arg1Stride3;
                arg2Ptr += arg2Stride3;
            }
            condPtr += condStride4;
            arg1Ptr += arg1Stride4;
            arg2Ptr += arg2Stride4;
        }
        return;
    }
    case 6:
    {
        int condStride0 = loopingStridesC[0];
        int condStride1 = loopingStridesC[1];
        int condStride2 = loopingStridesC[2];
        int condStride3 = loopingStridesC[3];
        int condStride4 = loopingStridesC[4];
        int condStride5 = loopingStridesC[5];
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
                                *resPtr++ = (*condPtr > 0.5) ? *arg1Ptr : *arg2Ptr;
                                condPtr += condStride0;
                                arg1Ptr += arg1Stride0;
                                arg2Ptr += arg2Stride0;
                            }
                            condPtr += condStride1;
                            arg1Ptr += arg1Stride1;
                            arg2Ptr += arg2Stride1;
                        }
                        condPtr += condStride2;
                        arg1Ptr += arg1Stride2;
                        arg2Ptr += arg2Stride2;
                    }
                    condPtr += condStride3;
                    arg1Ptr += arg1Stride3;
                    arg2Ptr += arg2Stride3;
                }
                condPtr += condStride4;
                arg1Ptr += arg1Stride4;
                arg2Ptr += arg2Stride4;
            }
            condPtr += condStride5;
            arg1Ptr += arg1Stride5;
            arg2Ptr += arg2Stride5;
        }
        return;
    }
    default:
    {
        Exception::raise("CEValues::evalConditional", "Not implemented for more than 6 variable dependencies");
    }
    }
}

