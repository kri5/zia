#include <iostream>
#include <string.h>

#include "Init.h"
#include "Network/NetworkID.h"
#include "RootConfig.hpp"
#include "Modules/ModuleManager.h"

#include "MemoryManager.hpp"

Init::Init(int argc, char **argv) : _argc(argc), _argv(argv)
{
	_conf = new Config();
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
    delete this->_pool;
}

/// Run the sequential initialization
int         Init::start()
{
    this->readCommandLine();
	try
	{
		this->readConfiguration();
	}
	catch (ticpp::Exception& ex)
	{
		Logger::getInstance() << Logger::Warning << "Can't load configuration file" << ex.what() << Logger::Flush;
        return EXIT_FAILURE;
	}
    if (this->checkConfig() == false)
        return EXIT_FAILURE;
    this->initModules();
    this->initSockets();
    this->initThreads();

    return EXIT_SUCCESS; // EXIT_FAILURE on error
}

/// Read and process arguments from the command line
void        Init::readCommandLine()
{

}

void		Init::addVhost(ticpp::Element& node)
{
	std::string		addr;
	std::string		port;

	addr = node.GetAttribute("address");
	port = node.GetAttribute("port");
	if (addr == "" || port == "")
	{
		Logger::getInstance() << Logger::Warning << "Can't have VirtualHost without both address and port" << Logger::Flush;
		return ;
	}
	Vhost*	v = new Vhost(NetworkID::factory(addr, port), this->_conf);
	this->parseConfigNode(static_cast<ticpp::Node*>(&node), static_cast<Config*>(v));
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

void		Init::addMimeType(ticpp::Element& node)
{
	std::string fileExts = node.GetAttribute("file");

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
	try
	{
		this->readConfiguration(fileName, cfg);
	}
	catch (ticpp::Exception& ex)
	{
		Logger::getInstance() << Logger::Warning << "Can't include file " << fileName << ": " << Zia::Newline << ex.what() << Logger::Flush;
	}
}

void		Init::parseConfigNode(ticpp::Node* node, Config* cfg)
{
	ticpp::Iterator<ticpp::Element>	it;

	for (it = it.begin(node); it != it.end(); ++it)
	{
		if (it->Value() == "VirtualHost")
			this->addVhost(*it);
		else if (it->Value() == "Include")
			this->includeConfigFile(it->GetText(), cfg);
		else if (it->Value() == "type")
			this->addMimeType(*it); 
		// Generic parameters
		else
		{
			cfg->setParam(it->Value(), it->GetText());
			Logger::getInstance() << Logger::Info << Logger::NoStdOut << "Adding " << it->Value() << " = " << *(this->_conf->getParam(it->Value())) << " to conf" << Zia::Newline << Logger::End;
		}
	}
	Logger::getInstance() << Logger::NoStdOut << Logger::Flush;
}

/// Read the XML configuration
void        Init::readConfiguration(const std::string fileName, Config* cfg)
{
	if (!cfg)
		cfg = this->_conf;
	ticpp::Document	doc(fileName);
	doc.LoadFile();

	ticpp::Node* node =	doc.FirstChild();
	this->parseConfigNode(node, cfg);
}

void        Init::initModules()
{
    ModuleManager::getInstance().init(10); //FIXME : get thread number from conf.
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

/// Start the server sockets
void        Init::initSockets()
{
	std::list<Vhost*>::iterator		it = this->_vhosts.begin();
	std::list<Vhost*>::iterator		end = this->_vhosts.end();

	this->addWildcardVhosts();
	this->addNonWildcardVhosts();
}

/// Spawn the threads
void        Init::initThreads()
{
    //FIXME : adapt the number of threads from the config.
    _pool = new Pool(10, 300);
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
    else if (this->_conf->isSet("Timeout") == false) //FIXME : check if timeout is a number.
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
