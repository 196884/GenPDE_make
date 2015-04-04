#ifndef AV_INTERPOLATOR_INTERFACE_H
#define AV_INTERPOLATOR_INTERFACE_H

class AVInterpolatorInterface
{
public:
    virtual ~AVInterpolatorInterface() {}
    
    virtual void updateY(const double* y_begin) = 0;
    
    virtual double interpolate(double x) const = 0;
};

#endif // AV_INTERPOLATOR_INTERFACE_H
