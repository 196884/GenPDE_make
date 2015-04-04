#ifndef DOUBLE_H
#define DOUBLE_H

#include <cmath>
#include <boost/math/distributions/normal.hpp> // for normal_distribution

/// \brief A collection of misc. function to handle doubles
class Double
{
public:
	inline static double max(double a, double b) { return (a > b) ? a : b; }

	inline static double min(double a, double b) { return (a < b) ? a : b; }

	inline static double abs(double x) { return (x > 0) ? x : -x; }

	inline static double sqrt(double x) { return std::sqrt(x); }

	inline static double log(double x) { return std::log(x); }

	inline static double exp(double x) { return std::exp(x); }

	inline static double normalCdf(double x)
	{
		static boost::math::normal s;
		return boost::math::cdf(s, x);
	}

	inline static double normalPdf(double x)
	{
		static boost::math::normal s;
		return boost::math::pdf(s, x);
	}

	inline static bool isEqual(double a, double b, double c) { return abs(a-b) <= c; }
};

#endif // DOUBLE_H
