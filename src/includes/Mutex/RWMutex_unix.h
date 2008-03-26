#ifndef RWMUTEX_UNIX_H
#define RWMUTEX_UNIX_H

#include "zia.h"
#include <pthread.h>
#include "Mutex/IRWMutex.h"

class RWMutex : public IRWMutex
{
public:
        RWMutex();
        ~RWMutex();
        void                rdlock();
        void                wrlock();
        void                unlock();

private:
        pthread_rwlock_t    rwmutex;

};

#endif // RWMUTEX_UNIX_H
