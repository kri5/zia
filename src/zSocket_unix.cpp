#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "zSocket_unix.h"

zSocket::zSocket()
{
	listenSocket = ::socket(PF_INET, SOCK_STREAM, 0);
	if (listenSocket == SOCKET_ERROR)
	{
		throw 0;
	}
}

zSocket::~zSocket()
{
}

void zSocket::bind( std::string address, int port ) const
{
	struct sockaddr_in service;
	memset(&service, 0, sizeof(service));
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr(address.c_str());
	service.sin_port = htons(port);

	if (::bind(listenSocket, (struct sockaddr *)&service, sizeof(service)) == SOCKET_ERROR)
	{
		throw 0;
	}
}

void zSocket::listen( int queue ) const
{
	if (::listen(listenSocket, queue) == SOCKET_ERROR)
	{
		throw 0;
	}
}

zSocket *zSocket::accept()
{
	int acceptSocket = ::accept(listenSocket, NULL, NULL);
	if (acceptSocket == SOCKET_ERROR)
	{
		throw 0;
	}
	zSocket *ret = new zSocket();
	ret->listenSocket = acceptSocket;
	return (ret);
}

int zSocket::send( char *buf, int length ) const
{
	int iResult = ::send(listenSocket, buf, length, 0);
	if (iResult == SOCKET_ERROR)
	{
		throw 0;
	}
	return (iResult);
}

int zSocket::recv( char *buf, int length ) const
{
	int iResult = ::recv(listenSocket, buf, length, 0);
	if (iResult == SOCKET_ERROR)
	{
		throw 0;
	}
	return (iResult);
}

void zSocket::close( bool shutdown ) const
{
	if (shutdown)
		::shutdown(listenSocket, SHUT_RDWR);
	::close(listenSocket);
}
