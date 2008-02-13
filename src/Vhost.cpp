#include <sstream>

#include "Vhost.h"
#include "Logger.hpp"
#include "ZException.hpp"

#include "MemoryManager.hpp"

Vhost::Vhost(NetworkID* netId) : _netId(netId)
{
}

Vhost::~Vhost()
{
	delete this->_netId;
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
		throw ZException<Vhost>(INFO, Vhost::Error::InvalidConfig, name.c_str());
	return (Config::getParam(name));
}
