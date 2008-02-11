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

NetworkID*		NetworkID::factory(std::string addr, std::string port)
{
	if (addr == "" && port == "")
	{
		Logger::getInstance() << Logger::Info << "Instanciating wildcard NetworkID" << Logger::Flush;
		return new NetworkIDWildcard(new AddressWildcard(), new PortWildcard());
	}
	else if (addr == "" && port != "")
	{
		//Logger::getInstance() << Logger::Error << "Can't have an address wildcard without port !" << Logger::Flush; //commented because the exception will also log this message.
		throw ZException<NetworkID>(INFO, NetworkID::Error::InvalidConfig, "Can't have an address wildcard without port !");
	}
	else if (addr != "" && port == "")
	{
		Logger::getInstance() << Logger::Info << "Instanciating NetworkID with wildcard port." << Logger::Flush;
		return new NetworkID(new Address(addr), new PortWildcard());
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


NetworkIDWildcard::NetworkIDWildcard(Address* addr, Port* port) : NetworkID(addr, port)
{
}
