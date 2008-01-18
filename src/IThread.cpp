#include "IThread.h"


IThread::IThread() : running(0), m_pid(0L)
{
}

void			IThread::run()
{
	// Don't start two threads into the same object
	if (running) return;

	// Creating an OS specific thread, using the static callback.
	pthread_create(&m_pid, NULL, IThread::dispatch, this); 
	running = 1;
}

void*			IThread::dispatch(void* thread_obj)
{
	// Call the thread code
	((IThread*)thread_obj)->code();

	// When code return, kill the thread object.
	delete (IThread*)thread_obj;
}


