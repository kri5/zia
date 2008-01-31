#ifndef __CLIENTSOCKET_H__
#define __CLIENTSOCKET_H__

#include "zia.h"
#include "ISocket.h"

#ifdef WIN32
 #include "ClientSocket_win32.h"
#else
 #include "ClientSocket_unix.h"
#endif

#endif //__CLIENTSOCKET_H__

