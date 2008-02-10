#ifndef __INIT_H__
#define __INIT_H__

#include "zia.h"

#include <list>
#include <string>
#include <openssl/ssl.h>
#include <ticpp.h>

#include "Logger.hpp"
#include "Vhost.h"
#include "Config.h"

class Init
{
    public:
        Init(int argc, char **argv);
        ~Init();

        int     run();
        void    readCommandLine();
        void    readConfiguration();
        void    initSSL();
        void    initSockets();
        void    initThreads();

    private:
		void			parseConfigNode(ticpp::Node*, Config*);
		void			addVhost(const ticpp::Element&);
		
		Config*				_conf;
		std::list<Vhost*>	_vhosts;
		std::map<Address*, std::map<Port*, std::vector<Vhost*> > >	_binds;

        int     _argc;
        char**  _argv;  
};

#endif // __INIT_H__
