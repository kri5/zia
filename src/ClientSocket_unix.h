#ifndef __CLIENT_SOCKET_UNIX_H__
#define __CLIENT_SOCKET_UNIX_H__

#include "zia.h"
#include "Socket_unix.h"

class ClientSocket : public Socket
{
public:
	ClientSocket(int acceptedSocket);
	~ClientSocket();
	int	send(char *buf, int length) const;
	int	recv(char *buf, int length) const;
};

#endif //__CLIENT_SOCKET_UNIX_H__
