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
	void close(bool shutdown) const;
protected:
	WSADATA wsaData;
	SOCKET listenSocket;
};

#endif // __SOCKET_WIN32_H__

