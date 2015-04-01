//
//  PDEPricerSolutionStorage.h
//  GenPDE
//
//  Created by Regis Dupont on 8/31/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef PDEPRICERSOLUTIONSTORAGE_H_
#define PDEPRICERSOLUTIONSTORAGE_H_

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

#endif // PDEPRICERSOLUTIONSTORAGE_H_