#ifndef SOCKET_UNIX_H__
#define SOCKET_UNIX_H__

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "ISocket.h"

/// Unix Socket class which will have generic code for both server and client socket.
class Socket : public ISocket
{
public:
	Socket();
	/// Close the socket.
	virtual ~Socket();
	/// Close the connection with auto-shutdown support.
	virtual void 	close(bool shutdown) const;
	int				getNativeSocket() const;
	const ISocket&	operator>>(fd_set&) const;
    bool            isSet(const fd_set&) const;
	int				getSocketValue() const;
protected:
	int listenSocket;
	static const int SOCKET_ERROR = -1;
};

#endif

