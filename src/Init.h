#ifndef __INIT_H__
#define __INIT_H__

#include "zia.h"
#include "Logger.hpp"
#include <list>
#include <string>
#include "XMLDocument.h"
#include "XMLNode.h"
#include "Vhost.h"
#include <openssl/ssl.h>
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
		std::string		getConfigNodeValue(Xml::Node*);
		void			parseConfigNode(Xml::Node*, Config*);
		void			addVhost(Xml::Node*);
		Config*			_conf;
		std::list<Vhost*>	_vhosts;

        int     _argc;
        char**  _argv;  
};

#endif // __INIT_H__
