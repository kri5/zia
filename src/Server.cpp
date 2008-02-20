#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "Server.h"
#include "ZException.hpp"

#include "MemoryManager.hpp"

Server::Server(const std::map<NetworkID*, std::vector<Vhost*> >& toBind) : _toBind(toBind)
{
	std::map<NetworkID*, std::vector<Vhost*> >::const_iterator		it = this->_toBind.begin();
	std::map<NetworkID*, std::vector<Vhost*> >::const_iterator		end = this->_toBind.end();

	_sockets.clear();
	_maxFd = 0;
	while (it != end)
	{
		MainSocket*		sock = new MainSocket(it->first, 16, it->second);
#ifndef WIN32
		_maxFd = sock->getSocketValue() > _maxFd ? sock->getSocketValue() : _maxFd;
#endif
		_sockets.push_back(sock);
		++it;
	}
}

Server::~Server()
{
	std::vector<MainSocket*>::iterator		it = this->_sockets.begin(); 
	std::vector<MainSocket*>::iterator		end = this->_sockets.end();

	while (it != end)
	{
		delete *it;
		++it;
	}
	this->_sockets.clear();
}

void		Server::run()
{
	std::vector<MainSocket*>::iterator		it; 
	std::vector<MainSocket*>::iterator		end = this->_sockets.end();
	fd_set									fds;
	struct timeval							tval;
	int										ret;

	Logger::getInstance() << Logger::Info << "All sockets initialized, starting main loop" << Logger::Flush;
	while (true)
	{
		tval.tv_sec = 1;
		tval.tv_usec = 0;
		FD_ZERO(&fds);
		for (it = this->_sockets.begin(); it != end; ++it)
		{
			*(*it) >> fds;
		}
		ret = select(this->_maxFd + 1, &fds, NULL, NULL, &tval);
		if (ret < 0)
			throw ZException<Server>(INFO, Error::Select);
		if (ret == 0)
		{
			continue ;
		}
		std::cout << "Data received !" << std::endl;
	}
}
