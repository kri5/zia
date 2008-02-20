#ifndef NETWORKID_H__
# define NETWORKID_H__

#include "Address.h"
#include "Port.h"

/// Will represent a couple Address/Port.
class	NetworkID
{
	public:
		struct	Error
		{
			enum	Code
			{
				Unknown,
				InvalidConfig,
				PortWildcard
			};
			static const char*	Msg[];
		};
		virtual ~NetworkID();
		/// Factory which will force the developper to use it, and not the constructor.
		static NetworkID* factory(std::string addr, std::string port);
		/// Will compare two NetworkID.
		virtual bool	compare(const NetworkID&) const;
		virtual bool	compare(const char* c) const;
		virtual bool	operator==(const NetworkID& c) const {return this->compare(c);}
		virtual bool	operator==(const char* c) const {return this->compare(c);}

		/// Will return the Address of this network id.
		Address*		getAddress() const;
		/// Will return the Port of this network id.
		Port*			getPort() const;
	protected:
		static bool		isWildcard(std::string);
		/// Private constructor to ensure instantiation via the factory method.
		NetworkID(Address*, Port*);

		Address*	_addr;
		Port*		_port;
};

#endif //NETWORKID_H__

