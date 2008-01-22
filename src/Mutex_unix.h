#ifndef MUTEX_UNIX_H
#define MUTEX_UNIX_H

#include <pthread.h>
#include "zia.h"
#include "IMutex.h"

class Mutex : public IMutex
{
public:
	Mutex();
	~Mutex();
	void			lock();
	bool			trylock();
	void			unlock();
protected:
private:
	pthread_mutex_t		mutex;
};

#endif // MUTEX_UNIX_H

