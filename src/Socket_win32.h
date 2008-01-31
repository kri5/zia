#ifndef __SOCKET_WIN32_H__
#define __SOCKET_WIN32_H__

#include "zia.h"
#include "ISocket.h"

class Socket : public ISocket
{
public:
	Socket();
	~Socket();
	/// Send data to the client.
	int	send(char *buf, int length) const;
	/// Read input data from the client.
	virtual int	recv(char *buf, int length) const = 0;
	//virtual int select() const = 0;
	/// Close the connection with auto-shutdown support.
	virtual void close(bool shutdown) const = 0;
protected:
	WSADATA wsaData;
	SOCKET listenSocket;
	static const int SOCKET_ERROR = -1;
};

#endif // __SOCKET_WIN32_H__

