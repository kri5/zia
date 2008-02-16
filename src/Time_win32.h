#ifndef TIME_WIN32_H__
#define TIME_WIN32_H__

#include <atltime.h>

#include "ITime.h"

class Time : ITime
{
	public:
		struct	Error
		{
			enum Code
			{
				Unknown,
				CantConvert
			};
			static const char*	Msg[];
		};
		/// Will create a time object for the current date (GetSystemTime)
		Time();
		Time(const FILETIME&);
		Time(const SYSTEMTIME&);
		virtual ~Time();

		time_t				getTimestamp() const;
		const char*			getStr() const;
		bool				operator<(const ITime&) const;
		bool				operator>(const ITime&) const;
		bool				operator<=(const ITime&) const;
		bool				operator>=(const ITime&) const;

	private:
		CTime*				_time;
};

#endif //TIME_WIN32_H__
