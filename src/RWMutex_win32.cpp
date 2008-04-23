#include "Mutex/RWMutex.h"
#include "Logger.hpp"

#include "MemoryManager.hpp"

RWMutex::RWMutex()
{
    InitializeSRWLock(SRWLock);
}

RWMutex::~RWMutex()
{
}

void            RWMutex::rdLock()
{
	AcquireSRWLockShared(SRWLock);
}

void            RWMutex::wrLock()
{
	AcquireSRWLockExclusive(SRWLock);
}

void            RWMutex::rdUnlock()
{
	ReleaseSRWLockShared(SRWLock);
}

void			RWMutex::wrUnlock()
{
	ReleaseSRWLockExclusive(SRWLock);
}
