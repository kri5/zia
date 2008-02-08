#include "ZException.h"
#include "Logger.hpp"
#include <iostream>
#include <sstream>

ZException::ZException(int line = 42, 
        const char* file = "no file",
        const char* msg = "no message")
{
    _line = line;
    _file = file;
    _msg = msg;

    Logger::getInstance() << Logger::Error
        << "Exception thrown from file ("
        << _file << ") at line " << _line
        << " with message : " << _msg
        << Logger::Flush;
}

ZException::~ZException() throw()
{

}

const char*   ZException::what() const throw()
{
    std::ostringstream  er;

    er << "Exception thrown from file (" << _file << ")"
        << " at line " << _line 
        << " with message : " << _msg << std::endl;

    return er.str().c_str();
}

std::string     ZException::getFile()
{
    return _file;
}

std::string     ZException::getMessage()
{
    return _msg;
}

int             ZException::getLine()
{
    return _line;
}
