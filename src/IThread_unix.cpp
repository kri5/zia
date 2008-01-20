#include "IThread_unix.h"

void			IThread::run()
{
	// Don't start two threads into the same object
	if (running) return;

	// Creating an OS specific thread, using the static callback.
	pthread_create(&m_pid, NULL, IThread::dispatch, this); 
	running = 1;
}

void      IThread::stop()
{
	// Don't stop a thread already stopped.
	if (!running) return;

	// Stopping the thread.
	pthread_cancel(m_pid);
	running = 0;
}