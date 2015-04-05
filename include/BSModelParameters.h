#ifndef BS_MODEL_PARAMETERS_H
#define BS_MODEL_PARAMETERS_H

class BSModelParameters
{
public:
    BSModelParameters();

    BSModelParameters(
        double spot,
        double risk_free_rate,
        double volatility
    );

    virtual ~BSModelParameters() {}

    double getSpot()         const;
    double getRiskFreeRate() const;
    double getVolatility()   const;

protected:
    double m_spot;
    double m_riskFreeRate;
    double m_volatility;
};

#endif // BS_MODEL_PARAMETERS_H
