#ifndef PDE_SOLVER_INTERFACE_BASE_H
#define PDE_SOLVER_INTERFACE_BASE_H

#include <vector>

typedef size_t SolutionSetUid;

class PDESolverInterfaceBase
{
public:
    virtual                   ~PDESolverInterfaceBase() {}

    virtual size_t            getNbDimensions() const = 0;
    virtual size_t            getStateGridSize(size_t dimension) const = 0;
    
    virtual SolutionSetUid    addSolutionSet(size_t nb_planes = 1) = 0;
    virtual double*           getSolutionSetPtr(SolutionSetUid uid) = 0;
    virtual const double*     getSolutionSetConstPtr(SolutionSetUid uid) const = 0;
    virtual void              removeSolutionSet(SolutionSetUid uid) = 0;
    
    virtual void              timeStepOnce(bool use_rannacher = false) = 0;
    virtual size_t            getTimeIndex() const = 0;
};


#endif // PDE_SOLVER_INTERFACE_BASE_H
