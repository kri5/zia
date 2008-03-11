#include "Buffer.h"
#include "Worker.h"

#include "MemoryManager.hpp"

Worker::~Worker()
{
}

/// Launch a new thread that will handle the new client connection
Worker*          Worker::create(Pool* pool)
{
    // Will construct a Worker (so a new Thread class) and call it method run, which will create the thread
    // then, this thread will call IThreadBase() which will call Worker::code, from this newlly created instance.
    Worker* w = new Worker(pool);
    w->run();
    return w;
}

/// Here we are in the first threaded method
void          Worker::code()
{
    while (this->_running)
    {
        Task* t = this->_pool->popTask();
        if (t != NULL)
        {
            t->execute();
            std::cout << "Task finished" << std::endl;
            delete t;
        }
        else
        {
            this->_pool->addSleepingThread(this);
            this->checkSleep(true);
        }
    }
}

