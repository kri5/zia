#include "Time/Time_unix.h"

Time::Time()
{
    init();
}

Time::Time(time_t time) : _time(time)
{
}

Time::~Time()
{
}

void    Time::init()
{
    _time = time(NULL);
}

time_t	Time::getTimestamp() const
{
	return this->_time;
}

std::string		Time::getStr() const
{
	return std::string(ctime(&this->_time));
}

bool    Time::elapsed(time_t seconds) const
{
    return ((time(NULL) - _time) >= seconds);
}

bool	Time::operator<(const ITime& right) const
{
	return (this->_time < right.getTimestamp());
}

bool	Time::operator>(const ITime& right) const
{
	return (this->_time > right.getTimestamp());
}

bool	Time::operator<=(const ITime& right) const
{
	return (this->_time <= right.getTimestamp());
}

bool	Time::operator>=(const ITime& right) const
{
	return (this->_time >= right.getTimestamp());
}


