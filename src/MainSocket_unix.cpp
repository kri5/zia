#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <cstdlib>

#include "Sockets/MainSocket_unix.h"
#include "ZException.hpp"
#include "Network/Vhost.h"
#include "Modules/IModuleManager.h"
#include "Modules/ModuleManager.h"
#include "RootConfig.hpp"
#include "API/INetwork.h"

#include "MemoryManager.hpp"

MainSocket::MainSocket(const NetworkID* netId, int queue, const std::vector<Vhost*>& vhosts) : _closed(false), _netId(netId), _vhosts(vhosts)
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
    struct sockaddr_in    clientSin;
    int                   clientSinLen;

    clientSinLen = sizeof(clientSin);
	int acceptSocket = ::accept(listenSocket, (struct sockaddr*)&clientSin, (socklen_t*)&clientSinLen);
	if (acceptSocket == SOCKET_ERROR)
	{
        Logger::getInstance() << Logger::Warning << "Can't accept client (" << strerror(errno) << ')' << Logger::Flush;
        return NULL;
		//throw ZException<IMainSocket>(INFO, MainSocket::Error::Accept, strerror(errno));
	}
    zAPI::IClientSocket*  ret = ModuleManager::getInstance().call(zAPI::IModule::NetworkHook, acceptSocket, inet_ntoa(clientSin.sin_addr), this->_netId->getPort().getPort(), RootConfig::getConfig(), &zAPI::INetwork::onAccept);
    if (ret == NULL)
        ret = new ClientSocket(acceptSocket, inet_ntoa(clientSin.sin_addr), this->_netId->getPort().getPort());
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
