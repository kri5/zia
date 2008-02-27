#ifndef SERVER_H__
# define SERVER_H__

#include <map>
#include <vector>

#include "zia.h"
#include "NetworkID.h"
#include "MainSocket.h"
#include "Vhost.h"

class	Server
{
	public:
		struct	Error
		{
			enum	Code
			{
				Unknown,
                Bind,
				Select
			};
			static const char* Msg[];
		};
		Server(const std::map<const NetworkID*, std::vector<const Vhost*> >&);
		~Server();
		void			run();

	private:
		int																_maxFd;
		const std::map<const NetworkID*, std::vector<const Vhost*> >&	_toBind;
		std::vector<MainSocket*>										_sockets;

        void    checkSockets(int nbSockets, const fd_set& fds) const;
};

#endif //SERVER_H__

