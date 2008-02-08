#ifndef LOGGER_HPP__
#define LOGGER_HPP__

#include "zia.h"
#include <fstream>
#include <sstream>
#include <string>
#include <ostream>
#include <iostream>

#ifdef WIN32
 #include "Mutex_win32.h"
#else
 #include "Mutex_unix.h"
#endif
#include "Singleton.hpp"

/// Provides Logging capabilities. Only usable with operator <<
class Logger : public Singleton<Logger>
{
	//Attributes
	public:
		/// Set of values used for setting current message loglevel. If two ore more are passed, the last is ised.
		enum	LEVEL
		{
			/// Default value.
			UNSET = -1,
			NONE,
			INFO,
			WARN,
			ERRORLVL,
			DEBUGMODE, // Modified because of system's #define DEBUG
			ALL
		};
		enum	UTIL
		{
			/// Passing this value to Logger will add a carriage return and then flush the output.
			FLUSH
		};
	private:
		Logger::LEVEL	    _level;
		std::ofstream*	    _file;
		bool			    _stdout;
		LEVEL			    _nextDebugLevel;
		std::ostringstream 	_stream;
        IMutex*             _logMutex;

		Logger();
		void 				log(LEVEL, std::string);
		void				setNextDebugLevel(LEVEL);
		void				flush();
		virtual ~Logger();

		// Methods
	public:
		/// Will set the server logging level. (Every levels upper than the one set will be silently ignored.
		void  setLogLevel(Logger::LEVEL);
		/// Set the log file
		void  setOutputFile(const char*);
		/// Set whether	or not Logger should write infos to stdout
		void  setStdOut(bool);
		/// Template operator overloading to easyly log messages and values.
		/// T must have his << overloaded for std::ostream
		/// Provided specialization are : LEVEL and UTIL
		template <typename T>
			Logger&	operator<<(T toLog)
			{
				if (this->_nextDebugLevel == UNSET)
				{
					this->log(WARN, "Warning: You haven't set a error level for Logger::log\n Setting default value to warning\n");
					this->_nextDebugLevel = ALL;
				}
				_stream << toLog;
				return *this;
			}
		Logger&	operator<<(UTIL);
		Logger&	operator<<(LEVEL);
		friend class Singleton<Logger>;
};

#endif  /* !__LOGGER_HPP__ */

