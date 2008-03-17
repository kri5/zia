#ifndef __WORKER_H__
#define __WORKER_H__

#include <fstream>
#include <exception>

#include "IThread.h"
#include "Pool.h"
#include "ITime.h"
#include "Task.h"

class Pool;

class Worker : public IThread
{
    public:
        static Worker*      create(Pool*);
        virtual ~Worker(); 

    private:
        Worker(Pool* pool);
        void                code();

        Pool*               _pool;
        int                 _timeoutDelay;
};

#endif // WORKER_H
