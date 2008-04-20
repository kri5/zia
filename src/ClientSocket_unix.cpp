#include <errno.h>

#include "Sockets/ClientSocket_unix.h"
#include "ZException.hpp"
#include "MemoryManager.hpp"
#include "Modules/IModuleManager.h"
#include "API/IModule.h"
#include "API/INetwork.h"
#include "Modules/ModuleManager.h"

int ClientSocket::_nbSockets = 0;
Mutex   ClientSocket::_mutex;

ClientSocket::ClientSocket(int acceptedSocket, const std::string& address, int port) : deleted(false), _closed(false), _address(address), _port(port)
{
	listenSocket = acceptedSocket;
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(listenSocket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval));
    ClientSocket::_nbSockets++;
}

ClientSocket::~ClientSocket()
{
    deleted = true;
	this->close(true);
    MutexLock   getLock(ClientSocket::_mutex);
    --ClientSocket::_nbSockets;
}

int             ClientSocket::send(const char *buf, int length)
{
    ModuleManager::getInstance().call(zAPI::IModule::NetworkHook, buf, length, &zAPI::INetwork::onSend);
    int iResult = ::send(listenSocket, buf, length, MSG_NOSIGNAL);

	if (iResult == SOCKET_ERROR)
        Logger::getInstance() << Logger::Error << "Send error : " << strerror(errno) << Logger::Flush;
	return (iResult);
}

int             ClientSocket::send(const std::string& buf, int length)
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

int             ClientSocket::recv(char *buf, int length)
{
	int iResult = ::recv(listenSocket, buf, length, 0);
	if (iResult == SOCKET_ERROR)
        Logger::getInstance() << Logger::Error << "Receive error : " << strerror(errno) << Logger::Flush;
    else
        ModuleManager::getInstance().call(zAPI::IModule::NetworkHook, buf, iResult, &zAPI::INetwork::onReceive);
	return (iResult);
}

int             ClientSocket::getNativeSocket() const
{
    return listenSocket;
}

int             ClientSocket::countSockets()
{
    return ClientSocket::_nbSockets;
}

bool            ClientSocket::isClosed() const
{
    return this->_closed;
}

void            ClientSocket::close(bool shutdown)
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

const std::string&  ClientSocket::getIP() const
{
    return this->_address;
}

int                 ClientSocket::getPort() const
{
    return this->_port;
}
