#include <errno.h>
#include <string.h>

#include "MainSocket_unix.h"
#include "ZException.hpp"
#include "Vhost.h"

#include "MemoryManager.hpp"

MainSocket::MainSocket(const NetworkID* netId, int queue, const std::vector<Vhost*>& vhosts) : _netId(netId), _vhosts(vhosts)
{
	listenSocket = ::socket(PF_INET, SOCK_STREAM, 0);
	if (listenSocket == SOCKET_ERROR)
		throw ZException<MainSocket>(INFO, MainSocket::Error::Create);
	bind(netId);
	listen(queue);
}

MainSocket::~MainSocket()
{
	this->close(true);
}

void	MainSocket::bind(const NetworkID* netId) const
{
	struct sockaddr_in service;
	memset(&service, 0, sizeof(service));
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = netId->getAddress().getInAddr();
	service.sin_port = netId->getPort().getHtonsPort();

	if (::bind(listenSocket, (struct sockaddr *)&service, sizeof(service)) == SOCKET_ERROR)
	{
		throw ZException<IMainSocket>(INFO, IMainSocket::Error::Bind, strerror(errno));
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

const std::vector<Vhost*>&   MainSocket::getAssociatedVhosts()
{
    return _vhosts;
}

