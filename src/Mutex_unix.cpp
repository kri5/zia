#include "Mutex_unix.h"

Mutex::Mutex()
{
	lockCount = 0;
	pthread_mutex_init(&mutex, NULL);
}

Mutex::~Mutex()
{
	if (lockCount > 0)
		; // Put a line in the logs.
	pthread_mutex_destroy(&mutex);
}

void		Mutex::lock()
{
	if (pthread_mutex_lock(&mutex) == 0)
		lockCount += 1;
}

bool		Mutex::trylock()
{
	if (pthread_mutex_trylock(&mutex) == 0)
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
	pthread_mutex_unlock(&mutex);
}

