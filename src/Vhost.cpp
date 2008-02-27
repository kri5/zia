#include <sstream>

#include "Vhost.h"
#include "Logger.hpp"
#include "ZException.hpp"

#include "MemoryManager.hpp"

Vhost::Vhost(NetworkID* netId, Config* cfg) : Config(*cfg), _netId(netId) 
{
}

Vhost::~Vhost()
{
	delete this->_netId;
}

const Address&			Vhost::getAddress() const
{
	return this->_netId->getAddress();
}

const Port&			Vhost::getPort() const
{
	return this->_netId->getPort();
}

const NetworkID&	Vhost::getNetworkID() const
{
	return *(this->_netId);
}

std::string		Vhost::getParam(std::string name) const
{
	if (name == "User" || name == "Group")
		throw ZException<Vhost>(INFO, Vhost::Error::InvalidConfig, name.c_str());
	return (Config::getParam(name));
}

const Vhost*	Vhost::getVhost(const std::vector<const Vhost*>& vhosts, const std::string& host)
{
	int		i;
	int		size = vhosts.size();

	for (i = 0; i < size; ++i)
	{
		if (vhosts[i]->getParam("ServerName") == host.substr(0, host.find(':')))
			return vhosts[i];
	}
	Logger::getInstance() << Logger::Error << "No such Vhost" << Logger::Flush;
	return NULL;
}
