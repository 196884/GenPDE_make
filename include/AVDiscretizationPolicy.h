#ifndef AV_DISCRETIZATION_POLICY_H
#define AV_DISCRETIZATION_POLICY_H

#include "AVContext.h"
#include "PDEPricingModelInterface.h"
#include "AuxiliaryVariable.h"
#include "CEValues.h"

class AVDiscretizationPolicy
{
public:
    enum Type {
        Type_None      = 0,
        Type_Hardcoded = 1,
        Type_Sum       = 2,
        Type_Average   = 3
    };

public:
    AVDiscretizationPolicy( Type type );
    virtual ~AVDiscretizationPolicy() {}

    Type getType() const;

protected:
    Type m_type;
};

class AVDP_None : public AVDiscretizationPolicy
{
public:
    AVDP_None();
};

class AVDP_Hardcoded : public AVDiscretizationPolicy
{
public:
    AVDP_Hardcoded();
    virtual ~AVDP_Hardcoded();

    // Only sets the missing ones
    virtual void setAVDiscretizations( const AuxiliaryVariables& av_defs, AVContext& av_context ) const;

    void setAVDiscretizationValues(GenPDE::VariableUID av_uid, const std::vector<double>& values);

protected:
    AVContext* m_avContext;
};

class AVDP_Sum : public AVDiscretizationPolicy
{
public:
    AVDP_Sum(
        size_t nb_values,
        double nb_std_devs
    );

    virtual ~AVDP_Sum();

    size_t getNbValues()  const;
    double getNbStdDevs() const;

protected:
    size_t m_nbValues;
    double m_nbStdDevs;
};

namespace AVDP
{
    template <typename MODEL>
    void discretizeSum(
        const AVDP_Sum&           params,
        const AuxiliaryVariables& av_defs,
        MODEL&                    model,
        AVContext&                av_context
    )
    {
        const GenPDE::Date& pricingDate               = model.getPricingDate();
        double spot                                   = model.getSpot();
        const AuxiliaryVariables::AVUidVector& avUids = av_defs.getUids();
        double knownSum                               = 0.0;
        double indexMult                              = 0.0;

        for( GenPDE::VariableUID avUid : avUids )
        {
            const GenPDE::Date& avDate           = av_defs.getAuxiliaryVariable( avUid )->getDate();
            AVContext::CEVConstPtr currentValue  = av_context.evalCE( avUid );
            if( avDate <= pricingDate )
            {
                // The AV should be known already:
                knownSum = currentValue->getValue(); // throws if not single value

            } else {
                if( 0 == currentValue->getVarMemoryLayout().getSize() )
                {
                    // The point is in the future: we need to discretize
                    double variance   = model.getVariance(avDate);
                    double stdDev     = Double::sqrt(variance);
                    indexMult        += 1;
                    double halfWidth  = params.getNbStdDevs() * stdDev;
                    double lowerBound = Double::log(knownSum + indexMult * spot) - halfWidth;
                    size_t nbValues   = params.getNbValues();
                    double ds         = 2 * halfWidth / ((double) (nbValues - 1));
                    std::vector<double> avValues(nbValues);
                    for(size_t j = 0; j < nbValues; ++j)
                        avValues[j] = Double::exp(lowerBound + j * ds);
                    av_context.setAVDiscretizationValues( avUid, avValues );
                }
            }
        }
    }
}; // namespace AVDP



#endif // AV_DISCRETIZATION_POLICY_H
