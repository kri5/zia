#ifndef WIN32
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include "zia.h"
#include "Server.h"
#include "Workflow/Worker.h"
#include "ZException.hpp"
#include "RootConfig.hpp"

#include "MemoryManager.hpp"

Server::Server(const std::map<const NetworkID*, std::vector<const Vhost*> >& toBind, 
	Pool* pool) : _toBind(toBind), _pool(pool)
{
    std::map<const NetworkID*, std::vector<const Vhost*> >::const_iterator		it = this->_toBind.begin();
    std::map<const NetworkID*, std::vector<const Vhost*> >::const_iterator		end = this->_toBind.end();

    _maxFd = 0;
    try
    {
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
    catch (ZException<IMainSocket>& ex)
    {
	Logger::getInstance() << Logger::Error << "Socket error : " << ex.what() << Logger::Flush;
	throw ZException<Server>(INFO, Server::Error::Bind);
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
    RootConfig::deleteInstance();
}

void		Server::run()
{
    int					i;
    int					size = this->_sockets.size();
    struct pollfd*		pfds;
    int					ret;

    Logger::getInstance() << Logger::Info << "All sockets initialized, starting main loop" << Logger::Flush;
    pfds = new struct pollfd[size];
    while (true)
    {
	memset(pfds, 0, sizeof(*pfds) * size);
	for (i = 0; i < size; ++i)
	{
	    *(this->_sockets[i]) >> pfds[i];
	}
	ret = poll(pfds, size, 1000);
	if (ret < 0)
	{
	    throw ZException<Server>(INFO, Error::Select);
	}
	if (ret == 0)
	{
	    continue ;
	}
	this->checkSockets(ret, pfds);
    }
    delete[]	pfds;
}

void            Server::checkSockets(int nbSockets, const struct pollfd* pfds) const
{
    int         size = this->_sockets.size();
    int         i;

    for (i = 0; i < size && nbSockets > 0; ++i) //FIXME: if nbSockets is the number of sockets to read, maybe we should use it :p
    {
	if (this->_sockets[i]->isSet(pfds[i]))
	{
	    //Worker::create(*this->_sockets[i]->accept(), this->_sockets[i]->getAssociatedVhosts());
	    ClientSocket*  clt = this->_sockets[i]->accept();
	    if (clt)
	    {
            clt->setPollFlag(POLLIN | POLLERR | POLLHUP);
            if (this->_pool->addTask(clt, &(this->_sockets[i]->getAssociatedVhosts())) == false)
            {
                //FIXME: check for memory leak (not deleting clt)
                Logger::getInstance() << Logger::Info << "Can't add task : dropping clients" << Logger::Flush;
            }
            else
            {
                //std::cout << "new connected client" << std::endl;
            }
        }
	}
    }
}

