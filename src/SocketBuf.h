#ifndef __SOCKETBUF_H__
#define __SOCKETBUF_H__

#include <streambuf>
#include <iostream>

#include "Logger.hpp"
#include "Socket.h"

#ifndef WIN32
 #include <sys/ioctl.h>
#endif

#ifndef TIOCINQ
 #define TIOCINQ FIONREAD
#endif

class     SocketBuf : public std::streambuf
{
    public:
        SocketBuf(int socket);
        virtual ~SocketBuf();

    protected:
        int       overflow(int x);
        int       underflow();
        int       sync();

    private:
        int       socket;
        char      writeBuffer[1024];
        char      readBuffer[1024];
};


class     siostream: public std::iostream
{
    public:
        siostream(int socket) : std::iostream(&_buf), _buf(socket) { }

    private:
        SocketBuf _buf;
};

#endif // __SOCKETBUF_H__
