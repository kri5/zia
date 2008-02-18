#ifndef PORT_H__
#define PORT_H__

#include "Wildcard.hpp"
#include <string>

/// Will represent a port, as a part of a NetworkID (network identifier)
class Port : public IWildcard<Port>
{
	public:
		struct	Error
		{
			enum	Code
			{
				Unknown,
				Invalid
			};
			static const char* Msg[];
		};
		/// Will construct a Port object with an int as port value.
		Port(int);
		/// Will convert std::string to int, then assign this int to _port
		Port(std::string);
		virtual ~Port(){}
		/// Will return the port as an int.
		int				getPort() const;
		/// Will compare 2 Ports, and return true if they match. Will be reimplemented if port is a wildcard port.
		virtual bool	compare(const Port&) const;
		virtual bool	compare(const char* c) const;
		virtual bool	operator==(const Port& c) const {return this->compare(c);}
		virtual bool	operator==(const char* c) const {return this->compare(c);}
	protected:
		int		_port;
};

/// Wildcarded port.
class PortWildcard : public Port, public Wildcard<Port>
{
	public:
		PortWildcard();
};

#endif //PORT_H__


