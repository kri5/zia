#include <errno.h>

#include "IThread_unix.h"

#include "MemoryManager.hpp"

void			IThread::run()
{
	// Don't start two threads into the same object
	if (running) return;

	// Creating an OS specific thread, using the static callback.
    if (pthread_create(&m_pid, NULL, IThread::dispatch, this) != 0)
    {
        std::cout << "Errno == " << errno << std::endl;
        std::cout << "EAGAIN == " << EAGAIN << " EINVAL == " << EINVAL << " EPERM == " << EPERM << std::endl;
        std::cerr << strerror(errno) << std::endl;
        exit(0);
    }
	running = true;
}

void      IThread::stop()
{
	// Don't stop a thread already stopped.
	if (!running) return;

	// Stopping the thread.
    pthread_cancel(m_pid);
    //pthread_exit(NULL);
    //pthread_stop()
	running = false;
}
