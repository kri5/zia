#ifndef __SSL_CLIENT_SOCKET_UNIX_H__
#define __SSL_CLIENT_SOCKET_UNIX_H__

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <iostream>
#include "API/IClientSocket.h"

class SSLClientSocket : public zAPI::IClientSocket
{
  public:
    SSLClientSocket(int acceptedSocket, const std::string& address, int port);
    virtual ~SSLClientSocket();
    virtual int                 send(const char *buf, int length);
    virtual int                 send(const std::string& buf, int length);
    virtual int                 recv(char *buf, int length);
    virtual void                close(bool niceClosing);
    virtual bool                isClosed() const;
    virtual int                 getNativeSocket() const;
    virtual const std::string&  getIP() const;
    virtual int                 getPort() const;

  protected:
    SSL_CTX*                    ctx;
    SSL*                        ssl;
    int                         listenSocket;
    bool                        _closed;
    const std::string&          _address;
    int                         _port;

  private:
    void	logError() const;

};

#endif // __SSL_CLIENT_SOCKET_UNIX_H__
