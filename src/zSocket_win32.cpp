#include "zSocket_win32.h"

zSocket::zSocket()
{
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != NO_ERROR)
		throw 0;
	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET)
	{
		WSACleanup();
		throw 0;
	}
}

zSocket::~zSocket()
{
	WSACleanup();
}

void zSocket::bind( std::string address, int port ) const
{
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr(address.c_str());
	service.sin_port = htons(port);

	if (::bind(listenSocket, (SOCKADDR*) &service, sizeof(service)) == SOCKET_ERROR)
	{
		closesocket(listenSocket);
		WSACleanup();
		throw 0;
	}
}

void zSocket::listen( int queue ) const
{
	if (::listen(listenSocket, queue) == SOCKET_ERROR)
	{
		closesocket(listenSocket);
		WSACleanup();
		throw 0;
	}
}

zSocket *zSocket::accept()
{
	SOCKET acceptSocket = ::accept(listenSocket, NULL, NULL);
	if (acceptSocket == INVALID_SOCKET)
	{
		closesocket(listenSocket);
		WSACleanup();
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
		closesocket(listenSocket);
		WSACleanup();
		throw 0;
	}
	return (iResult);
}

int zSocket::recv( char *buf, int length ) const
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

void zSocket::close( bool shutdown ) const
{
	if (shutdown)
		::shutdown(listenSocket, SD_BOTH);
	closesocket(listenSocket);
}