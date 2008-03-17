#ifndef MUTEX_H__
# define MUTEX_H__

#ifdef WIN32
	#include "Mutex_win32.h"
#else
	#include "Mutex_unix.h"
#endif

#endif //MUTEX_H__

