#include <sstream>
#include "Logger.hpp"
#include "Port.h"

Port::Port(int port)
{
	_port = port;
}

Port::Port(std::string port)
{
	std::istringstream		stream(port);
	if (stream.good() == false)
	{
		std::string errMsg("Bad port value ");
		errMsg += port + ")";
		Logger::getInstance() << Logger::ERRORLVL << "Bad port value (" << port << ')' << Logger::FLUSH;
		throw 0;
	}
	stream >> _port;
}

int		Port::getPort() const
{
	return this->_port;
}

bool	Port::compare(const Port& right) const
{
	//FIXME
	throw 0;
}

PortWildcard::PortWildcard() : Port(0)
{
}
