#ifndef __SSL_CLIENT_SOCKET_UNIX_H__
#define __SSL_CLIENT_SOCKET_UNIX_H__

#include "zia.h"
#include "ClientSocket_unix.h"


class SSLClientSocket : public ClientSocket
{
    public:
        SSLClientSocket(int acceptedSocket);
        ~SSLClientSocket();
        int     send(char *buf, int length) const;
        int     recv(char *buf, int length) const;
   protected:
        SSL_CTX* ctx;
        SSL* ssl;
};

#endif // __SSL_CLIENT_SOCKET_UNIX_H__
