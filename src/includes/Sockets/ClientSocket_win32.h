#ifndef __CLIENT_SOCKET_WIN32_H__
#define __CLIENT_SOCKET_WIN32_H__

#include "zia.h"
#include "API/IClientSocket.h"
#include "Mutex/Mutex.h"

/** A specialized socket, which will only handle clients, and not connections to server.
 *  For others method than ctor/dtor documentation, refer to IClientSocket.
 */
class ClientSocket : public zAPI::IClientSocket
{
	public:
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
		ClientSocket(int acceptedSocket, const std::string&, int);
		/// Don't close the socket. (Done by ~Socket())
		~ClientSocket();
		virtual int			send(const char *buf, int length);
		virtual int			send(const std::string& buf, int length);
		virtual int			recv(char *buf, int length);
		virtual void		close(bool shutdown);
		virtual bool		isClosed() const;
		virtual SOCKET      getNativeSocket() const;
		virtual const		std::string&  getIP() const;
		virtual int			getPort() const;

		static int			countSockets();
	private:
		static int			_nbSockets;
		static Mutex		_mutex;
		int					listenSocket;
		bool				_closed;
		std::string			_address;
		int					_port;

};

#endif //__CLIENT_SOCKET_WIN32_H__

