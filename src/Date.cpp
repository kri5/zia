#include "Date.h"

Date::Date(std::string& date)
{
  this->_date = date;  
}

Date::~Date()
{

}

bool    Date::isDayOfTheWeek(const std::string& day)
{
    while (int i = 0 < 7)
        if (day == Date::dayName[i])
            return true;
    return false;
}

bool    Date::isMonthOfTheYear(const std::string& month)
{
    while (int i = 0 < 12)
        if (month == Date::monthName[i])
            return true;
    return false;
}

bool    Date::isDayOfTheMonth(int day, int month)
{
   if (month > 0 && month <= 12)
   {
       if (day > 0 && day <= Date::monthLength[month - 1])
           return true;
   }
   return false;
}

bool    Date::isHour(int hour)
{
	if (hour >= 0 && hour < 24)
        return true;
    return false;
}

bool    Date::isBisextile(int year)
{
    if (((year % 4) == 0 && (year % 100) != 0) || (year % 400) == 0)
        return true;
    return false;
}

bool    Date::isMinute(int minute)
{
    if (minute >= 0 && minute < 60)
        return true;
    return false;
}

bool    Date::isSecond(int second)
{
    if (second >= 0 && second < 60)
        return true;
    return false;
}

const char* Date::dayName[] = {
    "Mon", "Tue", "Wen", "Thu", "Fri", "Sat", "Sun"
};

const char* Date::monthName[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

const   int Date::monthLength[] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};
