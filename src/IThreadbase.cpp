#include "IThreadbase.h"

#include "MemoryManager.hpp"

IThreadbase::IThreadbase() : running(false), m_pid(0L)
{
}

void*			IThreadbase::dispatch(void* thread_obj)
{
    Logger::getInstance() << Logger::Info << Logger::NoStdOut << "Thread #" << ((IThreadbase*)thread_obj)->pid() << " started." << Logger::Flush;
	// Call the thread code
	((IThreadbase*)thread_obj)->code();

    Logger::getInstance() << Logger::Info << Logger::NoStdOut << "Thread #" << ((IThreadbase*)thread_obj)->pid() << " ended." << Logger::Flush;
	// When code return, kill the thread object.
    ((IThreadbase*)thread_obj)->stop();
	delete (IThreadbase*)thread_obj;
	return (0);
}
