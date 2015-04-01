//
//  AVInterpolatorInterface.h
//  GenPDE
//
//  Created by Regis Dupont on 8/30/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef AVINTERPOLATORINTERFACE_H_
#define AVINTERPOLATORINTERFACE_H_

class AVInterpolatorInterface
{
public:
    virtual ~AVInterpolatorInterface() {}
    
    virtual void updateY(const double* y_begin) = 0;
    
    virtual double interpolate(double x) const = 0;
};

#endif // AVINTERPOLATORINTERFACE_H_