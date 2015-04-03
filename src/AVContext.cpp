#include "VariableDependencies.h"
#include "CEValuesStored.h"
#include "AVContext.h"

AVContext::AVContext()
{}

AVContext::~AVContext()
{}

void AVContext::setAVDiscretizationValues(GenPDE::VariableUID av_uid, const std::vector<double>& values)
{
    VarDependencies avDep(GenPDE::VT_AuxiliaryVariable, av_uid, values.size());
    boost::shared_ptr<CEValues> cev(new CEValuesStored(avDep));
    std::copy(values.begin(), values.end(), cev->getDataPtr());
    m_avValuesMap[av_uid] = cev;
}

AVContext::CEVConstPtr AVContext::evalCE(GenPDE::VariableUID av_uid) const
{
    AVValuesMap::const_iterator it = m_avValuesMap.find(av_uid);
    if( m_avValuesMap.end() == it )
        Exception::raise("AVContext::evalCE", "Could not find values for specified AV");
    return it->second;
}
