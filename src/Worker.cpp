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
        if (this->_pool->empty())
        {
            //std::cout << "Worker : nothing left to do." << std::endl;
            this->_pool->addSleepingThread(this);
            this->checkSleep(true);
        }
        else
        {
            Task* t = this->_pool->popTask();
            //std::cout << "Found task" << std::endl;
            //std::cout << t << std::endl;
            t->execute();
            //std::cout << "ok" << std::endl;
        }
    }
}

