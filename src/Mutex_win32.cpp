#include "Mutex/Mutex_win32.h"
#include <iostream>

#include "Logger.hpp"

Mutex::Mutex()
{
	lockCount = 0;
	ghMutex = CreateMutex(NULL, FALSE, NULL);
	if (!ghMutex)
		throw "Mutex error: " + GetLastError();
}

Mutex::~Mutex()
{
	if (lockCount > 0)
		Logger::getInstance() << Logger::Error << "lockCount is more than 0." << Logger::Flush;
	CloseHandle(ghMutex);
}

void		Mutex::lock()
{
	if (WaitForSingleObject(ghMutex, INFINITE) == WAIT_OBJECT_0)
		lockCount += 1;
}

bool		Mutex::trylock()
{
	if (WaitForSingleObject(ghMutex, 0) == WAIT_OBJECT_0)
	{
		lockCount += 1;
		return true;
	}
	return false;
}

void		Mutex::unlock()
{
	if (lockCount > 0)
		lockCount -= 1;
	if (ReleaseMutex(ghMutex) == 0)
		std::cout << "Mutex error: " << GetLastError() << " see http://msdn.microsoft.com/en-us/library/ms681382(VS.85).aspx" << std::endl;
}
