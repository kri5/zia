#include <sstream>

#include "Logger.hpp"
#include "Port.h"
#include "ZException.hpp"

#include "MemoryManager.hpp"

Port::Port(int port)
{
	_port = port;
}

Port::Port(std::string port)
{
	std::istringstream		stream(port);

	if (stream.good() == false)
		throw ZException<Port>(INFO, Port::Error::Invalid, port);
	stream >> _port;
	if (stream.good() == false)
	{

	}
}

int		Port::getPort() const
{
	return this->_port;
}

bool	Port::compare(const Port& right) const
{
	return (this->_port == right.getPort());
}

PortWildcard::PortWildcard() : Port(0)
{
}
