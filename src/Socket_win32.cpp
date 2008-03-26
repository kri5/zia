#include "Sockets/Socket_win32.h"

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

const ISocket&	Socket::operator>>(struct pollfd& pdfs) const
{
	pdfs.fd = this->listenSocket;
	pdfs.events = POLLRDNORM | POLLWRNORM;
	return *this;
}

bool            Socket::isSet(const struct pollfd& pdfs) const
{
	if (pdfs.revents & (POLLRDNORM | POLLWRNORM))
		return true;
	return false;
}