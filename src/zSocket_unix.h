#ifndef __ZSOCKET_H__
#define __ZSOCKET_H__

#include "zia.h"
#include "ISocket.hpp"
#include <sys/types.h>
#include <sys/socket.h>

#define SOCKET_ERROR -1

class zSocket : public ISocket
{
public:
	zSocket();
	~zSocket();
	void bind(std::string address, int port) const;
	void listen(int queue) const;
	zSocket *accept();
	int	send(char *buf, int length) const;
	int	recv(char *buf, int length) const;
	void close(bool shutdown) const;
protected:
private:
	int listenSocket;
};

#endif // __ZSOCKET_H__
