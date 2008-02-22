#include "ClientSocket_unix.h"
#include "ZException.hpp"
#include "MemoryManager.hpp"

ClientSocket::ClientSocket(int acceptedSocket) : siostream(acceptedSocket) 
{
	listenSocket = acceptedSocket;
}

ClientSocket::~ClientSocket()
{
	this->close(true);
}

int             ClientSocket::send(const char *buf, int length) const
{
	int iResult = ::send(listenSocket, buf, length, 0);
	if (iResult == SOCKET_ERROR)
	{
		throw ZException<IClientSocket>(INFO, IClientSocket::Error::Send);
	}
	return (iResult);
}

int             ClientSocket::send(const std::string& buf, int length) const
{
    if (length == -1)
        return send(buf.c_str(), buf.size());
    return send(buf.c_str(), length);
}

int             ClientSocket::recv(char *buf, int length) const
{
	int iResult = ::recv(listenSocket, buf, length, 0);
	if (iResult == SOCKET_ERROR)
	{
		throw ZException<IClientSocket>(INFO, IClientSocket::Error::Recv);
	}
	return (iResult);
}

siostream&      ClientSocket::getStream()
{
    siostream* stream = new siostream(this->listenSocket);
    return *stream;
}

