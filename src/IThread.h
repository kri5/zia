#ifndef __ITHREAD_H__
#define __ITHREAD_H__

#include <iostream>

#ifndef WIN32
 #include <pthread.h>
#endif

class IThread
{
public:
	virtual		~IThread() { }
	void		run();
	unsigned long	pid() const { return m_pid; }

protected:
	IThread();
	virtual void	code() = 0;

private:
	int		running;
	unsigned long	m_pid;
	static void*	dispatch(void* thread_obj);
};

#endif // __ITHREAD_H__

