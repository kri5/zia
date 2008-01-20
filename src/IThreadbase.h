#ifndef __ITHREADBASE_H__
#define __ITHREADBASE_H__

#include "zia.h"

class IThreadbase
{
public:
	/// Virtual destructor, can be reimplemented if needed.
	virtual			~IThreadbase() { }
	/// Method to be called to start the thread.
	virtual void	run() = 0;
	/// Stop the thread.
	virtual void	stop() = 0;
	/// Return the process ID of the thread.
	unsigned long	pid() const { return m_pid; }

protected:
	IThreadbase();
	/// Entry point of the thread.
	virtual void	code() = 0;
	/// Indicate if the thread is running or not.
	bool				running;
	/// Internal value to save the thread pid.
	unsigned long	m_pid;
	/// Method for calling the threaded method "code" with "this" as parameter.
	static void*	dispatch(void* thread_obj);

private:
};

#endif // __ITHREADBASE_H__
