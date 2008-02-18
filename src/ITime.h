#ifndef ITIME_H__
# define ITIME_H__

// this include isn't required on win32.
#ifndef WIN32
#include <time.h>
#endif

class	ITime
{
	public:
		virtual ~ITime(){}
		virtual time_t				getTimestamp() const = 0;
		virtual const char*			getStr() const = 0;
		virtual bool				operator<(const ITime&) const = 0;
		virtual bool				operator>(const ITime&) const = 0;
		virtual bool				operator<=(const ITime&) const = 0;
		virtual bool				operator>=(const ITime&) const = 0;
};

#endif //ITIME_H__
