//
//  CEDiscontinuities.h
//  GenPDE
//
//  Created by Regis Dupont on 9/21/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef CEDISCONTINUITIES_H_
#define CEDISCONTINUITIES_H_

/// Represents a discontinuity on a 1D (SV) grid
class CEDiscontinuity1D
{
public:
    CEDiscontinuity1D(
        size_t sv_index_lower,
        double diff_lower,
        double diff_upper
    )
    :mSVIdxLower(sv_index_lower)
    ,mDiffLower(diff_lower)
    ,mDiffUpper(diff_upper)
    {}
    
    /*
    template <GenPDE::BinaryOperator OP>
    void adjustValue(
        double*       values_ptr,
        size_t        values_stride,
        const double* sv_grid,
    )
    */
protected:
    /// The index of the SV grid point immediately *before* the discontinuity
    size_t mSVIdxLower;
    
    /// The values of the 2 differences immediately before and immediately after the discontinuity
    /// The differences are defined as first argument minus second argument
    double mDiffLower;
    double mDiffUpper;
};

#endif // CEDISCONTINUITIES_H_