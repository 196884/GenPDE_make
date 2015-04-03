#ifndef VARIABLE_DEPENDENCIES_H
#define VARIABLE_DEPENDENCIES_H

#include <utility>
#include <vector>

#include "Framework.h"
#include "VariableEnums.h"
#include "VariableBindings.h"
#include "AuxiliaryVariable.h"
#include "StateVariable.h"

class VarDependencies
{
public:
    VarDependencies()
    :mNbSVDeps(0)
    ,mNbAVDeps(0)
    {}
  
    VarDependencies(
        GenPDE::VariableType type,
        GenPDE::VariableUID  uid,
        size_t               nb_values
    )
    :mNbSVDeps(0)
    ,mNbAVDeps(0)
    {
        if( 1 == nb_values )
            return;
        switch (type)
        {
        case GenPDE::VT_StateVariable:
            mNbSVDeps  = 1;
            mSVDeps[0] = uid;
            mSVDeps[1] = nb_values;
            break;
        case GenPDE::VT_AuxiliaryVariable:
            mNbAVDeps  = 1;
            mAVDeps[0] = uid;
            mAVDeps[1] = nb_values;
            break;
        default:
            Exception::raise("VarDependencies::CTOR", "Unknown type");
        }
    }
    
    VarDependencies(
        const VarDependencies& deps1,
        const VarDependencies& deps2
    )
    {
        mNbSVDeps = mergeDeps(&mSVDeps[0], &(deps1.mSVDeps[0]), deps1.mNbSVDeps, &(deps2.mSVDeps[0]), deps2.mNbSVDeps, MAX_NB_SV_DEPS);
        mNbAVDeps = mergeDeps(&mAVDeps[0], &(deps1.mAVDeps[0]), deps1.mNbAVDeps, &(deps2.mAVDeps[0]), deps2.mNbAVDeps, MAX_NB_AV_DEPS);
    }
    
    VarDependencies(
        const VarDependencies& deps1,
        const VarDependencies& deps2,
        const VarDependencies& deps3
    )
    {
        VarDependencies vdAux(deps1, deps2);
        mNbSVDeps = mergeDeps(&mSVDeps[0], &(vdAux.mSVDeps[0]), vdAux.mNbSVDeps, &(deps3.mSVDeps[0]), deps3.mNbSVDeps, MAX_NB_SV_DEPS);
        mNbAVDeps = mergeDeps(&mAVDeps[0], &(vdAux.mAVDeps[0]), vdAux.mNbAVDeps, &(deps3.mAVDeps[0]), deps3.mNbAVDeps, MAX_NB_AV_DEPS);
    }
    
    bool dependsOn(GenPDE::VariableType type, GenPDE::VariableUID uid) const
    {
        if( GenPDE::VT_AuxiliaryVariable )
        {
            for(size_t i=0; i<mNbAVDeps; ++i)
            {
                if( mAVDeps[2 * i] == uid )
                    return true;
                if( uid > mAVDeps[2 * i] )
                    return false;
            }
            return false;
        } else {
            for(size_t i=0; i<mNbSVDeps; ++i)
            {
                if( mSVDeps[2 * i] == uid )
                    return true;
                if( uid > mSVDeps[2 * i] )
                    return false;
            }
            return false;
        }
    }
    
    VarDependencies removeAVDependency(GenPDE::VariableUID av_uid) const
    {
        VarDependencies result(*this);
        result.mNbAVDeps = 0;
        for(size_t avIdx=0; avIdx<mNbAVDeps; ++avIdx)
        {
            if( av_uid == mAVDeps[ 2 * avIdx ] )
                continue;
            result.mAVDeps[ 2 * result.mNbAVDeps     ] = mAVDeps[ 2 * avIdx     ];
            result.mAVDeps[ 2 * result.mNbAVDeps + 1 ] = mAVDeps[ 2 * avIdx + 1 ];
            ++result.mNbAVDeps;
        }
        return result;
    }
    
    bool hasSameSVDeps(const VarDependencies& deps) const
    {
        if( mNbSVDeps != deps.mNbSVDeps )
            return false;
        for(size_t i=0; i<mNbSVDeps; ++i)
            if( mSVDeps[2 * i] != deps.mSVDeps[2 * i] )
                return false;
        return true;
    }
    
    size_t getNbAVDeps() const { return mNbAVDeps; }
    
    size_t getNbAVConfigurations() const
    {
        size_t result = 1;
        for(size_t i=0; i<mNbAVDeps; ++i)
            result *= mAVDeps[2 * i + 1];
        return result;
    }
    
protected:
    static size_t mergeDeps(
        size_t*       deps_data,
        const size_t* deps1_data,
        size_t        nb_deps1,
        const size_t* deps2_data,
        size_t        nb_deps2,
        size_t        max_nb_deps
    )
    {
        size_t depsIdx1(0);
        size_t depsIdx2(0);
        size_t nb_deps(0);
        while( depsIdx1 < nb_deps1 && depsIdx2 < nb_deps2 )
        {
            Exception::check(nb_deps < max_nb_deps, "VarDependencies::mergeDeps", "Reached max. number of variable dependencies");
            if( deps1_data[ 2 * depsIdx1 ] <= deps2_data[ 2 * depsIdx2 ] )
            {
                if( deps1_data[ 2 * depsIdx1 ] == deps2_data[ 2 * depsIdx2 ] )
                    ++depsIdx2;
                deps_data[ 2 * nb_deps     ] = deps1_data[ 2 * depsIdx1     ];
                deps_data[ 2 * nb_deps + 1 ] = deps1_data[ 2 * depsIdx1 + 1 ];
                ++depsIdx1;
                ++nb_deps;
            } else {
                deps_data[ 2 * nb_deps     ] = deps2_data[ 2 * depsIdx2     ];
                deps_data[ 2 * nb_deps + 1 ] = deps2_data[ 2 * depsIdx2 + 1 ];
                ++depsIdx2;
                ++nb_deps;
            }
        }
        while( depsIdx1 < nb_deps1 )
        {
            Exception::check(nb_deps < max_nb_deps, "VarDependencies::mergeDeps", "Reached max. number of variable dependencies");
            deps_data[ 2 * nb_deps     ] = deps1_data[ 2 * depsIdx1     ];
            deps_data[ 2 * nb_deps + 1 ] = deps1_data[ 2 * depsIdx1 + 1 ];
            ++depsIdx1;
            ++nb_deps;
        }
        while( depsIdx2 < nb_deps2 )
        {
            Exception::check(nb_deps < max_nb_deps, "VarDependencies::mergeDeps", "Reached max. number of variable dependencies");
            deps_data[ 2 * nb_deps     ] = deps2_data[ 2 * depsIdx2     ];
            deps_data[ 2 * nb_deps + 1 ] = deps2_data[ 2 * depsIdx2 + 1 ];
            ++depsIdx2;
            ++nb_deps;
        }
        return nb_deps;
    }

    friend std::ostream& operator<<(std::ostream& stream, const VarDependencies& deps);
    
protected:
    // mSVDeps[ 2 * i     ] is the UID of the variable
    // mSVDeps[ 2 * i + 1 ] is the number of discretized values
    size_t mSVDeps[2 * MAX_NB_SV_DEPS];
    size_t mAVDeps[2 * MAX_NB_AV_DEPS];
    size_t mNbSVDeps;
    size_t mNbAVDeps;
};

class VarMemoryLayout : public VarDependencies
{
public:
    VarMemoryLayout()
    :VarDependencies()
    ,mIsInnerAV(true)
    ,mNbDeps(0)
    ,mSize(1)
    {}
    
    VarMemoryLayout(
        GenPDE::VariableType type,
        GenPDE::VariableUID  uid,
        size_t               nb_values
    )
    :VarDependencies(type, uid, nb_values)
    ,mIsInnerAV(true)
    {
        init();
    }
    
    VarMemoryLayout(
        const VarDependencies& deps1,
        bool                   is_inner_av = true
    )
    :VarDependencies(deps1)
    ,mIsInnerAV(is_inner_av)
    {
        init();
    }
    
    VarMemoryLayout(
        const VarDependencies& deps1,
        const VarDependencies& deps2,
        bool                   is_inner_av = true
    )
    :VarDependencies(deps1, deps2)
    ,mIsInnerAV(is_inner_av)
    {
        init();
    }
    
    VarMemoryLayout(
        const VarDependencies& deps1,
        const VarDependencies& deps2,
        const VarDependencies& deps3,
        bool                   is_inner_av = true
    )
    :VarDependencies(deps1, deps2, deps3)
    ,mIsInnerAV(is_inner_av)
    {
        init();
    }
    
    size_t getNbDeps() const { return mNbDeps; }
    
    size_t getSize(size_t dep_idx) const { return mSizes[dep_idx]; }
    
    size_t getSize() const { return mSize; }
    
    bool isInnerAV() const { return mIsInnerAV; }
    
    GenPDE::VariableUID getInnerAV() const
    {
        Exception::check(mNbAVDeps > 0, "VarMemoryLayout::getInnerAV", "No AV dependency");
        return mAVDeps[0];
    }
    
    void getLoopStrides(const VarMemoryLayout& vml, int* loop_strides) const
    {
        size_t avIdx    = 0;
        size_t svIdx    = 0;
        int prevOffset  = 0;
        
        if( vml.mIsInnerAV )
        {
            for(size_t i=0; i<vml.mNbAVDeps; ++i)
            {
                size_t avUid = vml.mAVDeps[2*i];
                while( avIdx < mNbAVDeps && mAVDeps[2*avIdx] < avUid )
                    ++avIdx;
                if( avIdx < mNbAVDeps && mAVDeps[2*avIdx] == avUid )
                {
                    loop_strides[i] = ((int) mAVStrides[avIdx]) + prevOffset;
                    prevOffset      = -((int) (mAVStrides[avIdx] * mAVSizes[avIdx]));
                    ++avIdx;
                } else {
                    loop_strides[i] = prevOffset;
                    prevOffset      = 0;
                }
            }
            for(size_t i=0; i<vml.mNbSVDeps; ++i)
            {
                size_t svUid = vml.mSVDeps[2*i];
                while( svIdx < mNbSVDeps && mSVDeps[2*svIdx] < svUid )
                    ++svIdx;
                if( svIdx < mNbSVDeps && mSVDeps[2*svIdx] == svUid )
                {
                    loop_strides[vml.mNbAVDeps + i] = ((int) mSVStrides[svIdx]) + prevOffset;
                    prevOffset                      = -((int) (mSVStrides[svIdx] * mSVSizes[svIdx]));
                    ++svIdx;
                } else {
                    loop_strides[vml.mNbAVDeps + i] = prevOffset;
                    prevOffset                      = 0;
                }
            }
        } else {
            for(size_t i=0; i<vml.mNbSVDeps; ++i)
            {
                size_t svUid = vml.mSVDeps[2*i];
                while( svIdx < mNbSVDeps && mSVDeps[2*svIdx] < svUid )
                    ++svIdx;
                if( svIdx < mNbSVDeps && mSVDeps[2*svIdx] == svUid )
                {
                    loop_strides[i] = ((int) mSVStrides[svIdx]) + prevOffset;
                    prevOffset      = -((int) (mSVStrides[svIdx] * mSVSizes[svIdx]));
                    ++svIdx;
                } else {
                    loop_strides[i] = prevOffset;
                    prevOffset      = 0;
                }
            }
            for(size_t i=0; i<vml.mNbAVDeps; ++i)
            {
                size_t avUid = vml.mAVDeps[2*i];
                while( avIdx < mNbAVDeps && mAVDeps[2*avIdx] < avUid )
                    ++avIdx;
                if( avIdx < mNbAVDeps && mAVDeps[2*avIdx] == avUid )
                {
                    loop_strides[vml.mNbSVDeps + i] = ((int) mAVStrides[avIdx]) + prevOffset;
                    prevOffset                      = -((int) (mAVStrides[avIdx] * mAVSizes[avIdx]));
                    ++avIdx;
                } else {
                    loop_strides[vml.mNbSVDeps + i] = prevOffset;
                    prevOffset                      = 0;
                }
            }
        }
    }
    
    size_t getIndex(const VarBindings& var_bindings) const
    {
        size_t result = 0;
        for(size_t avIdx=0; avIdx<mNbAVDeps; ++avIdx)
        {
            size_t avUid = mAVDeps[2*avIdx];
            result += var_bindings.getAVIndex(avUid) * mAVStrides[avIdx];
        }
        for(size_t svIdx=0; svIdx<mNbSVDeps; ++svIdx)
        {
            size_t svUid = mSVDeps[2*svIdx];
            result += var_bindings.getSVIndex(svUid) * mSVStrides[svIdx];
        }
        return result;
    }
    
    friend std::ostream& operator<<(std::ostream& stream, const VarMemoryLayout& layout);
    
protected:
    /// Sets mNbDeps, mSizes and mStrides from the other members
    void init()
    {
        mNbDeps       = mNbSVDeps + mNbAVDeps;
        size_t stride = 1;
        if( mIsInnerAV )
        {
            for(size_t i=0; i<mNbAVDeps; ++i)
            {
                mSizes[i]   = mAVDeps[2*i+1];
                mStrides[i] = stride;
                stride     *= mSizes[i];
            }
            for(size_t i=0; i<mNbSVDeps; ++i)
            {
                mSizes[mNbAVDeps + i]   = mSVDeps[2*i+1];
                mStrides[mNbAVDeps + i] = stride;
                stride                 *= mSizes[mNbAVDeps + i];
            }
            mAVSizes   = &mSizes[0];
            mSVSizes   = &mSizes[mNbAVDeps];
            mAVStrides = &mStrides[0];
            mSVStrides = &mStrides[mNbAVDeps];
        } else {
            for(size_t i=0; i<mNbSVDeps; ++i)
            {
                mSizes[i]   = mSVDeps[2*i+1];
                mStrides[i] = stride;
                stride     *= mSizes[i];
            }
            for(size_t i=0; i<mNbAVDeps; ++i)
            {
                mSizes[mNbSVDeps + i]   = mAVDeps[2*i+1];
                mStrides[mNbSVDeps + i] = stride;
                stride                 *= mSizes[mNbSVDeps + i];
            }
            mAVSizes   = &mSizes[mNbSVDeps];
            mSVSizes   = &mSizes[0];
            mAVStrides = &mStrides[mNbSVDeps];
            mSVStrides = &mStrides[0];
        }
        mSize = stride;
    }
    
protected:
    // mIsInnerAV is True iff the memory is organized with the AVs having the smallest strides
    // (this is what the solver expects to vectorize the PDE solving).
    bool    mIsInnerAV;
    size_t  mNbDeps;
    size_t  mSize;
    size_t  mSizes[MAX_NB_SV_DEPS + MAX_NB_AV_DEPS];
    size_t  mStrides[MAX_NB_SV_DEPS + MAX_NB_AV_DEPS];
    size_t* mAVSizes;
    size_t* mSVSizes;
    size_t* mAVStrides;
    size_t* mSVStrides;
};

#endif // VARIABLE_DEPENDENCIES_H
