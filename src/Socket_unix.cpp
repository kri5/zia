#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#include "Sockets/Socket_unix.h"
#include "Logger.hpp"

#include "MemoryManager.hpp"

Socket::Socket() : _closed(false), _pollFlag(POLLIN | POLLOUT | POLLERR | POLLHUP)
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

bool    Socket::isClosed() const
{
    return this->_closed;
}

int	Socket::getNativeSocket() const
{
    return this->listenSocket;
}


const ISocket&  Socket::operator>>(struct pollfd& pfds) const
{
    pfds.fd = this->listenSocket;
    pfds.events = this->_pollFlag;
    return *this;
}

void            Socket::setPollFlag(int mask)
{
    this->_pollFlag = mask;
}

int				Socket::getSocketValue() const
{
    return this->listenSocket;
}

bool            Socket::isSet(const struct pollfd& pfds) const
{
    if (pfds.revents & this->_pollFlag)
        return true;
    return false;
}
