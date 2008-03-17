#include "Threads/IThreadbase.h"
#include "Logger.hpp"

#include "MemoryManager.hpp"

IThreadbase::IThreadbase() : _running(false), m_pid(0L), _sleepScheduled(false), _asleep(false)
{
}

void*			IThreadbase::dispatch(void* threadParam)
{
    IThreadbase*    ptr = static_cast<IThreadbase*>(threadParam);
    //Logger::getInstance() << Logger::Info << Logger::NoStdOut << "Thread #" << ptr->pid() << " started." << Logger::Flush;

	// Call the thread code
    ptr->code();

    //Logger::getInstance() << Logger::Info << Logger::NoStdOut << "Thread #" << ptr->pid() << " ended." << Logger::Flush;

	// When code return, kill the thread object.
    ptr->stop();
	delete ptr;
	return (0);
}

bool        IThreadbase::sleeping() const
{
    return this->_asleep;
}

