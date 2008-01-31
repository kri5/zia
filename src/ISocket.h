#ifndef __ISOCKET_HPP__
#define __ISOCKET_HPP__

#include "zia.h"

/// Socket interface to plugin OS dependant code.
class ISocket
{
public:
	/// Send data to the client.
	virtual int	send(char *buf, int length) const = 0;
	/// Read input data from the client.
	virtual int	recv(char *buf, int length) const = 0;
	//virtual int select() const = 0;
	/// Close the connection with auto-shutdown support.
	virtual void close(bool shutdown) const = 0;
};


#endif // __ISOCKET_HPP__

