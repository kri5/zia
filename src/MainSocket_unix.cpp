#include <errno.h>
#include <string.h>

#include "Sockets/MainSocket_unix.h"
#include "ZException.hpp"
#include "Network/Vhost.h"
#include "API/IModuleManager.h"
#include "Modules/ModuleManager.h"

#include "MemoryManager.hpp"

MainSocket::MainSocket(const NetworkID* netId, int queue, const std::vector<Vhost*>& vhosts) : _netId(netId), _vhosts(vhosts)
{
	listenSocket = ::socket(PF_INET, SOCK_STREAM, 0);
	if (listenSocket == SOCKET_ERROR)
		throw ZException<IMainSocket>(INFO, MainSocket::Error::Create);
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

    int yes = 1;
    if (setsockopt(this->listenSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

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

IClientSocket*	MainSocket::accept()
{
	int acceptSocket = ::accept(listenSocket, NULL, NULL);
	if (acceptSocket == SOCKET_ERROR)
	{
        Logger::getInstance() << Logger::Warning << "Can't accept client (" << strerror(errno) << ')' << Logger::Flush;
		//throw ZException<IMainSocket>(INFO, MainSocket::Error::Accept, strerror(errno));
	}
    IClientSocket*  ret = ModuleManager::getInstance().call(IModuleManager::NetworkHook, IModule::onAcceptEvent, acceptSocket);
    if (ret == NULL)
        ret = new ClientSocket(acceptSocket);
	return (ret);
}

const std::vector<Vhost*>&   MainSocket::getAssociatedVhosts()
{
    return _vhosts;
}

