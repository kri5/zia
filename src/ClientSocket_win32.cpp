#include "ClientSocket_win32.h"

ClientSocket::ClientSocket(int acceptedSocket) : Socket()
{
	listenSocket = acceptedSocket;
}

ClientSocket::~ClientSocket()
{
	this->close(true);
}

int ClientSocket::recv( char *buf, int length ) const
{
	int iResult = ::recv(listenSocket, buf, length, 0);
	if (iResult == SOCKET_ERROR)
	{
		closesocket(listenSocket);
		WSACleanup();
		throw 0;
	}
	return (iResult);
}

int ClientSocket::send(char *buf, int length) const
{
	int iResult = ::send(listenSocket, buf, length, 0);
	if (iResult == SOCKET_ERROR)
	{
		closesocket(listenSocket);
		WSACleanup();
		throw 0;
	}
	return (iResult);
}
