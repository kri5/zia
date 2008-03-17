#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#include "Sockets/Socket_unix.h"
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


const ISocket&  Socket::operator>>(struct pollfd& pfds) const
{
  pfds.fd = this->listenSocket;
  pfds.events = POLLIN | POLLOUT | POLLERR | POLLHUP;
  return *this;
}

int				Socket::getSocketValue() const
{
	return this->listenSocket;
}

bool            Socket::isSet(const struct pollfd& pfds) const
{
  if (pfds.revents & (POLLOUT | POLLIN | POLLERR | POLLHUP))
    return true;
  return false;
}
