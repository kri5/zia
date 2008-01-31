#ifndef __SOCKET_UNIX_H__
#define __SOCKET_UNIX_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "ISocket.h"

/// Socket class which will have generic code for both server and client socket.
class Socket : public ISocket
{
public:
	Socket();
	virtual ~Socket();
	/// Send data to the client.
	int	send(char *buf, int length) const;
	/// Read input data from the client.
	virtual int	recv(char *buf, int length) const = 0;
	//virtual int select() const = 0;
	/// Close the connection with auto-shutdown support.
	void close(bool shutdown) const;
protected:
	int listenSocket;
	static const int SOCKET_ERROR = -1;
};

#endif

