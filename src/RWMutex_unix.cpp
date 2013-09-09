#include <errno.h>
#include <cstring>
#include <iostream>

#include "Mutex/RWMutex.h"
#include "Logger.hpp"

#include "MemoryManager.hpp"

RWMutex::RWMutex()
{
    pthread_rwlock_init(&rwmutex, NULL);
}

RWMutex::~RWMutex()
{
    pthread_rwlock_destroy(&rwmutex);
}

void            RWMutex::rdLock()
{
    if (pthread_rwlock_rdlock(&rwmutex) != 0)
    {
       Logger::getInstance() << Logger::Error << strerror(errno) << Logger::Flush; 
    }
}

void            RWMutex::wrLock()
{
    if (pthread_rwlock_wrlock(&rwmutex) != 0)
    {
        Logger::getInstance() << Logger::Error << strerror(errno) << Logger::Flush;
    }
}

void            RWMutex::rdUnlock()
{
    if (pthread_rwlock_unlock(&rwmutex) != 0)
    {
        Logger::getInstance() << Logger::Error << strerror(errno) << Logger::Flush;
    }
}

void			RWMutex::wrUnlock()
{
	rdUnlock();
}
