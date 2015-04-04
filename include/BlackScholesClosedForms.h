#ifndef BLACK_SCHOLES_CLOSED_FORMS_H
#define BLACK_SCHOLES_CLOSED_FORMS_H

/// Computation of misc. closed forms in the Black-Scholes naive model
class BlackScholes
{
public:
	static double callPV(
		double spot,
		double rate,
		double volatility,
		double tenor,
		double strike)
	{
		const double df(Double::exp(-rate * tenor));
		const double a(Double::log(spot / (strike * df)));
		const double variance(volatility * volatility * tenor);
		const double num1(a + 0.5 * variance);
		const double num2(num1 - variance);
		const double denom(1.0 / Double::sqrt(variance));
		const double d1(num1 * denom);
		const double d2(num2 * denom);
		const double lhs(spot * Double::normalCdf(d1));
		const double rhs(df * strike * Double::normalCdf(d2));
		return lhs - rhs;
	}
    
	static double digitalCallPV(
		double spot,
		double rate,
		double volatility,
		double tenor,
		double strike)
	{
		const double df(Double::exp(-rate * tenor));
		const double a(Double::log(spot / (strike * df)));
		const double variance(volatility * volatility * tenor);
		const double num2(a - 0.5 * variance);
		const double d2(num2 / Double::sqrt(variance));
		return df * Double::normalCdf(d2);
	}
    
	static double putPV(
		double spot,
		double rate,
		double volatility,
		double tenor,
		double strike)
	{
		const double df(Double::exp(-rate * tenor));
		return strike * df - spot + callPV(spot, rate, volatility, tenor, strike);
	}
};

#endif // BLACK_SCHOLES_CLOSED_FORMS_H
