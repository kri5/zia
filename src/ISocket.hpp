#ifndef __ISOCKET_HPP__
#define __ISOCKET_HPP__

#include <iostream>
#include "zia.h"

/// Socket interface to plugin OS dependant code.
template <typename S>
class ISocket
{
public:
	/// Bind the socket on a specified host and port.
	virtual void bind(std::string address, int port) const = 0;
	/// Listen wait for incoming connections.
	virtual void listen(int queue) const = 0;
	/// Spawn a new socket for the new client.
	virtual S *accept() = 0;
	/// Send data to the client.
	virtual int	send(char *buf, int length) const = 0;
	/// Read input data from the client.
	virtual int	recv(char *buf, int length) const = 0;
	//virtual int select() const = 0;
	/// Close the connection with auto-shutdown support.
	virtual void close(bool shutdown) const = 0;
};


#endif // __ISOCKET_HPP__

