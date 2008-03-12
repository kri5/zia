#include "Buffer.h"
#include "Worker.h"

#include "Logger.hpp"
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
void            Worker::code()
{
    Task*       t;

    while (this->_running)
    {
        t = this->_pool->popTask();
        if (t != NULL)
        {
            Logger::getInstance() << Logger::Info << "Starting task" << Logger::Flush;
            t->execute();
            delete t;
            Logger::getInstance() << Logger::Info << "Task finished. remaining " << this->_pool->getTaskNbr() << Logger::Flush;
        }
        else
        {
            this->_pool->addSleepingThread(this);
            Logger::getInstance() << Logger::Info << "No more task to pop. thread is now asleep" << Logger::Flush;
            this->checkSleep(true);
        }
    }
    std::cout << "Thread is dying" << std::endl;
    exit(1);
}

