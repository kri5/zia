#include "zia.h"

#include "Logger.hpp"
#include <iostream>
#include <fstream>

#include "MemoryManager.hpp"

void  Logger::setLogLevel(Logger::LEVEL level)
{
    this->_level = level;
}

void  Logger::setOutputFile(const char* file)
{
    if (this->_file != NULL)
    {
        this->_logMutex->lock();
        if (this->_file != NULL)
        {
            this->_file->close();
            delete this->_file;
            this->_file = NULL;
        }
        this->_logMutex->unlock();
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

void		Logger::flush()
{
    this->_logMutex->lock();
    this->_stream << Zia::Newline;
    this->log(this->_nextDebugLevel, _stream.str());
    this->_stream.str("");
    this->_stream.clear();
    this->_logMutex->unlock();
}

Logger&		Logger::operator<< (Logger::UTIL val)
{
    if (val == Flush)
    {
        this->flush();
    }
	return *this;
}

Logger::Logger() : _file(NULL), _stdout(true), _nextDebugLevel(Unset)
{
    this->_logMutex = static_cast<IMutex*>(new Mutex());
    this->setLogLevel(Logger::All);
    this->setOutputFile("test.log");
}

Logger::~Logger()
{
    if (this->_stream.str() != "")
        this->flush();
    if (this->_file != NULL)
        delete this->_file;
}
