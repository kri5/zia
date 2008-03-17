#ifndef IMUTEX_H
#define IMUTEX_H

class IMutex
{
public:
	virtual			~IMutex(){}
	/// Lock the mutex, if the mutex is already locked it will block until release.
	virtual void	lock() = 0;
	/// Try to get a lock on the mutex, return true in case of success, false otherwise.
	/// May be deprecated due to some strange behaviour.
	virtual bool	trylock() = 0;
	/// Unlock the mutex.
	virtual void	unlock() = 0;
protected:
	/// How much time a thread has been locked by the same thread.
    //FIXME : this has to go from here !
	int				lockCount;
private:
};

#endif // IMUTEX_H
