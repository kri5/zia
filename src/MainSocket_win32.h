#ifndef __MAINSOCKET_WIN32_H__
#define __MAINSOCKET_WIN32_H__

#include <string>
#include "Socket_win32.h"
#include "ClientSocket_win32.h"

class MainSocket : public Socket
{
public:
	// MainSocket constructor : will build a main server socket, listening on "adress":"port", with a "queue" client queue.
	MainSocket(int queue, std::string adress, int port);
	
	/// Spawn a new socket for the new client.
	ClientSocket *accept();
private:
	/// Bind the socket on a specified host and port.
	void bind(std::string address, int port) const;
	/// Listen wait for incoming connections.
	void listen(int queue) const;

	

	/// The port the server is listening to.
	int				_port;
	/// The address the server will be listening.
	std::string		_adress;
};

#endif //__MAINSOCKET_WIN32_H__

