#include "BSModelParameters.h"

BSModelParameters::BSModelParameters()
:m_spot(         0 )
,m_riskFreeRate( 0 )
,m_volatility(   0 )
{}

BSModelParameters::BSModelParameters(
    double spot,
    double risk_free_rate,
    double volatility
)
:m_spot(         spot           )
,m_riskFreeRate( risk_free_rate )
,m_volatility(   volatility     )
{}

double BSModelParameters::getSpot() const
{
    return m_spot;
}

double BSModelParameters::getRiskFreeRate() const
{
    return m_riskFreeRate;
}

double BSModelParameters::getVolatility() const
{
    return m_volatility;
}

