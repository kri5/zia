#include "Vhost.h"
#include "Logger.hpp"
#include <sstream>

Vhost::Vhost(NetworkID* netId) : _netId(netId)
{
}

std::string			Vhost::getAddress() const
{
//FIXME
//return this->_address;
}

int					Vhost::getPort() const
{
//FIXME
//	return this->_port;
}

std::string		Vhost::getParam(std::string name) const
{
	if (name == "User" || name == "Group")
		throw 0;
	return (Config::getParam(name));
}
