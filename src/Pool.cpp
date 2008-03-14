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
    MutexLock   get_lock(this->_mutex);
    for (unsigned int i = 0; i < this->_nbThreads; ++i)
    {
        Worker* w = Worker::create(this);
        //Automatically adding thread to sleeping pool.
        this->_workingThreads.push_back(w);
    }
    this->_manager = Pool::Manager::create(this);
}

bool        Pool::addTask(Task* task)
{
    //std::cout << "before addtask lock" << std::endl;
    MutexLock   get_lock(*this->_mutex);
    //std::cout << "after addtask lock" << std::endl;
    //FIXME : adjust this limit and set it in the conf file.
    if (this->_tasks.size() < 150)
    {
        this->_tasks.push(task);
        //if (this->_manager->sleeping())
        //{
        //    this->_manager->awake();
        //}
        //else
        //    std::cout << "No need to awake manager" << std::endl;
        return true;
    }
    std::cout << "Warning : dropping task !!!" << std::endl;
    return false;
}

void    Pool::addSleepingThread(Worker* thread)
{
    //std::cout << "before addSleepingThread lock" << std::endl;
    MutexLock   get_lock(*this->_mutex);
    //std::cout << "after addSleepingThread lock" << std::endl;
    this->_threads.push(thread);
    //retirer le thread de la pile de thread actif
    std::list<Worker*>::iterator  it = this->_workingThreads.begin();
    std::list<Worker*>::iterator  ite = this->_workingThreads.end();
    while (it != ite)
    {
        if ((*it) == thread)
        {
            this->_workingThreads.erase(it);
            return ;
        }
        ++it;
    }
}

Task*   Pool::popTask()
{
    //std::cout << "Before popTask lock" << std::endl;
    MutexLock   get_lock(*this->_mutex);
    //std::cout << "after popTask lock" << std::endl;
    if (this->_tasks.size() > 0)
    {
        Task* task = this->_tasks.front();
        this->_tasks.pop();
        return task;
    }
    return NULL;
}

Worker*    Pool::popFreeThread()
{
    //std::cout << "before popFreeThreadlock" << std::endl;
    MutexLock   get_lock(*this->_mutex);
    //std::cout << "after popFreeThreadlock" << std::endl;
    if (this->_threads.size() > 0)
    {
        Worker* thread = this->_threads.front();
        this->_threads.pop();
        this->_workingThreads.push_back(thread);
        return thread;
    }
    return NULL;
}

void        Pool::__createThread()
{
    Worker* w = Worker::create(this);
    this->_workingThreads.push_back(w);
}

bool        Pool::createThread()
{
    //FIXME
}

void        Pool::relaunchThread(Worker* w)
{
    w->stop();
}

void        Pool::killThread()
{
    //FIXME
}

unsigned int    Pool::getNbThreads() const
{
    //std::cout << "before getNbThreads" << std::endl;
    MutexLock   get_lock(*this->_mutex);
    //std::cout << "after getNbThreads" << std::endl;
    return this->_nbThreads;
}

unsigned int    Pool::getFreeThreadsNbr() const
{
    //std::cout << "before getFreeThreadsNbr" << std::endl;
    MutexLock   get_lock(*this->_mutex);
    //std::cout << "after getFreeThreadsNbr" << std::endl;
    return this->_threads.size();
}

unsigned int    Pool::getTaskNbr() const
{
    //std::cout << "before getTaskNbr" << std::endl;
    MutexLock   get_lock(*this->_mutex);
    //std::cout << "after getTaskNbr" << std::endl;
    return this->_tasks.size();
}

bool            Pool::empty() const
{
    //std::cout << "before empty" << std::endl;
    MutexLock   get_lock(*this->_mutex);
    //std::cout << "after empty" << std::endl;
    return this->_tasks.empty();
}

