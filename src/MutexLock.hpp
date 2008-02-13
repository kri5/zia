#ifndef __MUTEXLOCK_H__
#define __MUTEXLOCK_H__

#ifdef WIN32
 #include "Mutex_win32.h"
#else
 #include "Mutex_unix.h"
#endif

class   MutexLock
{
    public:
        MutexLock(Mutex* mutex) : _mutex(mutex)
        {
            this->_mutex->lock();
        }
        ~MutexLock()
        {
            this->_mutex->unlock();
        }
    private:
        Mutex*  _mutex;
};

#endif  /*!__MUTEXLOCK_H__*/
