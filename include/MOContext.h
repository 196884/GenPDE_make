#ifndef MO_CONTEXT_H
#define MO_CONTEXT_H

#include <boost/shared_ptr.hpp>

#include "MarketObservable.h"

class CEValues;

class MOContext
{
protected:
    typedef boost::shared_ptr<const CEValues> CEVConstPtr;
    
public:
    virtual CEVConstPtr evalCE(MOUid uid) const = 0;
};

#endif //  MO_CONTEXT_H
