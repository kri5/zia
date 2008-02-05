#ifndef __CLIENT_SOCKET_UNIX_H__
#define __CLIENT_SOCKET_UNIX_H__

#include "zia.h"
#include "Socket_unix.h"
#include "IClientSocket.h"

class ClientSocket : public Socket, public IClientSocket
{
    public:
        ClientSocket(int acceptedSocket);
        virtual ~ClientSocket();
        virtual int	    send(const char *buf, int length) const;
        virtual int	    recv(char *buf, int length) const;
};

#endif //__CLIENT_SOCKET_UNIX_H__
