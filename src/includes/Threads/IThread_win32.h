#ifndef ITHREAD_H
#define ITHREAD_H

#include "zia.h"
#include "IThreadbase.h"

class IThread : public IThreadbase
{
public:
	IThread();
	void			run();
	void			stop();
	virtual void	code() = 0;
	void			sleep();
	void			awake();
protected:
	void			checkSleep(bool forceSleep = false);
private:
	HANDLE			hThread;
};

#endif // ITHREAD_H