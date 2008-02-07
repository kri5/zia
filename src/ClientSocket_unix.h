#ifndef __CLIENT_SOCKET_UNIX_H__
#define __CLIENT_SOCKET_UNIX_H__

#include "zia.h"
#include "Socket_unix.h"
#include "IClientSocket.h"

/** A specialized socket, which will only handle clients, and not connections to server.
 *  For others method than ctor/dtor documentation, refer to IClientSocket.
 */
class ClientSocket : public Socket, public IClientSocket
{
    public:
		/// Will set the socket from the one returned by MainSocket::accept();
        ClientSocket(int acceptedSocket);
		/// Don't close the socket. (Done by ~Socket())
        virtual ~ClientSocket();
        virtual int	    send(const char *buf, int length) const;
        virtual int	    recv(char *buf, int length) const;
};

#endif //__CLIENT_SOCKET_UNIX_H__
