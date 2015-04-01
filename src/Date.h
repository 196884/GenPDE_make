//
//  Date.h
//  GenPDE
//
//  Created by Regis Dupont on 8/22/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef DATE_H_
#define DATE_H_

#include "boost/date_time/gregorian/gregorian.hpp"

namespace GenPDE
{
    typedef boost::gregorian::date Date;
    // format is YYYYMMDD
    Date dateFromString(const std::string& s);
    std::string dateToString(const Date& date);
    /// d1 - d2, in days
    long dateDifferenceInDays(const Date& d1, const Date& d2);
    Date dateAddDays(const Date& d, long n);
    static double NbDaysPerYear(365.0);
    static double OneDayInYears(1.0 / 365.0);
}

#endif // DATE_H_