#ifndef PLANES_SOLUTION_1D_H
#define PLANES_SOLUTION_1D_H

/// \brief A class to represent a solution (along a set of planes) for a 1D PDE.
/// Note that auxiliary memory is also stored, and can be used to handle Dirichlet
/// boundary conditions
class PlanesSolution1D
{
public:
	/// Constructor
	PlanesSolution1D(
		size_t space_size,
		size_t nb_planes = 1)
		:mSpaceSize(space_size)
		,mNbPlanes(nb_planes)
		,mCurrentSolution(new std::vector<double>(space_size * nb_planes))
		,mWorkingMemory(new std::vector<double>(space_size * nb_planes))
	{}

	/// Getting the space size
	///
	/// \result					the space size
	size_t getSpaceSize() const { return mSpaceSize; }

	/// Getting the number of planes
	///
	/// \result 				the number of planes
	size_t getNbPlanes() const { return mNbPlanes; }

	/// Getting a raw pointer to the current solution
	double* getCurrentSolutionPtr() { return &(*mCurrentSolution)[0]; }

	/// Getting a raw pointer to the working memory
	double* getWorkingMemoryPtr() { return &(*mWorkingMemory)[0]; }

	/// Swapping the current solution and the working memory
	void swap() { mCurrentSolution.swap(mWorkingMemory); }

private:
	size_t                                  mSpaceSize;
	size_t                                  mNbPlanes;
	boost::shared_ptr<std::vector<double> > mCurrentSolution;
	boost::shared_ptr<std::vector<double> > mWorkingMemory;
};

#endif // PLANES_SOLUTION_1D_H
