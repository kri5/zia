#include "Time_win32.h"
#include "ZException.hpp"

#include "MemoryManager.hpp"

Time::Time() : _time(NULL)
{
	init();
}

Time::Time(const FILETIME& right)
{
	_time = new CTime(right);
}

Time::Time(const SYSTEMTIME& right)
{
	_time = new CTime(right);
}

Time::~Time()
{
	if (this->_time)
		delete this->_time;
}

void		Time::init()
{
	if (_time)
		delete _time;
	_time = new CTime(time(NULL));
}

time_t		Time::getTimestamp() const
{
	return this->_time->GetTime();
}

const char*		Time::getStr() const
{
	//Saturday, February 16, 2008 11:57:25 AM
	return this->_time->Format("%A, %B %d, %Y %I:%M:%S %p\n");
}

bool			Time::operator <(const ITime& right) const
{
	return *this < right;
}

bool			Time::operator >(const ITime& right) const
{
	return *this > right;
}

bool			Time::operator <=(const ITime& right) const
{
	return *this <= right;
}

bool			Time::operator >=(const ITime& right) const
{
	return *this >= right;
}

bool			Time::elapsed(time_t seconds) const
{
	return ((time(NULL) - this->getTimestamp()) >= seconds);
}