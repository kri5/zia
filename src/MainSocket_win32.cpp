#include "Sockets/MainSocket_win32.h"

#include "ZException.hpp"
#include "MemoryManager.hpp"

MainSocket::MainSocket(const NetworkID* netId, int queue, const std::vector<const Vhost*>& vhosts) : Socket(), _netId(netId), _vhosts(vhosts)
{
	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET)
	{
		WSACleanup();
		throw ZException<IMainSocket>(INFO, IMainSocket::Error::Create);
	}
	bind(netId);
	listen(queue);
}

void MainSocket::bind(const NetworkID* netId) const
{
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = netId->getAddress().getInAddr();
	service.sin_port = netId->getPort().getHtonsPort();

	if (::bind(listenSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
	{
		closesocket(listenSocket);
		WSACleanup();
		throw ZException<IMainSocket>(INFO, IMainSocket::Error::Bind);
	}
}

void MainSocket::listen(int queue) const
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

const std::vector<const Vhost*>&   MainSocket::getAssociatedVhosts()
{
    return _vhosts;
}
