#ifndef __SOCKET_H__
#define __SOCKET_H__

#ifdef WIN32
	#include "Socket_win32.h"
#else
	#include "Socket_unix.h"
#endif //WIN32

#endif //__SOCKET_H__