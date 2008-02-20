#include "Logger.hpp"

#include "NetworkID.h"
#include "ZException.hpp"

#include "MemoryManager.hpp"

NetworkID::NetworkID(Address* addr, Port* port) : _addr(addr), _port(port)
{
}

NetworkID::~NetworkID()
{
	delete this->_addr;
	delete this->_port;
}

bool			NetworkID::isWildcard(std::string id)
{
	return (id == "*");
}

NetworkID*		NetworkID::factory(std::string addr, std::string port)
{
	if (isWildcard(addr) && isWildcard(port))
	{
		throw ZException<NetworkID>(INFO, NetworkID::Error::PortWildcard);
	}
	else if (isWildcard(addr) && !isWildcard(port))
	{
		return new NetworkID(new AddressWildcard(), new Port(port));
	}
	else if (!isWildcard(addr) && isWildcard(port))
	{
		throw ZException<NetworkID>(INFO, NetworkID::Error::PortWildcard);
	}
	return new NetworkID(new Address(addr), new Port(port));
}

Address*		NetworkID::getAddress() const
{
	return this->_addr;
}

Port*			NetworkID::getPort() const
{
	return this->_port;
}

bool			NetworkID::compare(const NetworkID& right) const
{
	return (this->_addr->compare(*right.getAddress()) && this->_port->compare(*right.getPort()));
}

bool			NetworkID::compare(const char* str) const
{
	return (this->_addr->compare(str) 
				&& this->_port->compare(&str[strlen(this->_addr->getAddr().c_str()) + 1]));
}

