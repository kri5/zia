#include "Sockets/MainSocket_win32.h"

#include "Sockets/ClientSocket.h"

#include "Modules/ModuleManager.h"
#include "ZException.hpp"
#include "MemoryManager.hpp"
#include "RootConfig.hpp"
#include "Network/Vhost.h"

#include "API/INetwork.h"

#ifdef _WIN32
typedef int socklen_t;
#endif // _WIN32

MainSocket::MainSocket(const NetworkID* netId, int queue, const std::vector<Vhost*>& vhosts) : _netId(netId), _vhosts(vhosts)
{
	if (WSAStartup(MAKEWORD(2,2), &_wsaData) != NOERROR)
		throw ZException<IMainSocket>(INFO, MainSocket::Error::Create);

	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET)
	{
		WSACleanup();
		throw ZException<IMainSocket>(INFO, IMainSocket::Error::Create);
	}
	bind(netId);
	listen(queue);
}

MainSocket::~MainSocket()
{
	WSACleanup();
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

zAPI::IClientSocket*	MainSocket::accept()
{
	struct sockaddr_in    clientSin;
	int                   clientSinLen;

	clientSinLen = sizeof(clientSin);
	SOCKET acceptSocket = ::accept(listenSocket, (struct sockaddr*)&clientSin, (socklen_t*)&clientSinLen);
	if (acceptSocket == INVALID_SOCKET)
	{
		closesocket(listenSocket);
		WSACleanup();
        //FIXME: can be a little ruff...
		throw ZException<IMainSocket>(INFO, IMainSocket::Error::Accept);
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

SOCKET						MainSocket::getNativeSocket() const
{
	return listenSocket;
}
