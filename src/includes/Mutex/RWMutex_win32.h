#ifndef RWMUTEX_UNIX_H
#define RWMUTEX_UNIX_H

#include "zia.h"
#include "Mutex/IRWMutex.h"

class RWMutex : public IRWMutex
{
public:
        RWMutex();
        ~RWMutex();
        void                rdLock();
        void                wrLock();
        void                rdUnlock();
		void				wrUnlock();

private:
        PSRWLOCK			SRWLock;

};

#endif // RWMUTEX_UNIX_H
