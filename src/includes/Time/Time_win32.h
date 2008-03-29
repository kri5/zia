#ifndef TIME_WIN32_H__
#define TIME_WIN32_H__

#include <atltime.h>

#include "ITime.h"

class Time : public ITime
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

		void				init();
		time_t				getTimestamp() const;
		std::string			getStr() const;
		bool				operator<(const ITime&) const;
		bool				operator>(const ITime&) const;
		bool				operator<=(const ITime&) const;
		bool				operator>=(const ITime&) const;
		bool				elapsed(time_t seconds) const;

	private:
		CTime*				_time;
};

#endif //TIME_WIN32_H__
