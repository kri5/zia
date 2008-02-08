#include "Vhost.h"
#include "Logger.hpp"
#include <sstream>

#include "MemoryManager.hpp"

Vhost::Vhost(NetworkID* netId) : _netId(netId)
{
}

Address*			Vhost::getAddress() const
{
	return this->_netId->getAddress();
}

Port*			Vhost::getPort() const
{
	return this->_netId->getPort();
}

std::string		Vhost::getParam(std::string name) const
{
	if (name == "User" || name == "Group")
		throw 0;
	return (Config::getParam(name));
}
