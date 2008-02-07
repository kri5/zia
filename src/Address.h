#ifndef ADDRESS_H__
# define ADDRESS_H__

#include <string>
#include "Wildcard.hpp"

/// Will represent an IP address. This class is wildcardable.
class Address : public IWildcard<Address> 
{
	public:
		Address(std::string);
		std::string		getAddr() const;
		virtual bool	compare(const Address&) const;
	private:
		std::string		_addr;
};

/// Wildcard address.
class AddressWildcard : public Address, public Wildcard<Address>
{
	public:
		AddressWildcard();
};

#endif //ADDRESS_H__

