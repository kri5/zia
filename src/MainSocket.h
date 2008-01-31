#ifndef __MAINSOCKET_H__
#define __MAINSOCKET_H__

#include "zia.h"
#include "ISocket.h"

#ifdef WIN32
 #include "MainSocket_win32.h"
#else
 #include "MainSocket_unix.h"
#endif

#endif //__MAINSOCKET_H__

