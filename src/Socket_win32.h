#ifndef __SOCKET_WIN32_H__
#define __SOCKET_WIN32_H__

#include "zia.h"

class Socket
{
public:
	Socket();
	~Socket();
	
	/// Close the connection with auto-shutdown support.
	void close(bool shutdown) const;
protected:
	WSADATA wsaData;
	SOCKET listenSocket;
};

#endif // __SOCKET_WIN32_H__

