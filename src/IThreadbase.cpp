#include "IThreadbase.h"


IThreadbase::IThreadbase() : running(false), m_pid(0L)
{
}

void*			IThreadbase::dispatch(void* thread_obj)
{
	// Call the thread code
	((IThreadbase*)thread_obj)->code();

	// When code return, kill the thread object.
	delete (IThreadbase*)thread_obj;
	return (0);
}
