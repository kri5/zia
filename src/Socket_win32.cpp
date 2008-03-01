#include "Socket_win32.h"

#include "ZException.hpp"
#include "MemoryManager.hpp"

Socket::Socket()
{
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != NO_ERROR)
		throw ZException<Socket>(INFO, Socket::Error::Initialize);
}

Socket::~Socket()
{
	WSACleanup();
}

void Socket::close(bool shutdown)
{
	if (shutdown)
		::shutdown(listenSocket, SD_BOTH);
	closesocket(listenSocket);
}

const ISocket&	Socket::operator >>(fd_set &fds) const
{
	FD_SET(this->listenSocket, &fds);
	return *this;
}

bool			Socket::isSet(const fd_set& fds) const
{
	return (FD_ISSET(this->listenSocket, &fds) != 0);
}