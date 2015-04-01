//
//  PDESolver1DInterfaces.h
//  GenPDE
//
//  Created by Regis Dupont on 8/24/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef PDESOLVER1DINTERFACES_H_
#define PDESOLVER1DINTERFACES_H_

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

#endif // PDESOLVER1DINTERFACES_H_