#include <algorithm>

#include "Framework.h"
#include "AuxiliaryVariable.h"

AuxiliaryVariables::AuxiliaryVariables(const std::vector<AVConstPtr>& avs)
{
    for(AVConstPtr av : avs)
        m_avMap[av->getUid()] = av;
    topoSort();
}

AuxiliaryVariables::AVConstPtr AuxiliaryVariables::getAuxiliaryVariable(GenPDE::VariableUID uid) const
{
    AVMap::const_iterator it = m_avMap.find(uid);
    if( m_avMap.end() == it )
        Exception::raise("AuxiliaryVariables::getAuxiliaryVariable", "Could not find specified AV");
    return it->second;
}

void AuxiliaryVariables::topoSort()
{
    // For now, we assume that there are no 2 AVs with the same date
    // (throwing if it's not the case).
    // For the more general case, this is just topological sorting (though
    // for now we don't even have a query on hte AV definitions to infer the
    // graph topology...)
    size_t nbAVs = m_avMap.size();

    // We create a vector of AVs that we're going to sort by date:
    std::vector<AVConstPtr> avs(nbAVs);
    AVMap::const_iterator itCurr;
    AVMap::const_iterator itEnd = m_avMap.end();
    size_t currIdx = 0;
    for( itCurr = m_avMap.begin(); itEnd != itCurr; ++itCurr )
        avs[currIdx++] = itCurr->second;

    std::sort(
        avs.begin(),
        avs.end(),
        [](AVConstPtr a, AVConstPtr b) { return a->getDate() < b->getDate(); }
    );

    // We check for multiple AVs with the same date:
    for(size_t i = 1; i < nbAVs; ++i)
        Exception::check(
            avs[i-1]->getDate() < avs[i]->getDate(),
            "AuxiliaryVariables::getUids",
            "Could not order AVs by dependencies"
        );

    m_avUidVector.resize(nbAVs);
    for(currIdx = 0; currIdx < nbAVs; ++currIdx)
        m_avUidVector[currIdx] = avs[currIdx]->getUid();
}

const AuxiliaryVariables::AVUidVector& AuxiliaryVariables::getUids() const
{
    return m_avUidVector;
}

size_t AuxiliaryVariables::getNbAVs() const
{
    return m_avMap.size();
}
