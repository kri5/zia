#include "Logger.hpp"
#include <iostream>
#include <fstream>

void  Logger::setLogLevel(Logger::LOGLEVEL level)
{
  this->_level = level;
}

void  Logger::setOutputFile(const char* file)
{
  if (this->_file != NULL)
  {
    this->_file->close();
    delete this->_file;
    this->_file = NULL;
  }
  this->_file = new std::ofstream(file, std::ios_base::out | std::ios_base::trunc);
}

void  Logger::setStdOut(bool value)
{
  this->_stdout = value;
}

void  Logger::log(Logger::LOGLEVEL level, std::string msg)
{
  if (level < this->_level)
  {
    if (this->_stdout == true)
      std::cout << msg << std::endl;
    if (this->_file != NULL)
      *(this->_file) << msg << std::endl;
  }
}

void		Logger::setNextDebugLevel(Logger::LOGLEVEL logLvl)
{
	this->_nextDebugLevel = logLvl;
}

template <typename T>
Logger&		Logger::operator <<(const T& toLog)
{
	if (this->_nextDebugLevel == UNSET)
	{
		this->log(WARN, "Warning: You haven't set a error level for Logger::log\n Setting default value to warning");
		this->_nextDebugLevel = WARN;
	}
	std::ostringstream stream;
	stream << toLog;
	this->log(this->_nextDebugLevel, stream.str());
	return *this;
}

Logger::Logger() : _file(NULL)
{
  this->setLogLevel(Logger::ALL);
  this->setOutputFile("test.log");
  this->_stdout = true;
  this->_nextDebugLevel = UNSET;
}

Logger::~Logger()
{
  delete this->_file;
}
