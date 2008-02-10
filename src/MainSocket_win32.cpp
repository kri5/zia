#include "MainSocket_win32.h"

#include "MemoryManager.hpp"

MainSocket::MainSocket(int queue, std::string address, int port) : Socket()
{
	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET)
	{
		WSACleanup();
		throw ZException<IMainSocket>(INFO, IMainSocket::Error::Create);
	}
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
		throw ZException<IMainSocket>(INFO, IMainSocket::Error::Bind);
	}
}

void MainSocket::listen( int queue ) const
{
	if (::listen(listenSocket, queue) == SOCKET_ERROR)
	{
		closesocket(listenSocket);
		WSACleanup();
		throw ZException<IMainSocket>(INFO, IMainSocket::Error::Listen);
	}
}

ClientSocket *MainSocket::accept()
{
	SOCKET acceptSocket = ::accept(listenSocket, NULL, NULL);
	if (acceptSocket == INVALID_SOCKET)
	{
		closesocket(listenSocket);
		WSACleanup();
		throw ZException<IMainSocket>(INFO, IMainSocket::Error::Accept);
	}
	ClientSocket *ret = new ClientSocket(acceptSocket);
	return (ret);
}

