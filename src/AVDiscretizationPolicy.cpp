#include "AVDiscretizationPolicy.h"

AVDiscretizationPolicyHardcoded::AVDiscretizationPolicyHardcoded()
:m_avContext(new AVContext())
{}

AVContext* AVDiscretizationPolicyHardcoded::discretizeAVs(
    PDEPricingModelInterface&  model,
    const AuxiliaryVariables&  avs
)
{
    return m_avContext;
}

void AVDiscretizationPolicyHardcoded::setAVDiscretizationValues(GenPDE::VariableUID av_uid, const std::vector<double>& values)
{
    m_avContext->setAVDiscretizationValues(av_uid, values);
}

