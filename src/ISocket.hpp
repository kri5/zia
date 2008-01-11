#ifndef __ISOCKET_HPP__
#define __ISOCKET_HPP__

#include "zia.h"

class ISocket
{
public:
	virtual void bind(std::string& address, int port) const = 0;
	virtual void listen(int queue) const = 0;
	virtual ISocket accept() const = 0;
	virtual int	send(char *buf, int length) const = 0;
	virtual int	recv(char *buf, int length) const = 0;
	//virtual int select() const = 0;
	virtual void close(bool shutdown) const = 0;
};


#endif // __ISOCKET_HPP__