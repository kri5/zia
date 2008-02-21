#include "SocketBuf.h"

using namespace std;

SocketBuf::SocketBuf(int socket) : _socket(socket)
{
    setg(_inBuffer, _inBuffer, _inBuffer);
    setp(_outBuffer, _outBuffer + 1024);
}

SocketBuf::~SocketBuf() { }

int           SocketBuf::overflow(int x)
{
    cout << "SocketBuf::overflow(): " << x << endl;
    return x;
}

int           SocketBuf::underflow()
{
    cout << "SocketBuf::underflow()" << endl;

    if (gptr() < egptr()) return *(unsigned char*)gptr();

    int len;

    if (ioctl(_socket, TIOCINQ, &len) < 0)
    {
        cout << "SocketBuf error" << endl;
        len = 1;
    }

    if (len == 0)
        len = 1;

    int read = recv(_socket, &_inBuffer, len, 0);
    if (read > 0)
    {
        setg(_inBuffer, _inBuffer, _inBuffer + read);
    }
    else
    {
        return EOF;
    }

    return *(unsigned char*)gptr();
}

int             SocketBuf::sync()
{
    cout << "SocketBuf::sync()" << endl;

    if (pbase() == pptr())
        return 0;

    int len = pptr() - pbase();
    int rc = send(_socket, pbase(), len, 0);
    if (rc < 0)
        return rc;

    setp(_outBuffer, _outBuffer + 1024);
    return 0;
}




