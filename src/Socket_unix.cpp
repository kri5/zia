#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "Socket_unix.h"

#include "MemoryManager.hpp"

Socket::Socket()
{
}

Socket::~Socket()
{
}

void Socket::close(bool shutdown) const
{
	if (shutdown)
		::shutdown(listenSocket, SHUT_RDWR);
	::close(listenSocket);
}

