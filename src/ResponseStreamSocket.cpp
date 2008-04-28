#ifndef _WIN32
 #include <errno.h>
 #include <arpa/inet.h>
 #include <netinet/in.h>
 #include <poll.h>
#endif // _WIN32

#include "Stream/ResponseStreamSocket.h"
#include "ZException.hpp"

ResponseStreamSocket::ResponseStreamSocket(zAPI::IClientSocket* sock, char* prev, size_t prevSize) :
    _socket(sock), _previouslyReadDatas(prev), _prevBufferSize(prevSize)
{
    _previouslyReadDatasPtr = _previouslyReadDatas;
}

ResponseStreamSocket::~ResponseStreamSocket()
{
    delete[] _previouslyReadDatas;
}

size_t      ResponseStreamSocket::read(char* buf, size_t size)
{
    if (this->_prevBufferSize > 0 && this->_previouslyReadDatasPtr != NULL)
    {
        if (size > this->_prevBufferSize)
            size = this->_prevBufferSize;
        strncpy(buf, this->_previouslyReadDatasPtr, size);
        this->_prevBufferSize -= size;
        this->_previouslyReadDatasPtr += size;
        if (this->_prevBufferSize == 0)
            this->_previouslyReadDatasPtr = NULL;
        else
            this->_previouslyReadDatasPtr += size;
        return size;
    }
    return this->_socket->recv(buf, size);
}

bool        ResponseStreamSocket::completed() const
{
    return (this->sockPoll() == 0 && this->_prevBufferSize == 0); 
}

bool        ResponseStreamSocket::good() const
{
    return (this->sockPoll() >= 0);
}

size_t        ResponseStreamSocket::getSize() const
{
    throw ZException<ResponseStreamSocket>(INFO, ResponseStreamSocket::Error::Size);
}

int             ResponseStreamSocket::sockPoll() const
{
    struct pollfd   fds;
    int             ret;

    memset(&fds, 0, sizeof(fds));
    fds.fd = this->_socket->getNativeSocket();
#ifndef WIN32
    fds.events = (POLLIN | POLLERR | POLLHUP);
#else
    fds.events = (POLLRDNORM);
#endif
    ret = ::poll(&fds, 1, 0);
    if (ret < 0)
	{
#ifndef WIN32     
		Logger::getInstance() << "Poll error: " << strerror(errno) << Logger::Flush;
#else
		Logger::getInstance() << "Poll error: " << WSAGetLastError() << Logger::Flush;
#endif
        return -1;
	}
    if (ret > 0)
        return 1;
    return 0;
}
