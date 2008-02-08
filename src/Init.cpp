#include "Init.h"
#include "NetworkID.h"
#include <iostream>

Init::Init(int argc, char **argv) : _argc(argc), _argv(argv)
{
	_conf = new Config();
}

/// Will destroy logger, since this function is supposed to be the last called in this server.
Init::~Init()
{
	delete this->_conf;
	Logger::deleteInstance();
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

std::string		Init::getConfigNodeValue(Xml::Node* node)
{
	return ("novalue");
}

void		Init::addVhost(Xml::Node* node)
{
	std::string		addr;
	std::string		port;

	addr = node->getAttr("address");
	port = node->getAttr("port");
	Vhost*	v = new Vhost(NetworkID::factory(addr, port));
	this->_binds[v->getAddress()][v->getPort()].push_back(v);
	this->_vhosts.push_back(v);
}

void		Init::parseConfigNode(Xml::Node* node, Config* cfg)
{
	Xml::Node*		subNode;
	while (subNode = node->getNextChildNode())
	{
		std::string		nodeName = subNode->getName();

		if (nodeName == "VirtualHost")
		{
			this->addVhost(subNode);
		}
		else
		{
			this->_conf->setParam(nodeName, subNode->getValue());
			std::cout << "Adding " << nodeName << " = " << this->_conf->getParam(nodeName) << " to conf" << std::endl;
		}
	}
}

/// Read the XML configuration
void        Init::readConfiguration()
{
	Xml::Document* doc = new Xml::Document("zia.conf");
	Xml::Node* root = doc->getRootNode();
	this->parseConfigNode(root, this->_conf);
}

/// Initialize the SSL features
void        Init::initSSL()
{
  SSL_library_init(); // load encryption & hash algorithms for SSL
  SSL_load_error_strings(); // load the error strings for good error reporting
  Logger::getInstance() << Logger::INFO << "SSL loaded successfully." << Logger::FLUSH;
}

/// Start the server sockets
void        Init::initSockets()
{
	Address*		addr;
	Port*			port;

	//Browsing the Address* map
	std::cout << "Starting binding server socket(s)" << std::endl;
	std::map<Address*, std::map<Port*, std::vector<Vhost*> > >::iterator	itAddr = this->_binds.begin();
	while (itAddr != this->_binds.end())
	{
		addr = itAddr->first;
		std::map<Port*, std::vector<Vhost*> >::iterator		itPorts = itAddr->second.begin();
		while (itPorts != itAddr->second.end())
		{
			port = itPorts->first;
			std::cout << "Binding addr " << addr->getAddr() << ":" << port->getPort() << std::endl;
			++itPorts;
		}
		++itAddr;
	}
}

/// Spawn the threads
void        Init::initThreads()
{

}

