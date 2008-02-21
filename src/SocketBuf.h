#ifndef __SOCKETBUF_H__
#define __SOCKETBUF_H__

#include <streambuf>
#include <iostream>

#include "Socket.h"

#ifndef WIN32
 #include <sys/ioctl.h>
#endif

#ifndef TIOCINQ
 #define TIOCINQ FIONREAD
#endif

using namespace std;

class     SocketBuf : public streambuf
{
    public:
        SocketBuf(int socket);
        virtual ~SocketBuf();

    protected:
        int       overflow(int x);
        int       underflow();
        int       sync();

    private:
        int       _socket;
        char      _inBuffer[1024];
        char      _outBuffer[1024];
};


class     siostream: public iostream
{
    public:
        siostream(int socket) : _buf(socket), iostream(&_buf) { }

    private:
        SocketBuf _buf;
};

#endif // __SOCKETBUF_H__
