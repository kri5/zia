#include <iostream>
#include <cstring>

#include "Init.h"
#include "Network/NetworkID.h"
#include "RootConfig.hpp"
#include "Modules/ModuleManager.h"

#include "MemoryManager.hpp"

Init::Init(int argc, char **argv) : _argc(argc), _argv(argv)
{
	_conf = new Config();
	_pool = NULL;
}

/// Will destroy logger, since this function is supposed to be the last called in this server.
Init::~Init()
{
	std::list<Vhost*>::iterator		it = this->_vhosts.begin();
	std::list<Vhost*>::iterator		end = this->_vhosts.end();
	while (it != end)
	{
		delete *it;
		++it;
	}
	this->_vhosts.clear();
	delete this->_conf;
	if (this->_pool)
		delete this->_pool;
}

/// Run the sequential initialization
int         Init::start()
{
	this->readCommandLine();
	if (this->readConfiguration() == false)
	{
		Logger::getInstance() << Logger::Warning << "Can't load configuration file" << Logger::Flush;
		return EXIT_FAILURE;
	}
	if (this->checkConfig() == false)
		return EXIT_FAILURE;
	this->_conf->dump();
	std::cout << "Conf Dumped" << std::endl;
	this->initModules();
	std::cout << "Modules Initialized" << std::endl;
	this->initSockets();
	std::cout << "Sockets Initialized" << std::endl;
	this->initThreads();
	std::cout << "Threads Initialized" << std::endl;

	return EXIT_SUCCESS; // EXIT_FAILURE on error
}

/// Read and process arguments from the command line
void        Init::readCommandLine()
{

}

void		Init::addVhost(tinyxml2::XMLElement& node)
{
	std::string		addr;
	std::string		port;

	addr = node.Attribute("address");
	port = node.Attribute("port");
	if (addr == "" || port == "")
	{
		Logger::getInstance() << Logger::Warning << "Can't have VirtualHost without both address and port" << Logger::Flush;
		return ;
	}
	Vhost*	v = new Vhost(NetworkID::factory(addr, port), this->_conf);
	this->parseConfigNode(static_cast<tinyxml2::XMLNode*>(&node), static_cast<Config*>(v));
    if (v->isSet("ServerName") == false)
    {
        Logger::getInstance() << Logger::Warning << "This vhost (" << addr << ':' << port << ") doesn't have a ServerName." << Zia::Newline
            << "It will not be used" << Logger::Flush;
        delete v;
    }
    else if (v->isSet("DocumentRoot") == false)
    {
        Logger::getInstance() << Logger::Warning << "This vhost (" << addr << ':' << port << ") doesn't have a DocumentRoot." << Zia::Newline
            << "It will not be used" << Logger::Flush;
        delete v;
    }
    else //insertion dans la liste des vhosts.
    {
        this->_vhosts.push_back(v);
    }
}

void		Init::addMimeType(tinyxml2::XMLElement& node)
{
	std::string fileExts = node.Attribute("file");

	if (fileExts == "")
		return ;
	size_t	begin = 0;
	size_t	end;
	while ((end = fileExts.find(';', begin)) != std::string::npos)
	{
		this->_conf->addMimeType(fileExts.substr(begin, end - begin), node.GetText());
		begin = end + 1;
	}
    this->_conf->addMimeType(fileExts.substr(begin, end - begin), node.GetText());
}

void		Init::includeConfigFile(std::string fileName, Config* cfg)
{
	if (this->readConfiguration(fileName, cfg) != true)
		Logger::getInstance() << Logger::Warning << "Can't include file " << fileName << Zia::Newline << Logger::Flush;
}

void		Init::parseConfigNode(tinyxml2::XMLNode* node, Config* cfg)
{
	for (tinyxml2::XMLNode* it = node->FirstChild(); it; it = it->NextSibling())
	{
		if (strcmp(it->Value(),"VirtualHost") == 0)
			this->addVhost(*it->ToElement());
		else if (strcmp(it->Value(),"Include") == 0)
			this->includeConfigFile(it->ToElement()->GetText(), cfg);
		else if (strcmp(it->Value(),"type") == 0)
			this->addMimeType(*it->ToElement()); 
		// Generic parameters
		else
		{
			cfg->setParam(it->Value(), it->ToElement()->GetText());
			Logger::getInstance() << Logger::Info << Logger::NoStdOut << "Adding " << it->Value() << " = " << *(this->_conf->getParam(it->Value())) << " to conf" << Zia::Newline << Logger::End;
		}
	}
	Logger::getInstance() << Logger::NoStdOut << Logger::Flush;
}

/// Read the XML configuration
bool	Init::readConfiguration(const std::string fileName, Config* cfg)
{
	if (!cfg)
		cfg = this->_conf;
	tinyxml2::XMLDocument	doc;
	std::cout << fileName << std::endl;
	if (doc.LoadFile(fileName.c_str()) != 0)
		return false;

	tinyxml2::XMLNode* node = doc.FirstChild();
	this->parseConfigNode(node, cfg);
	return true;
}

void        Init::initModules()
{
    int nb = this->_conf->getIntParam("PoolTaskSize", Init::_defaultPoolTaskSize);

    ModuleManager::getInstance().init(nb);
    ModuleManager::getInstance().scanModuleDir();
    //ModuleManager::getInstance().initProcessContent();
}

void		Init::addWildcardVhosts()
{
	std::list<Vhost*>::iterator		it = this->_vhosts.begin();
	std::list<Vhost*>::iterator		end = this->_vhosts.end();
	std::map<const NetworkID*, std::vector<Vhost*> >::iterator	itNet;
	std::map<const NetworkID*, std::vector<Vhost*> >::iterator	endNet;

	//Will just create the element. This is soooo ugly :(
	this->_bindList[NetworkID::factory("*", *(this->_conf->getParam("Listen")))];
	while (it != end)
	{
		if ((*it)->getAddress() == "*")
		{
			itNet = this->_bindList.begin();
			endNet = this->_bindList.end();
			bool		found = false;
			while (itNet != endNet)
			{
				if (itNet->first->getPort() == (*it)->getPort())
				{
					found = true;
					itNet->second.push_back(*it);
				}
				++itNet;
			}
			if (found == false)
				this->_bindList[&((*it)->getNetworkID())].push_back(*it);
		}
		++it;
	}
}

void		Init::addNonWildcardVhosts()
{
	std::list<Vhost*>::iterator		it = this->_vhosts.begin();
	std::list<Vhost*>::iterator		end = this->_vhosts.end();
	std::map<const NetworkID*, std::vector<Vhost*> >::iterator	itNet;
	std::map<const NetworkID*, std::vector<Vhost*> >::iterator	endNet;

	while (it != end)
	{
		if (!((*it)->getAddress() == "*"))
		{
			bool	found = false;

			itNet = this->_bindList.begin();
			endNet = this->_bindList.end();
			while (itNet != endNet)
			{
				if (itNet->first->getAddress() == "*" && itNet->first->getPort() == (*it)->getPort())
				{
					found = true;
					std::cout << found << std::endl;
					itNet->second.push_back(*it);
				}
				++itNet;
			}
			std::cout << found << std::endl;
			if (found == false)
				this->_bindList[&((*it)->getNetworkID())].push_back(*it);
		}
		++it;
	}
}

/// Start the server sockets
void        Init::initSockets()
{
	this->addWildcardVhosts();
	this->addNonWildcardVhosts();
}

/// Spawn the threads
void        Init::initThreads()
{
    int threadNb = this->_conf->getIntParam("PoolThreadNumber", Init::_defaultPoolThreadNumber);
    int taskSize = this->_conf->getIntParam("PoolTaskSize", Init::_defaultPoolTaskSize);

    _pool = new Pool(threadNb, taskSize);
    _pool->init();
}

const std::map<const NetworkID*, std::vector<Vhost*> >&	Init::getBindList() const
{
	return this->_bindList;
}

bool    Init::checkConfig() const
{
    //Feel free to add some :)
    if (this->_conf->isSet("Listen") == false)
    {
        Logger::getInstance() << Logger::Error << "You must specify a port to listen to. Shuting down." << Logger::Flush;
        return false;
    }
    else if (this->_conf->getIntParam("Timeout", false) == false)
    {
        Logger::getInstance() << Logger::Error << "You must specify a port to timeout delay. Shuting down." << Logger::Flush;
        return false;
    }
	RootConfig::getInstance().setConfig(this->_conf);
    return true;
}

Pool*   Init::getPool() const
{
    return this->_pool;
}
