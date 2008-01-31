#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include "Singleton.hpp"
#include <fstream>

class Logger  : public Singleton<Logger>
{
  //Attributes
  public:
    enum  LOGLEVEL
    {
      NONE,
      INFO,
      WARN,
      ERROR,
      DEBUG,
      ALL
    };
  private:
    Logger::LOGLEVEL  _level;
    std::ofstream*    _file;
    bool              _stdout;

  // Methods
  public:
    Logger();
    virtual ~Logger();
    void  setLogLevel(Logger::LOGLEVEL);
    void  setOutputFile(const char*);
    void  setStdOut(bool);
    void  log(Logger::LOGLEVEL, std::string);
};

#endif  /* !__LOGGER_HPP__ */
