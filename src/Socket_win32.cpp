#include "Socket_win32.h"

Socket::Socket()
{
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != NO_ERROR)
		throw 0;
	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET)
	{
		WSACleanup();
		throw 0;
	}
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