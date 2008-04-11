#ifndef __MUTEXLOCK_H__
#define __MUTEXLOCK_H__

#include "Mutex/IMutex.h"
#include <iostream>

class   MutexLock
{
    public:
        MutexLock(IMutex* mutex) : _mutex(mutex)
        {
            //std::cout << "Locked" << std::endl;
            mutex->lock();
        }
        MutexLock(IMutex& mutex) : _mutex(&mutex)
        {
            //std::cout << "Locked" << std::endl;
            mutex.lock();
        }
        ~MutexLock()
        {
            this->_mutex->unlock();
            //std::cout << "UnLocked" << std::endl;
        }
    private:
        IMutex*     _mutex;
};

#endif  /*!__MUTEXLOCK_H__*/
