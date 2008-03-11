#ifndef ITHREADBASE_H__
#define ITHREADBASE_H__

#include "zia.h"
#include "Mutex.h"

class IThreadbase
{
public:
	/// Virtual destructor, can be reimplemented if needed.
	virtual			~IThreadbase() { }
	/// Method to be called to start the thread.
	virtual void	run() = 0;
	/// Stop the thread.
	virtual void	stop() = 0;
    /// Will turn the thread into sleeping mode.
    virtual void    sleep() = 0;
    /// Will awake the thread.
    virtual void    awake() = 0;
	/// Return the process ID of the thread.
	unsigned long	pid() const { return m_pid; }

	/// Method for calling the threaded method "code" with "this" as parameter.
	static void*	dispatch(void* thread_obj);
    /// Will check if thread is sleeping.
    bool            sleeping() const;
protected:
	IThreadbase();
	/// Entry point of the thread.
	virtual void	code() = 0;
	/// Indicate if the thread is running or not.
	bool			_running;
	/// Internal value to save the thread pid.
	unsigned long	m_pid;
    Mutex*          _mutex;
    bool            _sleepScheduled;
    bool            _asleep;

private:
};

#endif // __ITHREADBASE_H__
