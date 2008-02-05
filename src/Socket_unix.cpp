#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "Socket_unix.h"

Socket::Socket()
{
	listenSocket = ::socket(PF_INET, SOCK_STREAM, 0);
	if (listenSocket == SOCKET_ERROR)
	{
		throw 0;
	}
}

Socket::~Socket()
{
}

int		Socket::send(const char* buf, int length) const
{
	int iResult = ::send(listenSocket, buf, length, 0);
	if (iResult == SOCKET_ERROR)
	{
		throw 0;
	}
	return (iResult);
}

int   Socket::send(const std::string& buf, int length = -1) const
{
    if (length == -1)
        return send(buf.c_str(), buf.size());
    return send(buf.c_str(), length);
}

void Socket::close( bool shutdown ) const
{
	if (shutdown)
		::shutdown(listenSocket, SHUT_RDWR);
	::close(listenSocket);
}

