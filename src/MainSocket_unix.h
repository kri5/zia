#ifndef __MAINSOCKET_UNIX_H__
#define __MAINSOCKET_UNIX_H__

#include <string>
#include "Socket_unix.h"
#include "ClientSocket_unix.h"
#include "IMainSocket.h"

class MainSocket : public Socket, public IMainSocket
{
public:
	// MainSocket constructor : will build a main server socket, listening on "adress":"port", with a "queue" client queue.
	MainSocket(int queue, std::string adress, int port);
	~MainSocket();
	
	ClientSocket *accept();
	int	recv(char *buf, int length) const;
private:
	void bind(std::string address, int port) const;
	void listen(int queue) const;

	/// The port the server is listening to.
	int				_port;
	/// The address the server will be listening.
	std::string		_adress;
};

#endif //__MAINSOCKET_UNIX_H__
