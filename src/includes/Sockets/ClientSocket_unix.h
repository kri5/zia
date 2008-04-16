#ifndef __CLIENT_SOCKET_UNIX_H__
#define __CLIENT_SOCKET_UNIX_H__

#include "zia.h"
#include "API/IClientSocket.h"
#include "Mutex/Mutex.h"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>

#ifdef __FreeBSD__
 #define MSG_NOSIGNAL 0
#elif __NetBSD__
 #define MSG_NOSIGNAL 0
#endif

/** A specialized socket, which will only handle clients, and not connections to server.
 *  For others method than ctor/dtor documentation, refer to IClientSocket.
 */
class ClientSocket : public zAPI::IClientSocket
{
    public:
        bool            deleted;
		struct  Error
		{
			enum    Code
			{
				Unknown,
				Send,
				Recv,
				Disconnect
			};
			static const char* Msg[];
		};
		/// Will set the socket from the one returned by MainSocket::accept();
        ClientSocket(int acceptedSocket);
		/// Don't close the socket. (Done by ~Socket())
        virtual ~ClientSocket();
        virtual int	    send(const char *buf, int length) const;
        virtual int 	send(const std::string& buf, int length) const;
        virtual int	    recv(char *buf, int length) const;
        virtual void    close(bool shutdown);
        virtual bool    isClosed() const;
        virtual int	    getNativeSocket() const;

        static  int     countSockets();
        static const int SOCKET_ERROR = -1;
    private:
        static  int     _nbSockets;
        static  Mutex   _mutex;
        int             listenSocket;
        bool            _closed;
};

#endif //__CLIENT_SOCKET_UNIX_H__
