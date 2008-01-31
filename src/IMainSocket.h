#ifndef IMAIN_SOCKET_H__
#define IMAIN_SOCKET_H__

class IMainSocket
{
public:
	/// Spawn a new socket for the new client.
	ClientSocket *accept();
	/// Will be user to receive a connection request from a new client.
	int	recv(char *buf, int length) const;
protected:
	/// Bind the socket on a specified host and port.
	void bind(std::string address, int port) const;
	/// Listen wait for incoming connections.
	void listen(int queue) const;
};

#endif

