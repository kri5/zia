#ifndef __MAINSOCKET_UNIX_H__
#define __MAINSOCKET_UNIX_H__

#include <string>
#include "Socket_unix.h"
#include "ClientSocket_unix.h"

class MainSocket : public Socket
{
public:
	// MainSocket constructor : will build a main server socket, listening on "adress":"port", with a "queue" client queue.
	MainSocket(int queue, std::string adress, int port);
	~MainSocket();
	
	/// Spawn a new socket for the new client.
	ClientSocket *accept();
	/// Will be user to receive a connection request from a new client.
	int	recv(char *buf, int length) const;
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

#endif //__MAINSOCKET_UNIX_H__
