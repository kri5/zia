#include "Utils/Buffer.h"
#include "Workflow/Worker.h"

#include "Logger.hpp"
#include "MemoryManager.hpp"
#include "Time/Time.h"

Worker::Worker(Pool* pool, unsigned int id) : _pool(pool), _id(id)
{
}

Worker::~Worker()
{
}

/// Launch a new thread that will handle the new client connection
Worker*          Worker::create(Pool* pool, unsigned int id)
{
    // Will construct a Worker (so a new Thread class) and call it method run, which will create the thread
    // then, this thread will call IThreadBase() which will call Worker::code, from this newlly created instance.
    Worker* w = new Worker(pool, id);
    w->run();
    return w;
}

/// Here we are in the first threaded method
void            Worker::code()
{
    while (this->_running)
    {
        Task*   t = this->_pool->popTask();
        if (t != NULL)
        {
            t->execute(this->_id);
            this->_pool->finishTask(t);
        }
        else
        {
            this->_pool->addSleepingThread(this);
            //Logger::getInstance() << Logger::Info << Logger::PrintStdOut << "Sending thread " << this->m_pid << " to bed" << Logger::Flush;
            this->checkSleep(true);
        }
    }
    //FIXME : can't let this on prod.
    std::cout << "Thread is dying" << std::endl;
    exit(1);
}

