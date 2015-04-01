/*
 * PDEUpdater1D.h
 *
 *  Created on: Oct 31, 2009
 *      Author: dupontr
 */

#ifndef PDEUPDATER1D_H_
#define PDEUPDATER1D_H_

// Forward declaration
class Solver1DInterface;

/// \brief An abstract class used as a callback by the solver to set the coefficients
/// and boundary conditions of the PDE to solve.
/// Note that the Dirichlet BC has to be set on the boundaries of the <b>current</b> solution,
/// and will be enforced on the <b>next</b> solution (that is, if using Dirichlet you have to
/// override the current solution on the boundary(ies) concerned...)
class PDEUpdater1D
{
public:
	virtual void update(Solver1DInterface& solver) = 0;
};

#endif /* PDEUPDATER1D_H_ */
