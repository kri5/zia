#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#include "Socket_unix.h"
#include "Logger.hpp"

#include "MemoryManager.hpp"

Socket::Socket() : _closed(false)
{
}

Socket::~Socket()
{
}

void Socket::close(bool shutdown)
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

int	Socket::getNativeSocket() const
{
	return this->listenSocket;
}

const ISocket&	Socket::operator>>(fd_set& fds) const
{
	FD_SET(this->listenSocket, &fds);
	return *this;
}

int				Socket::getSocketValue() const
{
	return this->listenSocket;
}

bool            Socket::isSet(const fd_set& fds) const
{
    return FD_ISSET(this->listenSocket, &fds);
}

