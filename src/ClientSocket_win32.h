#ifndef __CLIENT_SOCKET_WIN32_H__
#define __CLIENT_SOCKET_WIN32_H__

#include "zia.h"
#include "Socket_win32.h"
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
		~ClientSocket();
		int	recv(char *buf, int length) const;
		int send(const char *buf, int length) const;
	private:
};

#endif //__CLIENT_SOCKET_WIN32_H__

