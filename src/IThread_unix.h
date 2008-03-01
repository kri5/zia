#ifndef ITHREAD_H
#define ITHREAD_H

#include <pthread.h>
#include "IThreadbase.h"

class IThread : public IThreadbase
{
public:
    virtual ~IThread(){}
	void			run();
	void			stop();
	virtual void	code() = 0;
protected:
private:
};

#endif // ITHREAD_H
