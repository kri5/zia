#ifndef __ICLIENT_SOCKET_H__
#define __ICLIENT_SOCKET_H__

/// Interface for ClientSocket classes, to ensure multi-platform compactibility
class IClientSocket
{
public:
	/// Will receive data from client.
	virtual int	recv(char *buf, int length) const = 0;
	/// Will send datas to client.
	virtual int send(const char *buf, int length) const = 0;
};

#endif //__ICLIENT_SOCKET_H__

