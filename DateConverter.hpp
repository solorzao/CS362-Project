#pragma once

#include <string>
using std::string;

struct Date
{
    int Year;
    int Days;
    Date(int y, int d) : Year(y), Days(d) { }
};

// Converts current time into TwoIntDate structure
Date GetCurrentDate();

// Make TwoIntDate from 3 ints: MM,DD,YYYY
Date MakeDate(int month, int day, int year);

// Get number of days separating two TwoIntDates (absolute value)
int GetDaysBetween(Date date1, Date date2);

// Compare 2 TwoIntDates, return -1 if first date before, 0 if equal, 1 if later
int CompareDates(Date date1, Date date2);

// Add a number of days to a TwoIntDate (returns new TwoIntDate)
Date AddDaysToDate(Date date, int days);

// Get a string from TwoIntDate in form "MM/DD/YYYY"
string GetDateString(Date date);

// Get the ints from a Date (pass ints y and d by reference)
void GetDateInts(Date date, int* y, int* d);

// Return Date object from string in format of "MM/DD/YYYY"
Date GetDateFromString(string dateStr);