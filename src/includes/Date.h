#ifndef __DATE_H__
#define __DATE_H__

#include <iostream>
#include <sstream>
#include <string>
#include <time.h>

#include "Workflow/Parser.h"

class   Date : public Parser
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
        int             getSeconds()    const;
        int             getMinutes()    const;
        int             getHours()      const;
        int             getDay()        const;
        int             getMonth()      const;
        int             getYear()       const;
        void            readTime();
        int             getMonthFromString(std::string month);
        std::string     toString();
        time_t          toTimestamp();

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
