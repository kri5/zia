#ifndef MUTEX_WIN32_H
#define MUTEX_WIN32_H

#include "zia.h"
#include "IMutex.h"

class Mutex : public IMutex
{
public:
	Mutex();
	~Mutex();
	void		lock();
	bool		trylock();
	void		unlock();
protected:
private:
	HANDLE		ghMutex;
};

#endif // MUTEX_WIN32_H