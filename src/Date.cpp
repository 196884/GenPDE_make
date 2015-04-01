//
//  Date.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/22/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include "Date.h"

GenPDE::Date GenPDE::dateFromString(const std::string& s)
{
    return boost::gregorian::from_undelimited_string(s);
}

std::string GenPDE::dateToString(const GenPDE::Date& date)
{
    return boost::gregorian::to_iso_string(date);
}

long GenPDE::dateDifferenceInDays(const Date& d1, const Date& d2)
{
    return (d1 - d2).days();
}

GenPDE::Date GenPDE::dateAddDays(const Date& d, long n)
{
    boost::gregorian::date_duration dd(n);
    return d + dd;
}

//double GenPDE::NbDaysPerYear  = 365.0;

//double GenPDE::NbYearsPerDay  = 1.0 / 365.0;
