#ifndef PDE_SOLVER_1D_INTERFACES_H
#define PDE_SOLVER_1D_INTERFACES_H

#include <vector> 

/// An abstract class used as the interface available on the solver
/// from the PDE updating classes.
class Solver1DInterface
{
public:
	virtual double getCurrentTime() const = 0;
	virtual double getNextTime() const = 0;
	virtual std::vector<double>& getDiffusionCoefs() = 0;
	virtual std::vector<double>& getConvectionCoefs() = 0;
	virtual std::vector<double>& getVariableCoefs() = 0;
    // The time grid used in the solver is in days... we might want to divide by NbDaysPerYear!
    virtual void   setTimeScaling(double time_scaling) = 0;
};

#endif // PDE_SOLVER_1D_INTERFACES_H
