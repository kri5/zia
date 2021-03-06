#include <errno.h> //FIXME: check win32 compactibility
#include <cstring>
#ifndef WIN32
# include <poll.h>
#endif

#include "Workflow/Pool.h"
#include "Time/Time.h"
#include "Logger.hpp"
#include "Modules/ModuleManager.h"
#include "ZException.hpp"

#include "MemoryManager.hpp"

Pool::Manager*    Pool::Manager::create(Pool* pool)
{
    Manager* m = new Manager(pool);
    m->run();
    return m;
}

Pool::Manager::Manager(Pool* pool) : _pool(pool)
{
    _timer = new Time();
}

Pool::Manager::~Manager()
{
    delete this->_timer;
}

void    Pool::Manager::initKeepAlivePoll() //warning : high contendence.
{
    std::list<KeepAliveClient>::iterator    it = this->_keepAlive.begin();
    std::list<KeepAliveClient>::iterator    ite = this->_keepAlive.end();
    size_t                                  size = this->_keepAlive.size();
    int                                     i = 0;

    this->_fds = new struct pollfd[size];
    memset(this->_fds, 0, sizeof(*(this->_fds)) * size);
    while (it != ite)
    {
        if ((*it).timer->elapsed((*it).timeout)) //FIXME: essayer de faire ca toutes les secondes uniquement.
        {
            delete (*it).clt;
            delete (*it).timer;
            it = this->_keepAlive.erase(it);
        }
        else
        {
            this->_fds[i].fd = ((*it).clt)->getNativeSocket();
#ifndef WIN32
            this->_fds[i].events = (POLLIN | POLLERR | POLLHUP);
#else
            this->_fds[i].events = (POLLRDNORM);
#endif
            ++it;
        }
        ++i;
    }
}

void    Pool::Manager::checkKeepAlive()
{
    int                                 i = 0;
    int                                 ret;

    ret = poll(this->_fds, this->_keepAlive.size(), 1);
    if (ret < 0)
	{
#ifndef WIN32    
		throw ZException<Pool::Manager>(INFO, Error::Poll, strerror(errno));
#else
		std::cout << strerror(WSAGetLastError()) << " " << WSAGetLastError() << std::endl;
		throw ZException<Pool::Manager>(INFO, Error::Poll, strerror(WSAGetLastError()));
#endif
	}
	if (ret > 0)
    {
        std::list<KeepAliveClient>::iterator  it = this->_keepAlive.begin();
        std::list<KeepAliveClient>::iterator  ite = this->_keepAlive.end();
        while (it != ite)
        {
#ifndef WIN32
            if (this->_fds[i].revents & (POLLIN | POLLERR | POLLHUP))
#else
                if (this->_fds[i].revents & (POLLRDNORM))
#endif
            {
                this->_pool->addTask((*it).clt, (*it).vhosts);
                delete (*it).timer;
                it = this->_keepAlive.erase(it);
            }
            else
                ++it;
            ++i;
        }
    }
    delete[] this->_fds;
}

void    Pool::Manager::code()
{
    while (this->_running)
    {
        if (this->_pool->empty())
        {
            //Logger::getInstance() << Logger::PrintStdOut << Logger::Info << "No more task, Manager's going to bed" << Logger::Flush;
            //this->checkSleep(true);
        }
        else
        {
            //si on arrive ici, c'est qu'aucun thread n'est actuellement libre, ou il sont tous endormis.
            //reste-t-il des threads libre (il seraient alors tous endormis)
            if (this->_pool->getFreeThreadsNbr() > 0)
            {
                //  oui : on pop le premier thread libre, il se chargera de trouver la task.
                //std::cout << "Poping one thread to handle request" << std::endl;
                this->_pool->popFreeThread()->awake();
            }
            else
            {
                //std::cout << "No more free thread, guess you'll have to wait" << std::endl;
                //  non : a voir pour un algo de recreation de thread.
            }
        }
		this->_pool->flushKeepAlive(this->_keepAlive);
#ifdef WIN32 //because Windows can't poll on a empty fds set...
		if (this->_keepAlive.size() > 0)
		{
#endif
			this->initKeepAlivePoll();
			this->checkKeepAlive();
#ifdef WIN32
        }
		Sleep(1);
#endif
        if (this->_timer->elapsed(3))
        {
            ModuleManager::getInstance().scanModuleDir();
            this->_timer->init();
        }
    }
}
