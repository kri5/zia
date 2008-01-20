#include "IThread_win32.h"

void			IThread::run()
{
	// Don't start two threads into the same object
	if (running) return;

	// Creating an OS specific thread, using the static callback.
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)IThread::dispatch, this, 0, &m_pid);
	running = 1;
}

void      IThread::stop()
{
	// Don't stop a thread already stopped.
	if (!running) return;

	// Stopping the thread.
	TerminateThread(hThread, 0);
	running = 0;
}