#include "Pool.h"
#include "Logger.hpp"

//#include "MemoryManager.hpp"

Pool::Manager*    Pool::Manager::create(Pool* pool)
{
    Manager* m = new Manager(pool);
    m->run();
    return m;
}

Pool::Manager::Manager(Pool* pool) : _pool(pool)
{
}

void    Pool::Manager::code()
{
    while (this->_running)
    {
        if (this->_pool->empty())
        {
            //Logger::getInstance() << Logger::PrintStdOut << Logger::Info << "No more task, Manager's going to bed" << Logger::Flush;
            this->checkSleep(true);
        }
        else
        {
            //si on arrive ici, c'est qu'aucun thread n'est actuellement libre, ou il sont tous endormis.
            //reste-t-il des threads libre (il seraient alors tous endormis)
            //Logger::getInstance() << Logger::Info << "nbTread " << this->_pool->getFreeThreadsNbr() << Logger::Flush;
            //Logger::getInstance() << Logger::Info << "Nb Task " << this->_pool->getTaskNbr() << Logger::Flush;
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
    }
}
