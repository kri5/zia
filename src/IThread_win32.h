#ifndef ITHREAD_H
#define ITHREAD_H

#include "zia.h"
#include "IThreadbase.h"

class IThread : public IThreadbase
{
public:
	void			run();
	void			stop();
	virtual void	code() = 0;
protected:
private:
	HANDLE			hThread;
};

#endif // ITHREAD_H