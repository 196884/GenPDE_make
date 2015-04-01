/*
 * PDESolver1D.cpp
 *
 *  Created on: Oct 30, 2009
 *      Author: dupontr
 */

#include "Framework.h"

#include "PDESolver1D.h"

#include <sstream>
#include "TridiagonalRoutines.h"

#include <iostream>

using namespace std;

PDESolver1D::PDESolver1D(
		const std::vector<double>&             space_grid,
		const std::vector<double>&             time_grid,
        PDEUpdater1D*                          pde_updater,
		double                                 theta,
		bool                                   is_lower_bc_dirichlet,
		bool                                   is_upper_bc_dirichlet
    )
    :mTimeGrid(time_grid)
    ,mCurrentTimeIndex(time_grid.size()-1)
	,mSpaceGrid(space_grid)
	,mSpaceSize(space_grid.size())
	,mSpaceOperator(3 * space_grid.size())
	,mIsLowerBCDirichlet(is_lower_bc_dirichlet)
	,mIsUpperBCDirichlet(is_upper_bc_dirichlet)
	,mPDEUpdater(pde_updater)
	,mDefaultTheta(theta)
    ,mTimeScaling(1.0)
	,mNextAvailableSolSetUid(0)
	,mDiffusionCoefs(space_grid.size())
	,mConvectionCoefs(space_grid.size())
	,mVariableCoefs(space_grid.size())
{
	// A few sanity checks
	if ( mDefaultTheta < 0.0 || mDefaultTheta > 1.0 )
		Exception::raise("PDESolver1D::CTOR", "theta must be in the range [0.0, 1.0]");

	// Construction of the finite differences
	discretizeFiniteDiffOps();
}

size_t PDESolver1D::getSpaceGridSize() const
{
    return mSpaceSize;
}

double PDESolver1D::getCurrentTime() const
{
	return mTimeGrid[mCurrentTimeIndex];
}

double PDESolver1D::getNextTime() const
{
	size_t index(mCurrentTimeIndex);
	if ( index > 0 )
		--index;
	return mTimeGrid[index];
}

std::vector<double>& PDESolver1D::getDiffusionCoefs()
{
	return mDiffusionCoefs;
}

std::vector<double>& PDESolver1D::getConvectionCoefs()
{
	return mConvectionCoefs;
}

std::vector<double>& PDESolver1D::getVariableCoefs()
{
	return mVariableCoefs;
}

void PDESolver1D::discretizeFiniteDiffOps()
{
	mFiniteDiffOpD1.resize(3 * mSpaceSize);
	mFiniteDiffOpD2.resize(3 * mSpaceSize);

	// The first D1 is done upwind
	const double invDiffFront(1.0 / (mSpaceGrid[1] - mSpaceGrid[0]));
	mFiniteDiffOpD1[0] = 0.0;
	mFiniteDiffOpD1[1] = mIsLowerBCDirichlet ? 0 : -invDiffFront;
	mFiniteDiffOpD1[2] = mIsLowerBCDirichlet ? 0 : +invDiffFront;
	// The first D2 is never used, and set to zero
	mFiniteDiffOpD2[0] = 0.0;
	mFiniteDiffOpD2[1] = 0.0;
	mFiniteDiffOpD2[2] = 0.0;

	// Generic case
	for (size_t gridIdx=1; gridIdx<mSpaceSize-1; ++gridIdx)
	{
		const size_t midIdx(3 * gridIdx + 1);
		const double e1(mSpaceGrid[gridIdx] - mSpaceGrid[gridIdx - 1]);
		const double e2(mSpaceGrid[gridIdx + 1] - mSpaceGrid[gridIdx]);
		const double e1Sq(e1 * e1);
		const double e2Sq(e2 * e2);
		const double e1e2(e1 * e2);
		const double multFactorD1(1.0 / (e1e2 * (e1 + e2)));
		mFiniteDiffOpD1[midIdx - 1] = -e2Sq         * multFactorD1;
		mFiniteDiffOpD1[midIdx    ] = (e2Sq - e1Sq) * multFactorD1;
		mFiniteDiffOpD1[midIdx + 1] = +e1Sq         * multFactorD1;

		const double multFactorD2(2.0 * multFactorD1);
		mFiniteDiffOpD2[midIdx - 1] = +e2        * multFactorD2;
		mFiniteDiffOpD2[midIdx    ] = -(e1 + e2) * multFactorD2;
		mFiniteDiffOpD2[midIdx + 1] = +e1        * multFactorD2;
	}

	// The last D1 is done downwind
	const double invDiffBack(1.0 / (mSpaceGrid[mSpaceSize - 1] - mSpaceGrid[mSpaceSize - 2]));
	mFiniteDiffOpD1[3 * mSpaceSize - 3] = mIsUpperBCDirichlet ? 0 : -invDiffBack;
	mFiniteDiffOpD1[3 * mSpaceSize - 2] = mIsUpperBCDirichlet ? 0 : +invDiffBack;
	mFiniteDiffOpD1[3 * mSpaceSize - 1] = 0.0;
	// The last D2 is never used, and set to zero
	mFiniteDiffOpD2[3 * mSpaceSize - 3] = 0.0;
	mFiniteDiffOpD2[3 * mSpaceSize - 2] = 0.0;
	mFiniteDiffOpD2[3 * mSpaceSize - 1] = 0.0;
}

void PDESolver1D::setSpaceOperator()
{
	// The space operator is sized in the constructor, no need to check here!
	double* spaceOpPos(&mSpaceOperator[0]);
	const double* diffCoefPos(&mDiffusionCoefs[0]);
	const double* convCoefPos(&mConvectionCoefs[0]);
	const double* varCoefPos(&mVariableCoefs[0]);
	const double* d1Pos(&mFiniteDiffOpD1[0]);
	const double* d2Pos(&mFiniteDiffOpD2[0]);
	for (size_t i=mSpaceSize; i>0; --i)
	{
		*spaceOpPos++ = *diffCoefPos   * *d2Pos++  +  *convCoefPos   * *d1Pos++;
		*spaceOpPos++ = *diffCoefPos   * *d2Pos++  +  *convCoefPos   * *d1Pos++  +  *varCoefPos++;
		*spaceOpPos++ = *diffCoefPos++ * *d2Pos++  +  *convCoefPos++ * *d1Pos++;
	}
}

SolutionSetUid PDESolver1D::addSolutionSet(size_t nb_planes)
{
    SolutionSetUid result = mNextAvailableSolSetUid++;
    boost::shared_ptr<PlanesSolution1D> newSolution(new PlanesSolution1D(mSpaceSize, nb_planes));
    mSolutions[result] = newSolution;
    return result;
}

void PDESolver1D::removeSolutionSet(SolutionSetUid uid)
{
	mSolutions.erase(uid);
}

double* PDESolver1D::getSolutionSetPtr(SolutionSetUid uid)
{
	std::map<SolutionSetUid, boost::shared_ptr<PlanesSolution1D> >::const_iterator it(mSolutions.find(uid));
	if ( it == mSolutions.end() )
	{
		std::ostringstream e;
		e << "could not find solution with solution_set_uid = ";
		e << uid;
		Exception::raise("PDESolver1D::getSolutionSetPtr", e.str());
	}
	return it->second->getCurrentSolutionPtr();
}

const double* PDESolver1D::getSolutionSetConstPtr(SolutionSetUid uid) const
{
	std::map<SolutionSetUid, boost::shared_ptr<PlanesSolution1D> >::const_iterator it(mSolutions.find(uid));
	if ( it == mSolutions.end() )
	{
		std::ostringstream e;
		e << "could not find solution with solution_set_uid = ";
		e << uid;
		Exception::raise("PDESolver1D::getSolutionSetPtr", e.str());
	}
	return it->second->getCurrentSolutionPtr();
}

void PDESolver1D::timeStepOnce(bool use_rannacher)
{
	if ( mCurrentTimeIndex == 0 )
		Exception::raise("PDESolver1D::timeStepOnce", "the first time node has already been reached, cannot step further");
	if ( mSolutions.empty() )
	{
		--mCurrentTimeIndex;
		return;
	}

	mPDEUpdater->update(*this);
	setSpaceOperator();
	const double dt(mTimeScaling * (mTimeGrid[mCurrentTimeIndex] - mTimeGrid[mCurrentTimeIndex - 1]));
	const double theta(use_rannacher ? 1.0 : mDefaultTheta);
	double* const spaceOpBegin(&(mSpaceOperator[0]));

	if ( theta == 1.0 )
	{
		////////////////////////////////////////////
		// Special case of a purely implicit step //
		////////////////////////////////////////////
		// We set the space operator:
		double* spaceOpPtr(spaceOpBegin);
		for (size_t i=mSpaceSize; i>0; --i)
		{
			*spaceOpPtr++ *= dt;
			*spaceOpPtr    = 1 + dt * *spaceOpPtr;
			++spaceOpPtr;
			*spaceOpPtr++ *= dt;
		}
		std::map<size_t, boost::shared_ptr<PlanesSolution1D> >::iterator solCurr(mSolutions.begin());
		std::map<size_t, boost::shared_ptr<PlanesSolution1D> >::const_iterator solEnd(mSolutions.end());
		// The first set of planes is treated apart, as we use it to also decompose the space operator
		{
			size_t nbPlanes(solCurr->second->getNbPlanes());
			double* currSolPtr(solCurr->second->getCurrentSolutionPtr());
			Tridiagonal::decomposeAndSolve(spaceOpBegin, currSolPtr, mSpaceSize, nbPlanes);
		}
		for (++solCurr; solCurr != solEnd; ++solCurr)
		{
			size_t nbPlanes(solCurr->second->getNbPlanes());
			double* currSolPtr(solCurr->second->getCurrentSolutionPtr());
			Tridiagonal::solveFromDecomposed(spaceOpBegin, currSolPtr, mSpaceSize, nbPlanes);
		}
	} else {
		////////////////////////////////////////////////////////
		// Normal theta-scheme step (explicit, then implicit) //
		////////////////////////////////////////////////////////

		///////////////////
		// Explicit part //
		///////////////////
		const double explicitMult((theta - 1) * dt);
		std::map<size_t, boost::shared_ptr<PlanesSolution1D> >::iterator solCurr(mSolutions.begin());
		std::map<size_t, boost::shared_ptr<PlanesSolution1D> >::const_iterator solEnd(mSolutions.end());
		for (; solCurr != solEnd; ++solCurr)
		{
			double* sPtr(solCurr->second->getCurrentSolutionPtr());
			double* wPtr(solCurr->second->getWorkingMemoryPtr());
			const size_t nbPlanes(solCurr->second->getNbPlanes());
			Tridiagonal::apply(
				spaceOpBegin,
				sPtr,
				wPtr,
				mSpaceSize,
				nbPlanes);
			for (size_t i=mSpaceSize * nbPlanes; i>0; --i, ++wPtr)
				*wPtr = *sPtr++ + explicitMult * *wPtr;
		}

		if ( theta > 0.0 )
		{
			///////////////////
			// Implicit part //
			///////////////////

			// We set the space operator
			const double implicitMult(theta * dt);
			double* spaceOpPtr(spaceOpBegin);
			for (size_t i=mSpaceSize; i>0; --i)
			{
				*spaceOpPtr++ *= implicitMult;
				*spaceOpPtr    = 1.0 + implicitMult * *spaceOpPtr;
				++spaceOpPtr;
				*spaceOpPtr++ *= implicitMult;
			}
			std::map<size_t, boost::shared_ptr<PlanesSolution1D> >::iterator solCurr(mSolutions.begin());
			std::map<size_t, boost::shared_ptr<PlanesSolution1D> >::const_iterator solEnd(mSolutions.end());
			// The first set of planes is treated apart, as we use it to also decompose the space operator
			{
				size_t nbPlanes(solCurr->second->getNbPlanes());
				double* currWMPtr(solCurr->second->getWorkingMemoryPtr());
				Tridiagonal::decomposeAndSolve(spaceOpBegin, currWMPtr, mSpaceSize, nbPlanes);
				solCurr->second->swap();
			}
			for (++solCurr; solCurr != solEnd; ++solCurr)
			{
				size_t nbPlanes(solCurr->second->getNbPlanes());
				double* currWMPtr(solCurr->second->getWorkingMemoryPtr());
				Tridiagonal::solveFromDecomposed(spaceOpBegin, currWMPtr, mSpaceSize, nbPlanes);
				solCurr->second->swap();
			}
		}
	}
	--mCurrentTimeIndex;
}

size_t PDESolver1D::getTimeIndex() const
{
    return mCurrentTimeIndex;
}
