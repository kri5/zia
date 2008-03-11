#include <errno.h>

#include "ClientSocket_unix.h"
#include "ZException.hpp"
#include "MemoryManager.hpp"

ClientSocket::ClientSocket(int acceptedSocket) 
{
	listenSocket = acceptedSocket;
}

ClientSocket::~ClientSocket()
{
	this->close(true);
}

int             ClientSocket::send(const char *buf, int length) const
{
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
	return (iResult);
}

IClientSocket&   ClientSocket::operator<<(const std::string& buf)
{
    send(buf, buf.size());
    return *this;
}

