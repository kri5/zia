#include <iostream>
#include <string.h>

#include "Init.h"
#include "NetworkID.h"
#include "Zerror.h"

#include "MemoryManager.hpp"

Init::Init(int argc, char **argv) : _argc(argc), _argv(argv)
{
 //   std::set_new_handler(ZNewHandler);
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
	}
    this->initSSL();
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
	//insertion dans la liste des vhosts.
	this->_vhosts.push_back(v);
}

void		Init::addMimeType(ticpp::Element& node, Config* cfg)
{
	std::string& fileExts = node.GetAttribute("file");

	if (fileExts == "")
		return ;
	size_t	len = fileExts.length();
	size_t	begin = 0;
	size_t	end;
	while ((end = fileExts.find(';', begin)) != std::string::npos)
	{
		cfg->addMimeType(fileExts.substr(begin, end - begin), node.GetText());
		begin = end + 1;
	}
}

void		Init::includeConfigFile(std::string fileName, Config* cfg)
{
	try
	{
		this->readConfiguration(fileName, cfg);
	}
	catch (ticpp::Exception& ex)
	{
		Logger::getInstance() << Logger::Warning << "Can't include file " << fileName << ex.what() << Logger::Flush;
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
			this->addMimeType(*it, cfg);
		// Generic parameters
		else
		{
			cfg->setParam(it->Value(), it->GetText());
			Logger::getInstance() << Logger::Info << Logger::NoStdOut << "Adding " << it->Value() << " = " << this->_conf->getParam(it->Value()) << " to conf" << Zia::Newline;
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

/// Initialize the SSL features
void        Init::initSSL()
{
  SSL_library_init(); // load encryption & hash algorithms for SSL
  SSL_load_error_strings(); // load the error strings for good error reporting
  Logger::getInstance() << Logger::Info << "SSL loaded successfully." << Logger::Flush;
}

void		Init::addWildcardVhosts()
{
	std::list<Vhost*>::iterator		it = this->_vhosts.begin();
	std::list<Vhost*>::iterator		end = this->_vhosts.end();
	std::map<const NetworkID*, std::vector<const Vhost*> >::iterator	itNet;
	std::map<const NetworkID*, std::vector<const Vhost*> >::iterator	endNet;

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
	std::map<const NetworkID*, std::vector<const Vhost*> >::iterator	itNet;
	std::map<const NetworkID*, std::vector<const Vhost*> >::iterator	endNet;

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

}

const std::map<const NetworkID*, std::vector<const Vhost*> >&		Init::getBindList()
{
	return this->_bindList;
}

