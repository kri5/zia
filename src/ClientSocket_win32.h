#ifndef __CLIENT_SOCKET_WIN32_H__
#define __CLIENT_SOCKET_WIN32_H__

#include "zia.h"
#include "Socket_win32.h"
#include "IClientSocket.h"

class ClientSocket : public Socket, public IClientSocket
{
public:
	ClientSocket(int acceptedSocket);
	~ClientSocket();
	int	recv(char *buf, int length) const;
	int send(const char *buf, int length) const;
private:
};

#endif //__CLIENT_SOCKET_WIN32_H__

