#include "zia.h"

#include "Logger.hpp"
#include <iostream>
#include <fstream>

// You don't wan't to include memorymanager here. Since the Logger is deleted after the MemoryManager (MManager uses the logger),
// if you do so, there always be 2 memory leaks in the logger.

void  Logger::setLogLevel(Logger::LEVEL level)
{
	this->_level = level;
}

void  Logger::setOutputFile(const char* file)
{
    if (this->_file != NULL)
    {
        MutexLock(this->_logMutex);
        if (this->_file != NULL)
        {
            this->_file->close();
            delete this->_file;
            this->_file = NULL;
        }
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
    MutexLock(this->_logMutex);

	this->_stream << Zia::Newline;
	this->log(this->_nextDebugLevel, _stream.str());
	this->_file->flush();
	this->_stream.str("");
	this->_stream.clear();

	this->_stdout = this->_defaultStdOut;
}

Logger&		Logger::operator<< (Logger::UTIL val)
{
	if (val == Flush)
		this->flush();
	else if (val == NoStdOut)
		this->setStdOut(false);
	else if (val == PrintStdOut)
		this->setStdOut(true);
	return *this;
}

Logger::Logger() : _file(NULL), _nextDebugLevel(Unset)
{
    _logMutex = new Mutex();

#ifdef NDEBUG
	setLogLevel(Logger::Warning);
	_defaultStdOut = false;
#else
	setLogLevel(Logger::All);
	_defaultStdOut = true;
#endif
	setOutputFile("test.log");
	_stdout = _defaultStdOut;
}

Logger::~Logger()
{
	if (this->_stream.str() != "")
		this->flush();
	if (this->_file != NULL)
		delete this->_file;
	delete _logMutex;
}

