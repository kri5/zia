#include "Threads/ZThread.h"
#include "Logger.hpp"

#include "MemoryManager.hpp"

ZThread::ZThread() : _asleep(false), _thread(nullptr)
{
	_asleep = false;
}

void	ZThread::run()
{
	// Don't start two threads into the same object
	if (_running) return;

	// Creating an OS specific thread, using the static callback.
	try
	{
		this->_thread = new std::thread(ZThread::dispatch, this);
	}
	catch (...)
	{
		Logger::getInstance() << Logger::Error << "Can't create new thread" << Logger::Flush;
	}
	_running = true;
}

void    ZThread::sleep()
{
	std::unique_lock<std::mutex> lock(this->_mutex);
	this->_sleepScheduled = true;
}

void    ZThread::awake()
{
	std::unique_lock<std::mutex> lock(this->_mutex);
	this->_cond.notify_one();
	//Logger::getInstance() << Logger::Info << Logger::PrintStdOut << "Removing thread " << this->m_pid << " from bed" << Logger::Flush;
	this->_asleep = false;
}

void    ZThread::checkSleep(bool forceSleep)
{
	std::unique_lock<std::mutex> lock(this->_mutex);
	if (!this->_asleep && (forceSleep || this->_sleepScheduled))
	{
		this->_asleep = true;
		this->_sleepScheduled = false;
		this->_cond.wait(lock);
	}
}


void*	ZThread::dispatch(ZThread* thread)
{
	// Call the thread code
	thread->code();

	// When code return, kill the thread object.
	thread->_thread->join();
	delete thread;
	return (0);
}

bool        ZThread::sleeping() const
{
	return this->_asleep;
}

