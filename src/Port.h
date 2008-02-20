#ifndef PORT_H__
#define PORT_H__

#include <string>
#ifndef WIN32
#include <arpa/inet.h>
#endif

#include "zia.h"

/// Will represent a port, as a part of a NetworkID (network identifier)
class Port
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
		u_short			getHtonsPort() const;
		/// Will compare 2 Ports, and return true if they match.
		virtual bool	compare(const Port&) const;
		virtual bool	compare(const char* c) const;
		virtual bool	operator==(const Port& c) const {return this->compare(c);}
		virtual bool	operator==(const char* c) const {return this->compare(c);}
	protected:
		int		_port;
};

#endif //PORT_H__


