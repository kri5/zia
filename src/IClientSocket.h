#ifndef __ICLIENT_SOCKET_H__
#define __ICLIENT_SOCKET_H__

#include <string>

/// Interface for ClientSocket classes, to ensure multi-platform compactibility
class IClientSocket
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
		virtual ~IClientSocket(){}
		/// Will receive data from client.
		virtual int	recv(char *buf, int length) const = 0;
		/// Will send datas to client.
		virtual int send(const char *buf, int length = -1) const = 0;
		/// Overloaded method that take a string
		virtual int send(const std::string& buf, int length) const = 0;
};

#endif //__ICLIENT_SOCKET_H__

