#ifndef __INIT_H__
#define __INIT_H__

#include "zia.h"

#include <list>
#include <string>
#include <openssl/ssl.h>
#include <tinyxml2.h>

#include "Logger.hpp"
#include "Network/Vhost.h"
#include "Config.h"
#include "Workflow/Pool.h"

class Init
{
	public:
		Init(int argc, char **argv);
		~Init();

		int     start();
		const std::map<const NetworkID*, std::vector<Vhost*> >&	getBindList() const;
        Pool*                                                           getPool() const;
	private:
		void    		readCommandLine();
		bool			readConfiguration(const std::string fileName = "zia.conf", Config* cfg = NULL);
        void            initModules();
		void    		initSSL();
		void    		initSockets();
		void    		initThreads();
		void			includeConfigFile(std::string fileName, Config* cfg);

		void			parseConfigNode(tinyxml2::XMLNode*, Config*);
		void			addVhost(tinyxml2::XMLElement&);
		void			addMimeType(tinyxml2::XMLElement& node);
		void			addWildcardVhosts();
		void			addNonWildcardVhosts();
        bool            checkConfig() const;

		Config*													_conf;
		std::list<Vhost*>										_vhosts;
		std::map<const NetworkID*, std::vector<Vhost*> >	_bindList;
        Pool*                                                   _pool;

		int     _argc;
		char**  _argv;  
        const static int                                        _defaultPoolThreadNumber = 10;
        const static int                                        _defaultPoolTaskSize = 300;
};

#endif // __INIT_H__
