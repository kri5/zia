#include "MainSocket_win32.h"

MainSocket::MainSocket(int queue, std::string address, int port) : Socket()
{
	bind(address, port);
	listen(queue);
}

void MainSocket::bind( std::string address, int port ) const
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

void MainSocket::listen( int queue ) const
{
	if (::listen(listenSocket, queue) == SOCKET_ERROR)
	{
		closesocket(listenSocket);
		WSACleanup();
		throw 0;
	}
}

ClientSocket *MainSocket::accept()
{
	SOCKET acceptSocket = ::accept(listenSocket, NULL, NULL);
	if (acceptSocket == INVALID_SOCKET)
	{
		closesocket(listenSocket);
		WSACleanup();
		throw 0;
	}
	ClientSocket *ret = new ClientSocket(acceptSocket);
	return (ret);
}

int MainSocket::recv( char *buf, int length ) const
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