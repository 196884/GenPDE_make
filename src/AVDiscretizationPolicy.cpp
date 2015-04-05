#include "AVDiscretizationPolicy.h"
#include "CEValues.h"

AVDiscretizationPolicy::AVDiscretizationPolicy( Type type )
:m_type( type )
{}

AVDiscretizationPolicy::Type AVDiscretizationPolicy::getType() const
{
    return m_type;
}

AVDP_None::AVDP_None()
:AVDiscretizationPolicy( Type_None )
{}

AVDP_Hardcoded::AVDP_Hardcoded()
:AVDiscretizationPolicy( Type_Hardcoded  )
,m_avContext(            new AVContext() )
{
    // Note: only owns the context until 'discretizeAVs' is called
}

AVDP_Hardcoded::~AVDP_Hardcoded()
{}

void AVDP_Hardcoded::setAVDiscretizations(
    const AuxiliaryVariables&  av_defs,
    AVContext&                 av_context
) const
{
    const AuxiliaryVariables::AVUidVector& avUids = av_defs.getUids();
    for( GenPDE::VariableUID avUid : avUids )
    {
        AVContext::CEVConstPtr currentValues = av_context.evalCE( avUid );
        if( !currentValues )
        {
            // Nothing set yet in the target context, we try to get a value from the
            // current context
            av_context.setAVDiscretizationValues( avUid, m_avContext->evalCE( avUid ) );
        }
    }
}

void AVDP_Hardcoded::setAVDiscretizationValues(GenPDE::VariableUID av_uid, const std::vector<double>& values)
{
    m_avContext->setAVDiscretizationValues(av_uid, values);
}

AVDP_Sum::AVDP_Sum(
    size_t nb_values,
    double nb_std_devs
)
:AVDiscretizationPolicy( Type_Sum    )
,m_nbValues(             nb_values   )
,m_nbStdDevs(            nb_std_devs )
{}

AVDP_Sum::~AVDP_Sum()
{}

size_t AVDP_Sum::getNbValues() const
{
    return m_nbValues;
}

double AVDP_Sum::getNbStdDevs() const
{
    return m_nbStdDevs;
}
