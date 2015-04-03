#ifndef AV_DISCRETIZATION_POLICY_H
#define AV_DISCRETIZATION_POLICY_H

#include "AVContext.h"
#include "PDEPricingModelInterface.h"
#include "AuxiliaryVariable.h"

class AVDiscretizationPolicy
{
public:
    virtual ~AVDiscretizationPolicy() {}

    // Does not keep ownership of the AVContext*
    virtual AVContext* discretizeAVs(
        PDEPricingModelInterface&  model,
        const AuxiliaryVariables&  avs
    ) = 0;
};

class AVDiscretizationPolicyHardcoded : public AVDiscretizationPolicy
{
public:
    AVDiscretizationPolicyHardcoded();

    virtual AVContext* discretizeAVs(
        PDEPricingModelInterface&  model,
        const AuxiliaryVariables&  avs
    );

    void setAVDiscretizationValues(GenPDE::VariableUID av_uid, const std::vector<double>& values);

private:
    AVContext* m_avContext;
};

#endif // AV_DISCRETIZATION_POLICY_H
