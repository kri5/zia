#include "MainSocket_unix.h"
#include "ZException.hpp"

#include "MemoryManager.hpp"

MainSocket::MainSocket(NetworkID* netId, int queue)
{
	listenSocket = ::socket(PF_INET, SOCK_STREAM, 0);
	if (listenSocket == SOCKET_ERROR)
		throw ZException<MainSocket>(INFO, MainSocket::Error::Create);
	bind(netId);
	listen(queue);
	_netId = netId;
}

MainSocket::~MainSocket()
{
	this->close(true);
}

void	MainSocket::bind(NetworkID* netId) const
{
	struct sockaddr_in service;
	memset(&service, 0, sizeof(service));
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr(netId->getAddress()->getAddr().c_str());
	service.sin_port = htons(netId->getPort()->getPort());

	if (::bind(listenSocket, (struct sockaddr *)&service, sizeof(service)) == SOCKET_ERROR)
	{
		throw ZException<IMainSocket>(INFO, IMainSocket::Error::Bind);
	}
}


void	MainSocket::listen(int queue) const
{
	if (::listen(listenSocket, queue) == SOCKET_ERROR)
	{
		throw ZException<IMainSocket>(INFO, IMainSocket::Error::Listen);
	}
}

ClientSocket*	MainSocket::accept()
{
	int acceptSocket = ::accept(listenSocket, NULL, NULL);
	if (acceptSocket == SOCKET_ERROR)
	{
		throw ZException<MainSocket>(INFO, MainSocket::Error::Accept);
	}
	ClientSocket *ret = new ClientSocket(acceptSocket);
	return (ret);
}

