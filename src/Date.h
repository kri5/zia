#ifndef __DATE_H__
#define __DATE_H__

#include <iostream>

class   Date
{
    public:
        Date(std::string&);
        ~Date();
        static  bool    isDayOfTheWeek(const std::string&);
        static  bool    isMonthOfTheYear(const std::string&);
        static  bool    isDayOfTheMonth(int, int);
        static  bool    isHour(int);
        static  bool    isMinute(int);
        static  bool    isSecond(int);
        static  bool    isBisextile(int);

        static  const char*     dayName[7];
        static  const char*     monthName[12];
        static  const int       monthLength[12];
    private:
        int         _seconds;
        int         _minutes;
        int         _hours;
        int         _day;
        int         _month;
        int         _year;
        std::string _date;
        std::string _dayOfTheWeek;
        std::string _monthOfTheYear;
};

#endif  /* !__DATE_H__ */
