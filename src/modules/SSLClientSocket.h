#ifndef __SSL_CLIENT_SOCKET_UNIX_H__
#define __SSL_CLIENT_SOCKET_UNIX_H__

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <iostream>
#include "API/IClientSocket.h"

class SSLClientSocket : public zAPI::IClientSocket
{
  public:
    SSLClientSocket(int acceptedSocket);
    virtual ~SSLClientSocket();
    virtual int     send(const char *buf, int length) const;
    virtual int     send(const std::string& buf, int length) const;
    virtual int     recv(char *buf, int length) const;
    virtual void    close(bool shutdown);
    virtual bool    isClosed() const;
    virtual int     getNativeSocket() const;

  protected:
    SSL_CTX*        ctx;
    SSL*            ssl;
    int             listenSocket;
    bool            _closed;

  private:
    void	logError() const;

};

#endif // __SSL_CLIENT_SOCKET_UNIX_H__
