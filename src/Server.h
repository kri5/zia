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
				Select
			};
			static const char* Msg[];
		};
		Server(const std::map<NetworkID*, std::vector<Vhost*> >&);
		~Server();
		void			run();

	private:
		int													_maxFd;
		const std::map<NetworkID*, std::vector<Vhost*> >&	_toBind;
		std::vector<MainSocket*>							_sockets;
};

#endif //SERVER_H__

