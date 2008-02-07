#ifndef IMAIN_SOCKET_H__
#define IMAIN_SOCKET_H__

/// Interface for multi platform MainSocket compactibility.
class IMainSocket
{
public:
	/// Spawn a new socket for the new client.
	virtual ClientSocket *accept() = 0;
	/// Will be user to receive a connection request from a new client.
	virtual int	recv(char *buf, int length) const = 0;
protected:
	/// Bind the socket on a specified host and port.
	virtual void bind(std::string address, int port) const = 0;
	/// Listen wait for incoming connections.
	virtual void listen(int queue) const = 0;
};

#endif

