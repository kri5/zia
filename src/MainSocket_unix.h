#ifndef __MAINSOCKET_UNIX_H__
#define __MAINSOCKET_UNIX_H__

#include <string>
#include "Socket_unix.h"
#include "ClientSocket_unix.h"
#include "IMainSocket.h"
#include "NetworkID.h"

class MainSocket : public Socket, public IMainSocket
{
	public:
		// MainSocket constructor : will build a main server socket, listening on "adress":"port", with a "queue" client queue.
		MainSocket(NetworkID*, int);
		~MainSocket();

		ClientSocket *accept();
	private:
		void bind(NetworkID*) const;
		void listen(int queue) const;

		/// The networkID this socket's listening to
		NetworkID*			_netId;
};

#endif //__MAINSOCKET_UNIX_H__
