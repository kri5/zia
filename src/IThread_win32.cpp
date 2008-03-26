#include "Threads/IThread_win32.h"
#include "Logger.hpp"
#include "MutexLock.hpp"

#include "MemoryManager.hpp"

IThread::IThread()
{
	_mutex = new Mutex();
}

void			IThread::run()
{
	// Don't start two threads into the same object
	if (_running) return;

	// Creating an OS specific thread, using the static callback.
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)IThread::dispatch, this, 0, &m_pid);
	_running = true;
}

void      IThread::stop()
{
	// Don't stop a thread already stopped.
	if (!_running) return;

	// Stopping the thread.
	TerminateThread(hThread, 0);
	CloseHandle(hThread);
	_running = false;
}

void	IThread::awake()
{
	MutexLock	get_lock(this->_mutex);
	this->_asleep = false;
	ResumeThread(this->hThread);
}

void	IThread::sleep()
{
	MutexLock	get_lock(this->_mutex);
	this->_sleepScheduled = true;
}

void	IThread::checkSleep(bool forceSleep)
{
	this->_mutex->lock();
	if (!this->_asleep && (forceSleep || this->_sleepScheduled))
	{
		//Logger::getInstance() << Logger::Info << "putting thread #" << this->m_pid << " to bed" << Logger::Flush;
		this->_asleep = true;
		this->_sleepScheduled = false;
		this->_mutex->unlock();
		SuspendThread(this->hThread);
		//Logger::getInstance() << Logger::Info << Logger::PrintStdOut << "Thread " << this->m_pid << " is awake" << Logger::Flush;
		return ;
	}
	this->_mutex->unlock();
}