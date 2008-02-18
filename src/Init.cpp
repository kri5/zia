#include <iostream>
#include <new>

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
int         Init::run()
{
    this->readCommandLine();
    this->readConfiguration();
    this->initSSL();
    this->initSockets();
    this->initThreads();

    return EXIT_SUCCESS; // EXIT_FAILURE on error
}

/// Read and process arguments from the command line
void        Init::readCommandLine()
{

}

void		Init::addVhost(const ticpp::Element& node)
{
	std::string		addr;
	std::string		port;

	addr = node.GetAttribute("address");
	port = node.GetAttribute("port");
	Vhost*	v = new Vhost(NetworkID::factory(addr, port));
	this->_binds[v->getPort()][v->getAddress()].push_back(v);
	this->_vhosts.push_back(v);
}

void		Init::parseConfigNode(ticpp::Node* node, Config* cfg)
{
	ticpp::Iterator<ticpp::Element>	it;

	for (it = it.begin(node); it != it.end(); ++it)
	{
		//std::cout << *it << std::endl;
		if (it->Value() == "VirtualHost")
		{
			this->addVhost(*it);
		}
		else
		{
			cfg->setParam(it->Value(), it->GetText());
			Logger::getInstance() << Logger::Info << Logger::NoStdOut << "Adding " << it->Value() << " = " << this->_conf->getParam(it->Value()) << " to conf" << Zia::Newline;
		}
	}
	Logger::getInstance() << Logger::Flush;
}

/// Read the XML configuration
void        Init::readConfiguration()
{
	try
	{
		ticpp::Document	doc("zia.conf");
		doc.LoadFile();

		ticpp::Node* node =	doc.FirstChild();
		this->parseConfigNode(node, this->_conf);
	}
	catch (ticpp::Exception& ex)
	{
		std::cerr << ex.what() << std::endl;
	}
}

/// Initialize the SSL features
void        Init::initSSL()
{
  SSL_library_init(); // load encryption & hash algorithms for SSL
  SSL_load_error_strings(); // load the error strings for good error reporting
  Logger::getInstance() << Logger::Info << "SSL loaded successfully." << Logger::Flush;
}

/// Start the server sockets
void        Init::initSockets()
{
	Address*		addr;
	Port*			port;

	//Browsing the Address* map
	std::cout << "Starting binding server socket(s)" << std::endl;
	std::map<Port*, std::map<Address*, std::vector<Vhost*> > >::iterator	itPort = this->_binds.begin();
	while (itPort != this->_binds.end())
	{
		port = itPort->first;
		std::map<Address*, std::vector<Vhost*> >::iterator		itAddr = itPort->second.begin();
		while (itAddr != itPort->second.end())
		{
			if (*(itAddr->first) == "*")
			{
				std::cout << "Wilcard address : binding on INADDR_ANY" << std::endl;
				break ;
			}
			addr = itAddr->first;
			std::cout << "Binding addr " << addr->getAddr() << ":" << port->getPort() << std::endl;
			++itAddr;
		}
		++itPort;
	}
}

/// Spawn the threads
void        Init::initThreads()
{

}

