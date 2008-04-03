#ifndef IMAIN_SOCKET_H__
#define IMAIN_SOCKET_H__

#include <string>

#include "zia.h"
#include "API/IClientSocket.h"
#include "Network/NetworkID.h"
/// Interface for multi platform MainSocket compactibility.
class IMainSocket
{
	public:
		struct	Error
		{
			enum Code
			{
				Unknown,
				Create,
				Bind,
				Listen,
				Accept
			};
			static const char* Msg[];
		};
		virtual ~IMainSocket(){}
		/// Spawn a new socket for the new client.
		virtual zAPI::IClientSocket *accept() = 0;
	protected:
		/// Bind the socket on a specified host and port.
		virtual void bind(const NetworkID*) const = 0;
		//virtual void bind(std::string address, int port) const = 0;
		/// Listen wait for incoming connections.
		virtual void listen(int queue) const = 0;
};

#endif

