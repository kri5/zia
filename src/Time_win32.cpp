#include "Time_win32.h"
#include "ZException.hpp"

#include "MemoryManager.hpp"

Time::Time()
{
	_time = new CTime(time(NULL));
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
	delete this->_time;
}

time_t		Time::getTimestamp() const
{
	return this->_time->GetTime();
}

const char*		Time::getStr() const
{
	//Saturday, February 16, 2008 11:57:25 AM
	return this->_time->Format("%A, %B %d, %Y %I:%M:%S %p");
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
