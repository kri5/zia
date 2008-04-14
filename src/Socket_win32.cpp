#include "Sockets/Socket_win32.h"

#include "ZException.hpp"
#include "MemoryManager.hpp"

Socket::Socket() : _pollFlag(POLLRDNORM | POLLWRNORM)
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
	if (this->_closed == false)
	{
		if (shutdown)
			::shutdown(listenSocket, SD_BOTH);
		closesocket(listenSocket);
		this->_closed = true;
	}
}

const ISocket&	Socket::operator>>(struct pollfd& pdfs) const
{
	pdfs.fd = this->listenSocket;
	pdfs.events = this->_pollFlag;
	return *this;
}

bool            Socket::isSet(const struct pollfd& pdfs) const
{
	if (pdfs.revents & this->_pollFlag)
		return true;
	return false;
}

void			Socket::setPollFlag(int pollFlag)
{
	_pollFlag = pollFlag;
}

bool			Socket::isClosed() const
{
	return this->_closed;
}
