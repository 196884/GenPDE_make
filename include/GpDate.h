#ifndef GP_DATE_
#define GP_DATE_

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

#endif // GP_DATE_
