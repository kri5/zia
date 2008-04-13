#include "Workflow/Pool.h"
#include "Logger.hpp"
#include "MutexLock.hpp"
#include "Workflow/Worker.h"
#include "Time/Time.h"

Pool::Pool(unsigned int nbThreads, unsigned int nbTasks) : _nbThreads(nbThreads), _nbTasks(nbTasks)
{
    _mutex = new Mutex();
    //_keepAliveMutex = new Mutex();
}

Pool::~Pool()
{
    delete this->_mutex;
    //delete this->_keepAliveMutex;
}

void        Pool::init()
{
    MutexLock   get_lock(this->_mutex);
    for (unsigned int i = 0; i < this->_nbThreads; ++i)
    {
        Worker* w = Worker::create(this, i);
        //Automatically adding thread to sleeping pool.
        this->_workingThreads.push_back(w);
    }
    for (unsigned int i = 0; i < this->_nbTasks; ++i)
    {
        this->_freeTasks.push(new Task(this));
    }
    this->_manager = Pool::Manager::create(this);
}

bool        Pool::addTask(ClientSocket* clt, const std::vector<Vhost*>* vhosts)
{
    MutexLock   get_lock(this->_mutex);
    if (this->_freeTasks.size() > 0)
    {
        Task*   t = this->_freeTasks.front();
        this->_freeTasks.pop();
        t->init(clt, vhosts);
        this->_tasks.push(t);
        return true;
    }
    std::cout << "Warning : dropping task !!!" << std::endl;
    return false;
}

void    Pool::addSleepingThread(Worker* thread)
{
    MutexLock   get_lock(this->_mutex);
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
    MutexLock   get_lock(this->_mutex);
    if (this->_tasks.size() > 0)
    {
        Task* task = this->_tasks.front();
        this->_tasks.pop();
        //std::cout << "poping " << task->_taskId << std::endl;
        return task;
    }
    return NULL;
}

void        Pool::finishTask(Task* t)
{
    MutexLock   get_lock(this->_mutex);

    //std::cout << "adding task " << t->_taskId << " to free task" << std::endl;
    this->_freeTasks.push(t);
}

Worker*    Pool::popFreeThread()
{
    MutexLock   get_lock(this->_mutex);
    if (this->_threads.size() > 0)
    {
        Worker* thread = this->_threads.front();
        this->_threads.pop();
        this->_workingThreads.push_back(thread);
        return thread;
    }
    return NULL;
}

bool        Pool::createThread()
{
    //FIXME
	return true;
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

void    Pool::rescheduleTask(Task* t)
{
    //std::cout << "rescheduling task " << t->_taskId << std::endl;
    MutexLock   getLock(this->_mutex);
    this->_tasks.push(t);
}

void            Pool::addKeepAliveClient(ClientSocket* clt, int timeout, const std::vector<Vhost*>* vhosts)
{
    MutexLock   get_lock(this->_mutex);

    this->_keepAlive.push(KeepAliveClient(clt, timeout, vhosts));
    this->_keepAlive.back().timer = new Time();
}

void    Pool::flushKeepAlive(std::list<KeepAliveClient>& sockets)
{
    MutexLock   get_lock(this->_mutex);

    while (this->_keepAlive.size() > 0)
    {
        //std::cout << "transfering keep alive client from queue to list" << std::endl;
        sockets.push_back(this->_keepAlive.front());
        this->_keepAlive.pop();
    }
}

