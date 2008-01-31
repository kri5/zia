#ifndef __CLIENT_SOCKET_WIN32_H__
#define __CLIENT_SOCKET_WIN32_H__

#include "zia.h"
#include "Socket_win32.h"

class ClientSocket : public Socket
{
public:
	ClientSocket(int acceptedSocket);
	~ClientSocket();
	int	recv(char *buf, int length) const;
private:
};

#endif //__CLIENT_SOCKET_WIN32_H__

