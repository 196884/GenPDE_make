//
//  PIPricerRmAVDeps.h
//  GenPDE
//
//  Created by Regis Dupont on 8/26/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef PIPRICERRMAVDEPS_H_
#define PIPRICERRMAVDEPS_H_

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

#endif // PIPRICERRMAVDEPS_H_

