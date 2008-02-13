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

void Socket::close(bool shutdown) const
{
	if (shutdown)
		::shutdown(listenSocket, SD_BOTH);
	closesocket(listenSocket);
}
