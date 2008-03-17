#ifndef __MUTEXLOCK_H__
#define __MUTEXLOCK_H__

#include "Mutex/IMutex.h"
#include <iostream>

class   MutexLock
{
    public:
        MutexLock(IMutex* mutex) : _mutex(mutex)
        {
            mutex->lock();
        }
        MutexLock(IMutex& mutex) : _mutex(&mutex)
        {
            mutex.lock();
        }
        ~MutexLock()
        {
            this->_mutex->unlock();
        }
    private:
        IMutex*     _mutex;
};

#endif  /*!__MUTEXLOCK_H__*/
