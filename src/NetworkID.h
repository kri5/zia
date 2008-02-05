#ifndef NETWORKID_H__
# define NETWORKID_H__

#include "Address.h"
#include "Port.h"
#include "Wildcard.hpp"

class	NetworkID : public IWildcard<NetworkID> 
{
	public:
		/// Factory which will force the developper to use it, and not the constructor.
		static NetworkID* factory(std::string, std::string);
		virtual bool	compare(const NetworkID&) const;
	protected:
		/// Private constructor to ensure instantiation via the factory method.
		NetworkID(Address*, Port*);

		Address*	_addr;
		Port*		_port;
};

class NetworkIDWildcard : public NetworkID, public Wildcard<NetworkID>
{
	public:
		NetworkIDWildcard(Address*, Port*);
};

#endif //NETWORKID_H__

