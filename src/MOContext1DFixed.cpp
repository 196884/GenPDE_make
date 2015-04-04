#include "MOContext1DFixed.h"
#include "VariableDependencies.h"
#include "CEValuesStored.h"

MOContext1DFixed::MOContext1DFixed(MOUid uid, const std::vector<double>& values)
:mUid(uid)
{
    VarDependencies svDep(GenPDE::VT_StateVariable, 0, values.size());
    mValues = boost::shared_ptr<CEValues>(new CEValuesStored(svDep));
    std::copy(values.begin(), values.end(), mValues->getDataPtr());
}
