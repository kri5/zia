#ifndef TIME_UNIX_H__
# define TIME_UNIX_H__

#include <time.h>

#include "ITime.h"

class Time : public ITime
{
	public:
		Time();
		Time(time_t);
		virtual ~Time();

		time_t		getTimestamp() const;
		const char*	getStr() const;
		bool		operator<(const ITime&) const;
		bool		operator>(const ITime&) const;
		bool		operator<=(const ITime&) const;
		bool		operator>=(const ITime&) const;
	private:
		time_t		_time;
};

#endif //TIME_UNIX_H__

