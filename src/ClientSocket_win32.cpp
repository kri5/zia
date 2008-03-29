#include <errno.h>

#include "Sockets/ClientSocket_win32.h"
#include "Logger.hpp"

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
	return (iResult);
}

IClientSocket&   ClientSocket::operator<<(const std::string& buf)
{
    send(buf, buf.size());
    return *this;
}

int				ClientSocket::countSockets()
{
	return ClientSocket::_nbSockets;
}