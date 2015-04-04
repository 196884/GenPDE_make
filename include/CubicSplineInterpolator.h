#ifndef CUBIC_SPLINE_INTERPOLATOR_H
#define CUBIC_SPLINE_INTERPOLATOR_H

/// \brief The kind of extrapolation to use
enum ExtrapolationType
{
	ExtrapolateFlat = 0,
	ExtrapolateLinear,
    ExtrapolateNone,  // will throw!
};

/// \brief An implementation of a cubic spline interpolator,
/// as per 'Numerical Recipes in C'
class CubicSplineInterpolator
{
public:
	/// Constructor
	CubicSplineInterpolator(
        const double*              x,
        const double*              y,
        size_t                     n,
		ExtrapolationType          extrapolation_type);
    
    CubicSplineInterpolator(
        const double*     x,
        size_t            n,
        ExtrapolationType extrapolation_type
    );

	/// Interpolation function
	double interpolate(double x) const;
    
    /// Keeps the same x-points, updates the function.
    void updateY(const double* );

private:
	void computeY2();
    void computeAB(); // the extrapolation parameters

private:
	size_t                    mSize;
	const double*             mX;
	const double*             mY;
	ExtrapolationType         mExtrapolationType;

	std::vector<double>       mY2;
    std::vector<double>       mAuxY2;
	double                    mABegin;
	double                    mBBegin;
	double                    mAEnd;
	double                    mBEnd;
};



/// \brief An implementation of a cubic spline interpolator,
/// as per 'Numerical Recipes in C'
class CubicSplineInterpolatorCaching
{
public:
	/// Constructor
	CubicSplineInterpolatorCaching(
        const double*              x,
        const double*              y,
        size_t                     n,
		ExtrapolationType          extrapolation_type);
    
    CubicSplineInterpolatorCaching(
        const double*     x,
        size_t            n,
        ExtrapolationType extrapolation_type
    );

	/// Interpolation function
	double interpolate(double x) const;
    
    /// Keeps the same x-points, updates the function.
    void updateY(const double* );

private:
	void computeY2();
    void computeAB(); // the extrapolation parameters
    void computeXData();

private:
	size_t                    mSize;
	const double*             mX;
	const double*             mY;
	ExtrapolationType         mExtrapolationType;

	std::vector<double>       mY2;
    std::vector<double>       mAuxY2;
    std::vector<double>       mInvDX;    // mInvDX[i]    = 1 / mDX[i];
    std::vector<double>       mInvDX2_6; // mInvDX2_6[i] = 6 / (mX[i+2] - mX[i]);
    std::vector<double>       mSig;      // mSig[i]      = mDX[i] * mInvDX2[i];
	double                    mABegin;
	double                    mBBegin;
	double                    mAEnd;
	double                    mBEnd;
    double                    mXLower;
    double                    mXUpper;
};

#endif // CUBIC_SPLINE_INTERPOLATOR_H
