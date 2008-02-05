#ifndef ADDRESS_H__
# define ADDRESS_H__

#include <string>
#include "Wildcard.hpp"

class Address : public IWildcard<Address> 
{
	public:
		Address(std::string);
		std::string		getAddr() const;
		virtual bool	compare(const Address&) const;
	private:
		std::string		_addr;
};

class AddressWildcard : public Address, public Wildcard<Address>
{
	public:
		AddressWildcard();
};

#endif //ADDRESS_H__

