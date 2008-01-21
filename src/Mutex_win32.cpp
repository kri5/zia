#include "Mutex_win32.h"

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
	if (lockCount == 0)
		if (WaitForSingleObject(ghMutex, INFINITE) != WAIT_FAILED)
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
	if (lockCount == 1)
		ReleaseMutex(ghMutex);
	if (lockCount > 0)
		lockCount -= 1;
}