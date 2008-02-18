#include "Time_unix.h"

Time::Time()
{
	_time = time(NULL);
}

Time::Time(time_t time) : _time(time)
{
}

Time::~Time()
{
}

time_t	Time::getTimestamp() const
{
	return this->_time;
}

const char*	Time::getStr() const
{
	return ctime(&this->_time);
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


