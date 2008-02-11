#include "Zerror.h"
#include "Logger.hpp"

#include <iostream>
void    ZNewHandler()
{
    Logger::getInstance() << Logger::Error << "Cannot allocate enough memory!" << Logger::Flush;
    throw std::bad_alloc();
    return;
}
