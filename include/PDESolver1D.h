#ifndef PDE_SOLVER_1D_H
#define PDE_SOLVER_1D_H

#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>

#include "PDESolverInterfaceBase.h"
#include "PDESolver1DInterfaces.h"

class PDEUpdater1D;
class PlanesSolution1D;

/// The main PDE solver class in 1D
///
/// Important features:
/// * the space grid is passed at the construction of the object,
///   and cannot be changed later;
/// * the time grid is passed at the construction of the object;
/// * the PDE solved is hardcoded to contain 3 space terms (diffusion, convection and variable coefficients);
/// * the solving scheme used is a classic 'theta-scheme'.
class PDESolver1D : public PDESolverInterfaceBase, Solver1DInterface
{
public:
	/// Constructor
	PDESolver1D(
		const std::vector<double>&             space_grid,
        const std::vector<double>&             time_grid,
        PDEUpdater1D*                          pde_updater,
		double                                 theta                 = 0.5,
		bool                                   is_lower_bc_dirichlet = false,
		bool                                   is_upper_bc_dirichlet = false
    );
    
    size_t getNbDimensions() const { return 1; }
    
    size_t getStateGridSize(size_t dimension) const { return (dimension == 0) ? mSpaceSize : 1; }
    
    /// Getting the size of the space grid (reminder: it cannot be changed!!!)
    size_t getSpaceGridSize() const;

	/// Getting the current time
	double getCurrentTime() const;

	/// Getting the next time (returns the current time if the end of the time grid has been reached)
	double getNextTime() const;

	/// Getting the diffusion coefficients
	std::vector<double>& getDiffusionCoefs();

	/// Getting the convection coefficients
	std::vector<double>& getConvectionCoefs();

	/// Getting the variable coefficients
	std::vector<double>& getVariableCoefs();

    SolutionSetUid addSolutionSet(size_t nb_planes = 1);

	void removeSolutionSet(SolutionSetUid uid);

	double* getSolutionSetPtr(SolutionSetUid uid);
    
	const double* getSolutionSetConstPtr(SolutionSetUid uid) const;

	void timeStepOnce(bool use_rannacher = false);
    
    size_t getTimeIndex() const;
    
    void   setTimeScaling(double time_scaling) { mTimeScaling = time_scaling; }

private:
	/// Helper function to handle the discretization of the discrete finite difference operators,
	/// at the construction of the object.
	/// f(x-e1) = f(x) - e1 f'(x) + e1^2/2 f''(x) - e1^3/6 f'''(x)...
	/// f(x)    = f(x)
	/// f(x+e2) = f(x) + e2 f'(x) + e2^2/2 f''(x) + e2^3/6 f'''(x)...
	/// f'(x)   ~ (e1^2 f(x+e2) - e2^2 f(x-e1) + (e2^2 - e1^2) f(x)) / (e1 e2 (e1 + e2))
	/// f''(x)  ~ (e1   f(x+e2) + e2   f(x-e1) - (e1 + e2) f(x)) / (e1 e2)
	void discretizeFiniteDiffOps();

	/// Setting the space operator from the 3 coefficients
	/// (assumes the coefficients are up to date).
	void setSpaceOperator();

private:
	/// The time nodes
	std::vector<double>       mTimeGrid;

	/// The current time index
	size_t                    mCurrentTimeIndex;

	/// The space grid (cannot ever be changed!)
	const std::vector<double> mSpaceGrid;

	/// The size of the space grid
	const size_t              mSpaceSize;

	/// The first order discrete finite difference operator
	std::vector<double>       mFiniteDiffOpD1;

	/// The second order discrete finite difference operator
	std::vector<double>       mFiniteDiffOpD2;

	/// The space operator (tridiagonal)
	std::vector<double>       mSpaceOperator;

	/// Is the lower boundary condition Dirichlet?
	const bool                mIsLowerBCDirichlet;

	/// Is the upper boundary condition Dirichlet?
	const bool                mIsUpperBCDirichlet;

	/// A class to call back into to update the PDE coefficients / Dirichlet BCs
    PDEUpdater1D*             mPDEUpdater;

	/// The theta for theta-scheme
	const double              mDefaultTheta;

	double                    mTimeScaling;

	/// The next available <tt>SolutionSetUid</tt>
	SolutionSetUid            mNextAvailableSolSetUid;

	/// The set of solutions (current, and auxiliary, i.e. working space for solving)
	std::map<SolutionSetUid, boost::shared_ptr<PlanesSolution1D> > mSolutions;

	/// The diffusion coefficients
	/// Note that the coefficients at the boundaries will <b>always</b> be disregarded.
	std::vector<double>       mDiffusionCoefs;

	/// The convection coefficients
	/// Note that the coefficients at the boundaries will be used only when the corresponding BC
	/// has been set to non-Dirichlet.
	std::vector<double>       mConvectionCoefs;

	/// The variable coefficients
	/// Note that the coefficients at the boundaries will be used only when the corresponding BC
	/// has been set to non-Dirichlet.
	std::vector<double>       mVariableCoefs;
};

#endif // PDE_SOLVER_1D_H
