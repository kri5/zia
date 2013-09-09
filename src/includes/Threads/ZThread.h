#ifndef ITHREADBASE_H__
#define ITHREADBASE_H__

#include "zia.h"
#include <thread>
#include <mutex>
#include <condition_variable>

#undef sleep

class ZThread 
{
	public:
		/// Virtual destructor, can be reimplemented if needed.
		virtual			~ZThread() { }
		/// Method to be called to start the thread.
		virtual void	run();
		/// Will turn the thread into sleeping mode.
		virtual void    sleep();
		/// Will awake the thread.
		virtual void    awake();

		/// Method for calling the threaded method "code" with "this" as parameter.
		static void*	dispatch(ZThread* thread_obj);
		/// Will check if thread is sleeping.
		bool            sleeping() const;
	protected:
		ZThread();

		virtual void	checkSleep(bool forceSleep);
		/// Entry point of the thread.
		virtual void	code() = 0;
		/// Indicate if the thread is running or not.
		bool			_running;
		/// Internal value to save the thread pid.
		std::mutex          	_mutex;
		bool            	_sleepScheduled;
		bool            	_asleep;
		std::condition_variable	_cond;
		std::thread*		_thread;

	private:
};

#endif // __ITHREADBASE_H__
