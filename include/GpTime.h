//
//  GpTime.h
//  GenPDE
//
//  Created by Regis Dupont on 9/17/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef GPTIME_H
#define GPTIME_H

#include <boost/date_time/posix_time/posix_time.hpp>

#include "Exceptions.h"
#include "GpDate.h"

// boost::posix_time
namespace GenPDE
{
    typedef boost::posix_time::ptime Time;
    
    Time timeFromString(const std::string& s)
    {
        return boost::posix_time::from_iso_string(s);
    }
    
    std::string timeToString(const Time& t)
    {
        return boost::posix_time::to_iso_string(t);
    }
    
    Time timeFromDateAndYearFraction(const Date& date, double year_fraction)
    {
        double dayFraction = year_fraction * GenPDE::NbDaysPerYear;
        long nbDays;
        if( dayFraction > 0 )
            nbDays = (long) (dayFraction + 0.5 / 60 / 60 / 24);
        else
            nbDays = (long) (dayFraction - 0.5 / 60 / 60 / 24);
        Date resDate = dateAddDays(date, nbDays);
        dayFraction -= (double) nbDays;
        double hourFraction = 24 * dayFraction;
        int hours    = (int) (hourFraction + 0.5 / 60 / 60);
        double minFraction = (hourFraction - ((double) hours)) * 60;
        int mins     = (int) (minFraction + 0.5 / 60);
        double secFraction = (minFraction - ((double) mins)) * 60;
        int secs     = (int) (secFraction + 0.5);
        //Exception::notImplemented("timeFromDateAndYearFraction");
        boost::posix_time::ptime result(resDate, boost::posix_time::time_duration(hours, mins, secs, 0));
        return result;
    }
}

#endif // GPTIME_H
