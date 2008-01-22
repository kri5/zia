#include "Mutex_win32.h"
#include <iostream>

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
		; // Put a line in the logs.
	CloseHandle(ghMutex);
}

void		Mutex::lock()
{
	if (WaitForSingleObject(ghMutex, INFINITE) == WAIT_OBJECT_0)
	{
			std::cout << "Locked! (real)" << std::endl;
			lockCount += 1;
	}
}

bool		Mutex::trylock()
{
	if (WaitForSingleObject(ghMutex, 0) == WAIT_OBJECT_0)
	{
		std::cout << "Locked! (real)" << std::endl;
		lockCount += 1;
		return true;
	}
	return false;
}

void		Mutex::unlock()
{
	if (lockCount > 0)
		lockCount -= 1;
	std::cout << "Unlocked!" << std::endl;
	ReleaseMutex(ghMutex);
}