#include <errno.h>

#include "Sockets/ClientSocket_win32.h"
#include "Logger.hpp"

#include "API/IModule.h"
#include "Modules/ModuleManager.h"
#include "ZException.hpp"
#include "MemoryManager.hpp"

int	ClientSocket::_nbSockets = 0;

ClientSocket::ClientSocket(int acceptedSocket) : Socket()
{
	listenSocket = acceptedSocket;
	++ClientSocket::_nbSockets;
}

ClientSocket::~ClientSocket()
{
	this->close(true);
	--ClientSocket::_nbSockets;
}

int ClientSocket::send(const char *buf, int length) const
{
	ModuleManager::getInstance().call(zAPI::IModule::NetworkHook, zAPI::IModule::onSendEvent, buf, length);
	int iResult = ::send(listenSocket, buf, length, 0);
	if (iResult == SOCKET_ERROR)
		Logger::getInstance() << Logger::Warning << "Can't send data (" << strerror(WSAGetLastError()) << ')' << Logger::Flush;
	return (iResult);
}
//FIXME : what's this for ?
int   ClientSocket::send(const std::string& buf, int length) const
{
    std::string tmp = buf;
    int ret;
    int remainingSize = length;
    if (length == -1)
        remainingSize = tmp.size();

    while (remainingSize > 0)
    {
        ret = send(tmp.c_str(), remainingSize);
        if (ret == -1) return -1;
        tmp.erase(0, ret);
        remainingSize -= ret;
    }
    return 0;
}

int ClientSocket::recv( char *buf, int length ) const
{
	int iResult = ::recv(listenSocket, buf, length, 0);
	if (iResult == SOCKET_ERROR)
		Logger::getInstance() << Logger::Warning << "Can't receive data (" << strerror(WSAGetLastError()) << ')' << Logger::Flush;
    else
		ModuleManager::getInstance().call(zAPI::IModule::NetworkHook, zAPI::IModule::onReceiveEvent, buf, iResult);
	return (iResult);
}

int             ClientSocket::getNativeSocket() const
{
    return listenSocket;
}

int				ClientSocket::countSockets()
{
	return ClientSocket::_nbSockets;
}
