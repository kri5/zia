#ifndef PORT_H__
#define PORT_H__

#include "Wildcard.hpp"
#include <string>

class Port : public IWildcard<Port>
{
	public:
		/// Will construct a Port object with an int as port value.
		Port(int);
		/// Will convert std::string to int, then assign this int to _port
		Port(std::string);
		int				getPort() const;
		/// Will compare 2 Ports, and return true if they match. Will be reimplemented if port is a wildcard port.
		virtual bool	compare(const Port&) const;
	protected:
		int		_port;
};

class PortWildcard : public Port, public Wildcard<Port>
{
	public:
		PortWildcard();
};

#endif //PORT_H__


