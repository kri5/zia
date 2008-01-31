#ifndef __CLIENT_SOCKET_WIN32_H__
#define __CLIENT_SOCKET_WIN32_H__

#include "zia.h"
#include "Socket_win32.h"

class ClientSocket : public Socket
{
public:
	ClientSocket();
	~ClientSocket();
	int	send(char *buf, int length) const;
	int	recv(char *buf, int length) const;
	void close(bool shutdown) const;
private:
}

#endif //__CLIENT_SOCKET_WIN32_H__

