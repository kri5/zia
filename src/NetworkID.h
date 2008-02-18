#ifndef NETWORKID_H__
# define NETWORKID_H__

#include "Address.h"
#include "Port.h"
#include "Wildcard.hpp"

/// Will represent a couple Address/Port.
class	NetworkID : public IWildcard<NetworkID> 
{
	public:
		struct	Error
		{
			enum	Code
			{
				Unknown,
				InvalidConfig
			};
			static const char*	Msg[];
		};
		virtual ~NetworkID();
		/// Factory which will force the developper to use it, and not the constructor.
		static NetworkID* factory(std::string, std::string);
		/// Will compare two NetworkID. can be overwritten by Wilcard<NetworkID>
		virtual bool	compare(const NetworkID&) const;
		virtual bool	compare(const char* c) const;
		virtual bool	operator==(const NetworkID& c) const {return this->compare(c);}
		virtual bool	operator==(const char* c) const {return this->compare(c);}

		/// Will return the Address of this network id.
		Address*		getAddress() const;
		/// Will return the Port of this network id.
		Port*			getPort() const;
	protected:
		/// Private constructor to ensure instantiation via the factory method.
		NetworkID(Address*, Port*);

		Address*	_addr;
		Port*		_port;
};

/// Wildcarded NetworkID, will always match comparaison.
class NetworkIDWildcard : public NetworkID, public Wildcard<NetworkID>
{
	public:
		NetworkIDWildcard(Address*, Port*);
};

#endif //NETWORKID_H__

