#include "Framework.h"
#include "AuxiliaryVariable.h"

AuxiliaryVariables::AuxiliaryVariables(const std::vector<AVConstPtr>& avs)
{
    for(AVConstPtr av : avs)
        m_avMap[av->getUid()] = av;
}

AuxiliaryVariables::AVConstPtr AuxiliaryVariables::getAuxiliaryVariable(GenPDE::VariableUID uid) const
{
    AVMap::const_iterator it = m_avMap.find(uid);
    if( m_avMap.end() == it )
        Exception::raise("AuxiliaryVariables::getAuxiliaryVariable", "Could not find specified AV");
    return it->second;
}

void AuxiliaryVariables::getUids(std::vector<GenPDE::VariableUID>& uids) const
{
    uids.resize(m_avMap.size());
    size_t currIdx = 0;
    AVMap::const_iterator itCurr;
    AVMap::const_iterator itEnd = m_avMap.end();
    for( itCurr = m_avMap.begin(); itEnd != itCurr; ++itCurr )
        uids[currIdx++] = itCurr->first;
}

size_t AuxiliaryVariables::getNbAVs() const
{
    return m_avMap.size();
}
