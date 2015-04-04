#ifndef MO_CONTEXT_1D_FIXED_H
#define MO_CONTEXT_1D_FIXED_H

#include <vector>

#include "Framework.h"
#include "MOContext.h"

class MOContext1DFixed : public MOContext
{
public:
    MOContext1DFixed(MOUid uid, const std::vector<double>& values);
    
    virtual CEVConstPtr evalCE(MOUid uid) const
    {
        if( uid != mUid )
            Exception::raise("MOContext1DFixed::MOContext1DFixed", "Uid not supported");
        return mValues;
    }
    
protected:
    MOUid                       mUid;
    boost::shared_ptr<CEValues> mValues;
};

#endif // MO_CONTEXT_1D_FIXED_H
