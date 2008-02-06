#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include "Singleton.hpp"
#include <fstream>
#include <string>

class Logger : public Singleton<Logger>
{
  //Attributes
  public:
    enum  LOGLEVEL
    {
	  UNSET = -1,
      NONE,
      INFO,
      WARN,
      ERRORLVL,
      DEBUG,
      ALL
    };
  private:
    Logger::LOGLEVEL	_level;
    std::ofstream*		_file;
    bool				_stdout;
	LOGLEVEL			_nextDebugLevel;

  // Methods
  public:
    Logger();
    virtual ~Logger();
    void  setLogLevel(Logger::LOGLEVEL);
    void  setOutputFile(const char*);
    void  setStdOut(bool);
    void  log(Logger::LOGLEVEL, std::string);
	template <typename T>
	Logger&	operator<<(const T&);
	void	setNextDebugLevel(LOGLEVEL);
};

#endif  /* !__LOGGER_HPP__ */
