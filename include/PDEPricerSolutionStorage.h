#ifndef PDE_PRICER_SOLUTION_STORAGE_H
#define PDE_PRICER_SOLUTION_STORAGE_H

#include <map>

#include "PricerUid.h"

/// A singleton
class PDEPricerSolutionStorage
{
private:
    typedef std::pair<double, boost::shared_ptr<CEValues> > ValuesAtTime;
    typedef std::vector<ValuesAtTime>                       PricerValues;
public:
private:
    /// Private constructor...
    PDEPricerSolutionStorage() {}
    
	/// The method to access the singleton instance
	/// \result					the singleton instance
	static PDEPricerSolutionStorage& instance()
	{
		static PDEPricerSolutionStorage result;
		return result;
	}

private:
    std::map<PricerUid, PricerValues> mAllPricerValues;
};

#endif // PDE_PRICER_SOLUTION_STORAGE_H
