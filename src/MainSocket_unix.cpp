#include <errno.h>
#include <string.h>

#include "Sockets/MainSocket_unix.h"
#include "ZException.hpp"
#include "Network/Vhost.h"
#include "Modules/IModuleManager.h"
#include "Modules/ModuleManager.h"

#include "MemoryManager.hpp"
#include "API/INetwork.h"

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

zAPI::IClientSocket*	MainSocket::accept()
{
	int acceptSocket = ::accept(listenSocket, NULL, NULL);
	if (acceptSocket == SOCKET_ERROR)
	{
        Logger::getInstance() << Logger::Warning << "Can't accept client (" << strerror(errno) << ')' << Logger::Flush;
		//throw ZException<IMainSocket>(INFO, MainSocket::Error::Accept, strerror(errno));
	}
    zAPI::IClientSocket*  ret = ModuleManager::getInstance().call(zAPI::IModule::NetworkHook, acceptSocket, &zAPI::INetwork::onAccept);
    if (ret == NULL)
        ret = new ClientSocket(acceptSocket);
	return (ret);
}

const std::vector<Vhost*>&   MainSocket::getAssociatedVhosts()
{
    return _vhosts;
}

int                         MainSocket::getNativeSocket() const
{
    return listenSocket;
}

bool                        MainSocket::isClosed() const
{
    return this->_closed;
}

void                        MainSocket::close(bool shutdown)
{
    if (this->_closed == false)
    {
        if (shutdown)
            if (::shutdown(listenSocket, SHUT_RDWR))
                Logger::getInstance() << Logger::Error << "Can't shutdown socket : " << strerror(errno) << Logger::Flush;
        if (::close(listenSocket) < 0)
            Logger::getInstance() << Logger::Error << "Can't close socket : " << strerror(errno) << Logger::Flush;
        this->_closed = true;
    }
}
