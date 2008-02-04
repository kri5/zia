#ifndef __SSL_CLIENT_SOCKET_UNIX_H__
#define __SSL_CLIENT_SOCKET_UNIX_H__

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <iostream>
#include "zia.h"
#include "ClientSocket.h"
#include "Logger.hpp"

class SSLClientSocket : public ClientSocket
{
  public:
    SSLClientSocket(int acceptedSocket);
    ~SSLClientSocket();
    int     send(char *buf, int length) const;
    int     recv(char *buf, int length) const;
    void    close(bool shutdown) const;

  protected:
    SSL_CTX* ctx;
    SSL* ssl;

  private:
    void	logError() const;

};

#endif // __SSL_CLIENT_SOCKET_UNIX_H__
