#include "Date.h"

Date::Date(std::string& date)
{
    std::string     tmp;

    this->_date = date;
    std::cout << date << std::endl;
    this->feed(date);
    this->readIdentifier(tmp);
    this->peekChar();
    this->saveContextPub();
    if (this->readIdentifier(tmp)
            && this->isDayOfTheWeek(tmp))
    {
        if (this->peekChar() == ',')
        {
            if (this->readInteger(tmp)
                    && this->isDayOfTheMonth(atoi(tmp.c_str()), 1))
            {
                this->_day = atoi(tmp.c_str());
                if (this->readIdentifier(tmp)
                        && this->isMonthOfTheYear(tmp))
                {
                    this->_month = getMonthFromString(tmp);
                    if (this->readInteger(tmp))
                    {
                        this->_year = atoi(tmp.c_str());
                        this->readTime(tmp);
                    }
                }
            }
        }
    }
    this->restoreContextPub();
    std::cout << this->toString() << std::endl;
    std::cout << this->toTimestamp() << std::endl;
    //std::cout << this->_day << ' ' << this->_month << ' ' << this->_year
    //    << ' ' << this->_hours << ':' << this->_minutes << ':' << this->_seconds << std::endl;
}

void    Date::readTime(std::string& t)
{
    std::string tmp;

    this->saveContextPub();
    if (this->readInteger(tmp)
            && this->isHour(atoi(tmp.c_str())))
    {
        this->_hours = atoi(tmp.c_str());
        if (this->readChar() == ':')
        {
            this->peekChar();
            if (this->readInteger(tmp)
                    && this->isMinute(atoi(tmp.c_str())))
            {
                this->_minutes = atoi(tmp.c_str());
                if (this->readChar() == ':')
                {
                    this->peekChar();
                    if (this->readInteger(tmp)
                            && this->isSecond(atoi(tmp.c_str())))
                    {
                        this->_seconds = atoi(tmp.c_str());
                    }
                }
            }
        }
    }
    this->restoreContextPub();
}

Date::~Date()
{

}

std::string     Date::toString()
{
    std::stringstream ss;
    std::string res;

    ss << this->_day << ' ' << this->monthName[this->_month - 1] << ' '
        << this->_year << ' ' << this->_hours << ':' << this->_minutes << ':'
        << this->_seconds;
    res = ss.str();
    return res;   
}

time_t  Date::toTimestamp()
{
    struct  tm  t;
    t.tm_sec = this->_seconds;
    t.tm_min = this->_minutes;
    t.tm_hour = this->_hours;
    t.tm_year = this->_year - 1900;
    t.tm_mon = this->_month - 1;
    t.tm_mday = this->_day;
    std::cout << t.tm_hour << ':' << t.tm_min << ':' << t.tm_sec << ' ' << t.tm_mon << ' ' << t.tm_year << std::endl;
    return mktime(&t);
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
    int i = 0;
    while (i < 12)
        if (month == Date::monthName[i++])
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

int     Date::getMonthFromString(std::string month)
{
    int i = 0;
    while (i < 12)
        if (month == Date::monthName[i++])
            return i;
    return 0;
}

int     Date::getSeconds()  const
{
    return this->_seconds;
}

int     Date::getMinutes()  const
{
    return this->_minutes;
}

int     Date::getHours()    const
{
    return this->_hours;
}

int     Date::getDay()  const
{
    return this->_day;
}

int     Date::getMonth()    const
{
    return this->_month;
}

int     Date::getYear() const
{
    return this->_year;
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
