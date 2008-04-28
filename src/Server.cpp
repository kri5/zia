#ifndef WIN32
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/resource.h>
#include <poll.h>
#endif

#include "zia.h"
#include "Server.h"
#include "Workflow/Worker.h"
#include "ZException.hpp"
#include "RootConfig.hpp"
#include "Modules/ModuleManager.h"
#include "Sockets/ClientSocket.h"

#include "MemoryManager.hpp"

Server::Server(const std::map<const NetworkID*, std::vector<Vhost*> >& toBind, 
        Pool* pool) : _toBind(toBind), _pool(pool)
{
    std::map<const NetworkID*, std::vector<Vhost*> >::const_iterator		it = this->_toBind.begin();
    std::map<const NetworkID*, std::vector<Vhost*> >::const_iterator		end = this->_toBind.end();

    try
    {
        while (it != end)
        {
            MainSocket*		sock = new MainSocket(it->first, 16, it->second);
            _sockets.push_back(sock);
            ++it;
        }
    }
    catch (ZException<IMainSocket>& ex)
    {
        Logger::getInstance() << Logger::Error << "Socket error : " << ex.what() << Logger::Flush;
        throw ZException<Server>(INFO, Server::Error::Bind);
    }
#ifndef WIN32
    struct rlimit   l;
    getrlimit(RLIMIT_NOFILE, &l);
    _maxFd = (l.rlim_cur - 4) / 2;
#else
	_maxFd = FD_SETSIZE;
#endif
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
    //int                 DEBUG = 10;

    //Hooks : onServerStart
    ModuleManager::getInstance().call(zAPI::IModule::ServerEventHook, zAPI::IModule::onServerStartEvent, &zAPI::IServerEvent::onServerStart);
    Logger::getInstance() << Logger::Info << "All sockets initialized, starting main loop" << Logger::Flush;
    pfds = new struct pollfd[size];
    //while (DEBUG--)
    while (true)
    {
        memset(pfds, 0, sizeof(*pfds) * size);
        for (i = 0; i < size; ++i)
        {
            pfds[i].fd = this->_sockets[i]->getNativeSocket();
#ifndef WIN32
            pfds[i].events = (POLLIN | POLLOUT | POLLERR | POLLHUP);
#else
            pfds[i].events = (POLLRDNORM);
#endif
        }
        ret = poll(pfds, size, 1000);
        if (ret < 0)
        {
            std::cerr << "Error in poll Server::run" << std::endl;
            return ;
        }
        if (ret == 0)
        {
            continue ;
        }
        this->checkSockets(ret, pfds);
    }
    delete[]	pfds;
    //Hooks : onServerStop
    ModuleManager::getInstance().call(zAPI::IModule::ServerEventHook, zAPI::IModule::onServerStopEvent, &zAPI::IServerEvent::onServerStop);
}

void            Server::checkSockets(int nbSockets, const struct pollfd* pfds) const
{
    int         size = this->_sockets.size();
    int         i;

    for (i = 0; i < size && nbSockets > 0; ++i) //FIXME: if nbSockets is the number of sockets to read, maybe we should use it :p
    {
#ifndef WIN32
        if (pfds[i].revents & (POLLIN | POLLOUT | POLLERR | POLLHUP))
#else
        if (pfds[i].revents & (POLLRDNORM))
#endif
        {
            zAPI::IClientSocket*      clt = this->_sockets[i]->accept();
            if (clt)
            {
                //if (ClientSocket::countSockets() >= this->_maxFd)
                //{
                //    std::cout << "Max client reached, disconnecting" << std::endl;
                //    delete clt;
                //}
                //else
                //{
                    if (this->_pool->addTask(clt, &(this->_sockets[i]->getAssociatedVhosts())) == false)
                    {
                        //FIXME: check for memory leak (not deleting clt)
                        Logger::getInstance() << Logger::Info << "Can't add task : dropping clients" << Logger::Flush;
                    }
                    //else
                    //{
                    //    std::cout << "new connected client" << std::endl;
                    //}
                //}
            }
        }
    }
}

