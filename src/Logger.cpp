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

Logger::Logger() : _file(NULL)
{
  this->setLogLevel(Logger::ALL);
  this->setOutputFile("test.log");
  this->_stdout = true;
}

Logger::~Logger()
{
  ;
}
