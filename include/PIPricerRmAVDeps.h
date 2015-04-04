#ifndef PI_PRICER_RM_AVDEPS_H
#define PI_PRICER_RM_AVDEPS_H

#include <vector>
#include <boost/shared_ptr.hpp>

#include "PricingInstruction.h"
#include "PricerUid.h"
#include "AuxiliaryVariable.h"

class PIPricerRmAVDeps : public PricingInstruction
{
public:
    virtual ~PIPricerRmAVDeps() {}
    
    PIPricerRmAVDeps(
        PricerUid                               pricer_uid,
        const std::vector<GenPDE::VariableUID>& av_uids
    );
    
    void apply(const ModelPtr& model, const TradePtr& trade) const;
    
    friend std::ostream& operator<<(std::ostream& stream, const PIPricerRmAVDeps& pi);
    
protected:
    virtual void print(std::ostream& stream) const;
    
protected:
    PricerUid                        mPricerUid;
    std::vector<GenPDE::VariableUID> mAVUids;
};

#endif // PI_PRICER_RM_AVDEPS_H
