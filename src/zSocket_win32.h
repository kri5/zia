#ifndef __ZSOCKET_H__
#define __ZSOCKET_H__

#include "zia.h"
#include "ISocket.hpp"

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
	WSADATA wsaData;
	SOCKET listenSocket;
};

#endif // __ZSOCKET_H__
