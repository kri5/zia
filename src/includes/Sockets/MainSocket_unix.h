#ifndef __MAINSOCKET_UNIX_H__
#define __MAINSOCKET_UNIX_H__

#include <string>
#include <vector>

#include "Sockets/Socket_unix.h"
#include "Sockets/ClientSocket_unix.h"
#include "Sockets/SSLClientSocket.h"
#include "Sockets/IMainSocket.h"
#include "Network/NetworkID.h"
#include "Network/Vhost.h"

class MainSocket : public Socket, public IMainSocket
{
	public:
		// MainSocket constructor : will build a main server socket, listening on "adress":"port", with a "queue" client queue.
		MainSocket(const NetworkID*, int, const std::vector<const Vhost*>&);
		~MainSocket();
        const std::vector<const Vhost*>&   getAssociatedVhosts();

		ClientSocket *accept();
	private:
		void    bind(const NetworkID*) const;
		void    listen(int queue) const;

		/// The networkID this socket's listening to
		const NetworkID*			        _netId;
		const std::vector<const Vhost*>&	_vhosts;
};

#endif //__MAINSOCKET_UNIX_H__
