#ifndef __MAINSOCKET_WIN32_H__
#define __MAINSOCKET_WIN32_H__

#include <string>
#include <vector>

#include "API/IClientSocket.h"
#include "IMainSocket.h"
#include "Network/Vhost.h"
#include "Network/NetworkID.h"

/// Specialised socket class, which will just handle connections to server, then spawn a ClientSocket
class MainSocket : public IMainSocket
{
public:
	// MainSocket constructor : will build a main server socket, listening on "adress":"port", with a "queue" client queue.
	MainSocket(const NetworkID*, int, const std::vector<Vhost*>&);
	~MainSocket();
	
    const std::vector<Vhost*>&   getAssociatedVhosts();

	/// Spawn a new socket for the new client.
	zAPI::IClientSocket			*accept();
	void						close(bool shutdown);
	bool						isClosed() const;
	SOCKET  					getNativeSocket() const;

private:
	int							listenSocket;
	/// Bind the socket on a specified host and port.
	void bind(const NetworkID*) const;
	/// Listen wait for incoming connections.
	void listen(int queue) const;

	

	const NetworkID*			_netId;
	const std::vector<Vhost*>&	_vhosts;
	WSADATA						_wsaData;
};

#endif //__MAINSOCKET_WIN32_H__

