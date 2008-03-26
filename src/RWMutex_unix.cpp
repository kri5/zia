#include <errno.h>
#include <iostream>

#include "Mutex/RWMutex_unix.h"
#include "MemoryManager.hpp"
#include "Logger.hpp"

RWMutex::RWMutex()
{
    pthread_rwlock_init(&rwmutex, NULL);
}

RWMutex::~RWMutex()
{
    pthread_rwlock_destroy(&rwmutex);
}

void            RWMutex::rdlock()
{
    if (pthread_rwlock_rdlock(&rwmutex) != 0)
    {
       Logger::getInstance() << Logger::Error << strerror(errno) << Logger::Flush; 
    }
}

void            RWMutex::wrlock()
{
    if (pthread_rwlock_wrlock(&rwmutex) != 0)
    {
        Logger::getInstance() << Logger::Error << strerror(errno) << Logger::Flush;
    }
}

void            RWMutex::unlock()
{
    if (pthread_rwlock_unlock(&rwmutex) != 0)
    {
        Logger::getInstance() << Logger::Error << strerror(errno) << Logger::Flush;
    }
}
