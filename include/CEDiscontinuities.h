#ifndef CE_DISCONTINUITIES_H
#define CE_DISCONTINUITIES_H

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

#endif // CE_DISCONTINUITIES_H
