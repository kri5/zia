#include "Pool.h"
#include "Logger.hpp"
#include "MutexLock.hpp"
#include "Worker.h"

Pool::Pool(unsigned int nbThreads) : _nbThreads(nbThreads)
{
    _mutex = new Mutex();
}

void        Pool::init()
{
    for (unsigned int i = 0; i < this->_nbThreads; ++i)
    {
        Worker* w = Worker::create(this);
        this->_threads.push(w);
    }
    this->_manager = Pool::Manager::create(this);
}

bool        Pool::addTask(Task* task)
{
    MutexLock   get_lock(*this->_mutex);
    //FIXME : adjust this limit and set it in the conf file.
    if (this->_tasks.size() < 150)
    {
        //std::cout << "Pushing task" << std::endl;
        this->_tasks.push(task);
        if (this->_manager->sleeping())
        {
            //std::cout << "awaking manager" << std::endl;
            this->_manager->awake();
        }
        else
            std::cout << "No need to awake manager" << std::endl;
        std::cout << "tasks size == " << this->_tasks.size() << std::endl;
        return true;
    }
    else
    {
        std::cout << "Warning : dropping task !!!" << std::endl;
    }
    return false;
}

void    Pool::addSleepingThread(IThread* thread)
{
    MutexLock   get_lock(*this->_mutex);
    this->_threads.push(thread);
}

Task*   Pool::popTask()
{
    MutexLock   get_lock(*this->_mutex);
    if (this->_tasks.size() > 0)
    {
        Task* task = this->_tasks.front();
        this->_tasks.pop();
        return task;
    }
    return NULL;
}

IThread*    Pool::popFreeThread()
{
    MutexLock   get_lock(*this->_mutex);
    if (this->_threads.size() > 0)
    {
        IThread* thread = this->_threads.front();
        this->_threads.pop();
        return thread;
    }
    return NULL;
}

bool        Pool::createThread()
{
    //FIXME
}

void        Pool::killThread()
{
    //FIXME
}

unsigned int    Pool::getNbThreads() const
{
    MutexLock   get_lock(*this->_mutex);
    return this->_nbThreads;
}

unsigned int    Pool::getFreeThreadsNbr() const
{
    MutexLock   get_lock(*this->_mutex);
    return this->_threads.size();
}

unsigned int    Pool::getTaskNbr() const
{
    MutexLock   get_lock(*this->_mutex);
    return this->_tasks.size();
}

bool            Pool::empty() const
{
    MutexLock   get_lock(*this->_mutex);
    return this->_tasks.empty();
}

