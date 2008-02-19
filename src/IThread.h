#ifndef __ITHREAD_H__
#define __ITHREAD_H__

#include "zia.h"

#ifdef WIN32
 #include "IThread_win32.h"
#else
 #include "IThread_unix.h"
#endif

#endif //__ITHREAD_H__

