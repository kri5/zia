#ifndef __ITHREADBASE_H__
#define __ITHREADBASE_H__

#include "zia.h"

class IThreadbase
{
public:
	virtual			~IThreadbase() { }
	virtual void	run() = 0;
	virtual void	stop() = 0;
	unsigned long	pid() const { return m_pid; }

protected:
	IThreadbase();
	virtual void	code() = 0;
	int				running;
	unsigned long	m_pid;
	static void*	dispatch(void* thread_obj);

private:
};

#endif // __ITHREADBASE_H__
