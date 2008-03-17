#ifndef ITHREAD_H
#define ITHREAD_H

#include <pthread.h>
#include "IThreadbase.h"

class IThread : public IThreadbase
{
public:
    IThread();
    virtual ~IThread();
	void			run();
	void			stop();
    void            sleep();
    void            awake();
	virtual void	code() = 0;
protected:
    void            checkSleep(bool forceSleep = false);
private:
    pthread_cond_t  _cond;
};

#endif // ITHREAD_H
