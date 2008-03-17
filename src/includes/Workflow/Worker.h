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
        static Worker*      create(Pool*);
        virtual ~Worker(); 

    private:
        Worker(Pool* pool);
        void                code();

        Pool*               _pool;
        int                 _timeoutDelay;
};

#endif // WORKER_H
