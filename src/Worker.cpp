#include "Buffer.h"
#include "Worker.h"

#include "Logger.hpp"
#include "MemoryManager.hpp"
#include "Time.h"
#include "RootConfig.hpp"

Worker::Worker(Pool* pool) : _pool(pool), _task(NULL)
{
    _time = new Time();
    _timeoutDelay = atoi(RootConfig::getParam("Timeout").c_str());
    std::cout << "delay for timeout == " << _timeoutDelay << std::endl;
}

Worker::~Worker()
{
    if (this->_task)
        delete this->_task;
    delete this->_time;
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
    while (this->_running)
    {
        this->_task = this->_pool->popTask();
        if (this->_task != NULL)
        {
            this->_task->execute(this->_time);
            delete this->_task;
            this->_task = NULL;
        }
        else
        {
            this->_pool->addSleepingThread(this);
            this->checkSleep(true);
        }
    }
    //FIXME : can't let this on prod.
    std::cout << "Thread is dying" << std::endl;
    exit(1);
}

bool    Worker::checkTimeout()
{
    if (this->_time->elapsed(this->_timeoutDelay))
    {
        std::cout << "Client has timeout" << std::endl;
        return true;
    }
    return false;
}


