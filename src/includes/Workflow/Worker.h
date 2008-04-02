#ifndef __WORKER_H__
#define __WORKER_H__

#include <fstream>
#include <exception>

#include "Threads/IThread.h"
#include "Workflow/Pool.h"
#include "Workflow/Task.h"
#include "Time/ITime.h"

class Pool;

class Worker : public IThread
{
    public:
        static Worker*      create(Pool*, unsigned int);
        virtual ~Worker(); 

    private:
        Worker(Pool* pool, unsigned int);
        void                code();

        Pool*               _pool;
        int                 _timeoutDelay;
        unsigned int        _id;
};

#endif // WORKER_H
