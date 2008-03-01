#include "IThreadbase.h"

#include "MemoryManager.hpp"

IThreadbase::IThreadbase() : running(false), m_pid(0L)
{
}

void*			IThreadbase::dispatch(void* thread_obj)
{
	// Call the thread code
	((IThreadbase*)thread_obj)->code();

	// When code return, kill the thread object.
    ((IThreadbase*)thread_obj)->stop();
	delete (IThreadbase*)thread_obj;
	return (0);
}
