#include "zia.h"


#include "Logger.hpp"
#include <iostream>
#include <fstream>

void  Logger::setLogLevel(Logger::LEVEL level)
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

void  Logger::log(Logger::LEVEL level, std::string msg)
{
  if (level < this->_level)
  {
    if (this->_stdout == true)
      std::cout << msg;
    if (this->_file != NULL)
      *(this->_file) << msg;
  }
}

void		Logger::setNextDebugLevel(Logger::LEVEL logLvl)
{
	this->_nextDebugLevel = logLvl;
}

Logger&		Logger::operator<< (Logger::LEVEL lvl)
{
	this->setNextDebugLevel(lvl);
	return *this;
}

Logger&		Logger::operator<< (Logger::UTIL val)
{
	if (val == FLUSH)
	{
		this->_stream << Zia::Newline;
		this->log(this->_nextDebugLevel, _stream.str());
		this->_stream.str("");
		this->_stream.clear();
	}
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
