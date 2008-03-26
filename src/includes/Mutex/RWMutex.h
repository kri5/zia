#ifndef RWMUTEX_H
#define RWMUTEX_H

#ifdef WIN32
 #include "RWMutex_win32.h"
#else
 #include "RWMutex_unix.h"
#endif

#endif // RWMUTEX_H
