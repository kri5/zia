#include "SocketBuf.h"

using namespace std;

SocketBuf::SocketBuf(int s) : socket(s)
{
    setg(readBuffer, readBuffer, readBuffer);
    setp(writeBuffer, writeBuffer + sizeof(writeBuffer) - 1);
}

SocketBuf::~SocketBuf()
{
    sync();
    // Should we close the buffer here ?
}

int           SocketBuf::overflow(int x)
{
    Logger::getInstance() << Logger::Info << "SocketBuf::overflow(): " << x << Logger::Flush; 
    size_t len = size_t(pptr() - pbase());
    if (x != traits_type::eof())
    {
        *pptr() = x;
        ++len;
    }

    if (len > 0)
    {
        const void* data = pbase();
        int res = send(socket, (char*)data, len, 0);
        if (res <= 0)
            return traits_type::eof();
        setp(writeBuffer, writeBuffer + sizeof(writeBuffer) - 1);
    }

    return 0;
}

int           SocketBuf::underflow()
{
    Logger::getInstance() << Logger::Info << "SocketBuf::underflow()" << Logger::Flush; 
    int res = recv(socket, readBuffer, sizeof(readBuffer), 0);
    if (res < 0)
        return traits_type::eof();
    setg(readBuffer, readBuffer, readBuffer + res);
    return readBuffer[0];
}

int             SocketBuf::sync()
{
    Logger::getInstance() << Logger::Info << "SocketBuf::sync()" << Logger::Flush; 
    overflow(traits_type::eof());
    return 0;
}




