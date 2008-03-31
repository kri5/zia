#include <errno.h>

#include "Sockets/ClientSocket_unix.h"
#include "ZException.hpp"
#include "MemoryManager.hpp"
#include "API/IModuleManager.h"
#include "API/IModule.h"
#include "Modules/ModuleManager.h"

int ClientSocket::_nbSockets = 0;

ClientSocket::ClientSocket(int acceptedSocket) 
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
	this->close(true);
    --ClientSocket::_nbSockets;
}

int             ClientSocket::send(const char *buf, int length) const
{
    ModuleManager::getInstance().call(IModuleManager::NetworkHook, IModule::onSendEvent, buf, length);
    int iResult = ::send(listenSocket, buf, length, MSG_NOSIGNAL);

	if (iResult == SOCKET_ERROR)
        Logger::getInstance() << Logger::Error << "Send error : " << strerror(errno) << Logger::Flush;
	return (iResult);
}

int             ClientSocket::send(const std::string& buf, int length) const
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

int             ClientSocket::recv(char *buf, int length) const
{
	int iResult = ::recv(listenSocket, buf, length, 0);
	if (iResult == SOCKET_ERROR)
        Logger::getInstance() << Logger::Error << "Receive error : " << strerror(errno) << Logger::Flush;
    else
        ModuleManager::getInstance().call(IModuleManager::NetworkHook, IModule::onReceiveEvent, buf, iResult);
	return (iResult);
}

IClientSocket&   ClientSocket::operator<<(const std::string& buf)
{
    send(buf, buf.size());
    return *this;
}

int             ClientSocket::countSockets()
{
    return ClientSocket::_nbSockets;
}
