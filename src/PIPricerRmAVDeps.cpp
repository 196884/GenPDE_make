//
//  PIPricerRmAVDeps.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/26/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "Framework.h"
#include "PIPricerRmAVDeps.h"
#include "PayoutExpression.h"

PIPricerRmAVDeps::PIPricerRmAVDeps(
    PricerUid                               pricer_uid,
    const std::vector<GenPDE::VariableUID>& av_uids
)
:mPricerUid(pricer_uid)
,mAVUids(av_uids)
{}

void PIPricerRmAVDeps::apply(const ModelPtr& model) const
{
    for(GenPDE::VariableUID avUid : mAVUids)
    {
        CEVPtr          oldPricer(model->getPricer(mPricerUid));
        VarMemoryLayout oldAVDeps(oldPricer->getVarMemoryLayout());
        if( !oldAVDeps.dependsOn(GenPDE::VT_AuxiliaryVariable, avUid) )
            return; // Nothing to be done: there really isn't a dependency!
        if( avUid != oldAVDeps.getInnerAV() )
            Exception::raise("PIPricerRmAVDeps::apply", "Can only remove innermost AV");
        boost::shared_ptr<const AuxiliaryVariable> avDetails = model->getAuxiliaryVariable(avUid);
        CEVConstPtr     avValues(avDetails->getDefinition()->evalCE(model));
        VarDependencies newAVDeps(oldAVDeps.removeAVDependency(avUid), avValues->getVarMemoryLayout());
        
        PricerUid      tmpUid;
        CEVPtr         tmpPricer(model->addTempPricer(newAVDeps, tmpUid));
        CEValues::rmAVDependency(
            tmpPricer,
            oldPricer,
            avValues,
            avDetails->getDiscretizationValues()
        );
        model->renamePricer(tmpUid, mPricerUid);
    }
}

void PIPricerRmAVDeps::print(std::ostream& stream) const
{
    stream << "<PricerRmAVDeps uid=\"" << mPricerUid << "\" AVs=\"";
    if( mAVUids.size() == 0 )
        Exception::raise("PIPricerRmAVDeps::print", "No AV Uids found");
    stream << mAVUids.front();
    for(size_t i=1; i<mAVUids.size(); ++i)
        stream << "," << mAVUids[i];
    stream << "\"/>";
}

std::ostream& operator<<(std::ostream& stream, const PIPricerRmAVDeps& pi)
{
    pi.print(stream);
    return stream;
}
