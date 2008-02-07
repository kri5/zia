#ifndef SOCKET_UNIX_H__
#define SOCKET_UNIX_H__

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

/// Unix Socket class which will have generic code for both server and client socket.
class Socket
{
public:
	/// Will create a socket.
	Socket();
	/// Close the socket.
	virtual ~Socket();
	/// Send data to the client.
	int	send(const char *buf, int length) const;
	/// Overloaded method that take a sring
	int send(const std::string& buf, int length) const;
	/// Read input data from the client.
	virtual int	recv(char *buf, int length) const = 0;
	//virtual int select() const = 0;
	/// Close the connection with auto-shutdown support.
	virtual void close(bool shutdown) const;
protected:
	int listenSocket;
	static const int SOCKET_ERROR = -1;
};

#endif

