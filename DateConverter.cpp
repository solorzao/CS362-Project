#include <ctime>
#include "DateConverter.hpp"

// Converts current time into TwoIntDate structure
// Ref: http://www.cplusplus.com/reference/ctime/localtime/
Date GetCurrentDate()
{
    time_t now;
    tm timeinfo;

    // Get current time as time_t
    time(&now);
    localtime_s(&timeinfo, &now);

    return Date(1900 + timeinfo.tm_year, timeinfo.tm_yday);
}

// Make TwoIntDate from 3 ints: MM,DD,YYYY
Date MakeDate(int month, int day, int year)
{
    tm* newDate = new tm();
    
    newDate->tm_mon = month - 1;
    newDate->tm_mday = day;
    newDate->tm_year = year - 1900;
    mktime(newDate);    // fills in rest of fields in newDate
    
    return Date(year, newDate->tm_yday);
}

// Get number of days separating two TwoIntDates (absolute value)
int GetDaysBetween(Date date1, Date date2)
{
    if (date1.Year == date2.Year)
    {
        if (date1.Days >= date2.Days)
        {
            return date1.Days - date2.Days;
        }
        else
        {
            return date2.Days - date1.Days;
        }
    }
    
    int result = 0, leap = 0;  
    int laterDays = date1.Days, laterYear = date1.Year, 
        earlierDays = date2.Days, earlierYear = date2.Year;

    if (date1.Year < date2.Year)
    {
        laterDays = date2.Days;
        laterYear = date2.Year;
        earlierDays = date1.Days;
        earlierYear = date1.Year;
    }
    
    result += laterDays;    // days elapsed in the later year entered
    for (int year = earlierYear; year < laterYear; ++year)
    {
        leap = (year % 4 == 0) ? 1 : 0; // for leap years add 1 to days left in year

        if (year > earlierYear)   // Full year
        {
            result += 365 + leap;
        }
        else    // Partial year (days left in the earlier year entered)
        {
            result += 365 + leap - earlierDays;
        }
    }

    return result;
}

// Compare 2 TwoIntDates, return -1 if first date before, 0 if equal, 1 if later
int CompareDates(Date date1, Date date2)
{
    if (date1.Year < date2.Year)
    {
        return -1;
    }
    if (date1.Year > date2.Year)
    {
        return 1;
    }
    if (date1.Days < date2.Days)
    {
        return -1;
    }
    if (date1.Days > date2.Days)
    {
        return 1;
    }
    return 0;
}

// Add a number of days to a TwoIntDate (returns new TwoIntDate)
Date AddDaysToDate(Date date, int days)
{
    int newDays = date.Days + days;
    int leap = (date.Year % 4 == 0) ? 1 : 0; // for leap years add 1 to days left in year
    int newYear = date.Year + (newDays / (365 + leap)); // increment year if necessary
    newDays = newDays - ((newYear - date.Year) * (365 + leap));
    return Date(newYear, newDays);
}

// Get a string from TwoIntDate in form "MM/DD/YYYY"
string GetDateString(Date date)
{
    tm* newDate = new tm();
    // NOTE: mktime ignores tm_ydays! But it does interpret tm_mday as days since 1st of tm_mon
    // so here I have to re-interpret Days as days since Jan 1st (+1 cuz 1/1 is day 0)
    newDate->tm_mon = 0;
    newDate->tm_mday = date.Days + 1;
    newDate->tm_year = date.Year - 1900;
    mktime(newDate);

    return "" + std::to_string(newDate->tm_mon + 1) + "/" + std::to_string(newDate->tm_mday)
        + "/" + std::to_string(date.Year);
}

// Get the ints from a Date (pass ints y and d by reference)
void GetDateInts(Date date, int* y, int* d)
{
    *y = date.Year;
    *d = date.Days;
}

Date GetDateFromString(string dateStr)
{
    return MakeDate(std::stoi(dateStr.substr(0, 2)), 
        std::stoi(dateStr.substr(3,2)), std::stoi(dateStr.substr(6, 4)));
}
