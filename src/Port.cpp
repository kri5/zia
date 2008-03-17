#include <sstream>

#include "Logger.hpp"
#include "Network/Port.h"
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
		throw ZException<Port>(INFO, Port::Error::Invalid, port.c_str());
	stream >> _port;
	if (stream.good() == false)
	{

	}
}

int		Port::getPort() const
{
	return this->_port;
}

u_short	Port::getHtonsPort() const
{
	return htons(this->_port);
}

bool	Port::compare(const Port& right) const
{
	return (this->_port == right.getPort());
}

bool	Port::compare(const char* str) const
{
	return (atoi(str) == this->_port);
}

