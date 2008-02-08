#include "Logger.hpp"

#include "NetworkID.h"

#include "MemoryManager.hpp"

NetworkID::NetworkID(Address* addr, Port* port) : _addr(addr), _port(port)
{
}

NetworkID*		NetworkID::factory(std::string addr, std::string port)
{
	if (addr == "" && port == "")
	{
		Logger::getInstance() << Logger::INFO << "Instanciating wildcard NetworkID" << Logger::FLUSH;
		return new NetworkIDWildcard(new AddressWildcard(), new PortWildcard());
	}
	else if (addr == "" && port != "")
	{
		Logger::getInstance() << Logger::ERRORLVL << "Can't have an address wildcard without port !" << Logger::FLUSH;
		throw 0;
	}
	else if (addr != "" && port == "")
	{
		Logger::getInstance() << Logger::INFO << "Instanciating NetworkID with wildcard port." << Logger::FLUSH;
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
	//FIXME
	throw 0;
}


NetworkIDWildcard::NetworkIDWildcard(Address* addr, Port* port) : NetworkID(addr, port)
{
}
