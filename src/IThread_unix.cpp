#include <errno.h>

#include "IThread_unix.h"
#include "MutexLock.hpp"
#include "Logger.hpp"

#include "MemoryManager.hpp"

IThread::IThread()
{
    _mutex = new Mutex();
    pthread_cond_init(&_cond, NULL);
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
    //pthread_exit(NULL);
    //pthread_stop()
	_running = false;
    //--nb;
}

void    IThread::sleep()
{
    this->_sleepScheduled = true;
}

void    IThread::awake()
{
    pthread_cond_signal(&(this->_cond));
    this->_asleep = false;
}

void    IThread::checkSleep(bool forceSleep)
{
    if (forceSleep || this->_sleepScheduled)
    {
        MutexLock   lock(this->_mutex);
        this->_asleep = true;
        this->_sleepScheduled = false;
        pthread_cond_wait(&(this->_cond), &(this->_mutex->getMutex()));
    }
}
