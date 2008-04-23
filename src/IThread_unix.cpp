#include <errno.h>

#include "Threads/IThread_unix.h"
#include "MutexLock.hpp"
#include "Logger.hpp"

#include "MemoryManager.hpp"

IThread::IThread()
{
    _mutex = new Mutex();
	_asleep = false;
    pthread_cond_init(&_cond, NULL);
}

IThread::~IThread()
{
    delete this->_mutex;
}

void			IThread::run()
{
	// Don't start two threads into the same object
	if (_running) return;

	// Creating an OS specific thread, using the static callback.
    int ret;
	_running = true;
    if ((ret = pthread_create(&m_pid, NULL, IThread::dispatch, this)) != 0)
    {
        Logger::getInstance() << Logger::Error << "Can't create new thread" << Logger::Flush;
        _running = false;
    }
}

void      IThread::stop()
{
	// Don't stop a thread already stopped.
	if (!_running) return;

	// Stopping the thread.
    pthread_cancel(m_pid);
	_running = false;
}

void    IThread::sleep()
{
    MutexLock   get_lock(this->_mutex);
    this->_sleepScheduled = true;
}

void    IThread::awake()
{
    MutexLock   get_lock(this->_mutex);
    pthread_cond_signal(&(this->_cond));
    //Logger::getInstance() << Logger::Info << Logger::PrintStdOut << "Removing thread " << this->m_pid << " from bed" << Logger::Flush;
    this->_asleep = false;
}

void    IThread::checkSleep(bool forceSleep)
{
    this->_mutex->lock();
    if (!this->_asleep && (forceSleep || this->_sleepScheduled))
    {
        //Logger::getInstance() << Logger::Info << "putting thread #" << this->m_pid << " to bed" << Logger::Flush;
        this->_asleep = true;
        this->_sleepScheduled = false;
        pthread_cond_wait(&(this->_cond), &(this->_mutex->getMutex()));
        //Logger::getInstance() << Logger::Info << Logger::PrintStdOut << "Thread " << this->m_pid << " is awake" << Logger::Flush;
    }
    this->_mutex->unlock();
}

