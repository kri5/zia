#ifndef __MAINSOCKET_WIN32_H__
#define __MAINSOCKET_WIN32_H__

#include <string>
#include <vector>

#include "Socket_win32.h"
#include "ClientSocket_win32.h"
#include "IMainSocket.h"
#include "Vhost.h"
#include "NetworkID.h"

/// Specialised socket class, which will just handle connections to server, then spawn a ClientSocket
class MainSocket : public Socket, public IMainSocket
{
public:
	// MainSocket constructor : will build a main server socket, listening on "adress":"port", with a "queue" client queue.
	MainSocket(const NetworkID*, int, const std::vector<const Vhost*>&);
	
    const std::vector<const Vhost*>&   getAssociatedVhosts();

	/// Spawn a new socket for the new client.
	ClientSocket *accept();
private:
	/// Bind the socket on a specified host and port.
	void bind(const NetworkID*) const;
	/// Listen wait for incoming connections.
	void listen(int queue) const;

	

	const NetworkID*			_netId;
	const std::vector<const Vhost*>&	_vhosts;
};

#endif //__MAINSOCKET_WIN32_H__

