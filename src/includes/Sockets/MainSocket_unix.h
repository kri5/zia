#ifndef __MAINSOCKET_UNIX_H__
#define __MAINSOCKET_UNIX_H__

#include <string>
#include <vector>

#include "Sockets/ClientSocket_unix.h"
#include "Sockets/IMainSocket.h"
#include "Network/NetworkID.h"
#include "Network/Vhost.h"

class MainSocket : public IMainSocket
{
	public:
		// MainSocket constructor : will build a main server socket, listening on "adress":"port", with a "queue" client queue.
		MainSocket(const NetworkID*, int, const std::vector<Vhost*>&);
		~MainSocket();
        const std::vector<Vhost*>&   getAssociatedVhosts();

        zAPI::IClientSocket *accept();
        void    close(bool shutdown);
        bool    isClosed() const;
        int	    getNativeSocket() const;
	private:
        static const    int SOCKET_ERROR = -1;
        int             listenSocket;
        bool            _closed;

		void    bind(const NetworkID*) const;
		void    listen(int queue) const;

		/// The networkID this socket's listening to
		const NetworkID*			        _netId;
		const std::vector<Vhost*>&	_vhosts;
};

#endif //__MAINSOCKET_UNIX_H__
