#include "ClientSocket_unix.h"

ClientSocket::ClientSocket(int acceptedSocket) 
{
	listenSocket = acceptedSocket;
}

ClientSocket::~ClientSocket()
{
	this->close(true);
}

int		ClientSocket::send(char *buf, int length) const
{
	int iResult = ::send(listenSocket, buf, length, 0);
	if (iResult == SOCKET_ERROR)
	{
		throw 0;
	}
	return (iResult);
}

int ClientSocket::recv( char *buf, int length ) const
{
	int iResult = ::recv(listenSocket, buf, length, 0);
	if (iResult == SOCKET_ERROR)
	{
		throw 0;
	}
	return (iResult);
}

