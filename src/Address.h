#ifndef ADDRESS_H__
# define ADDRESS_H__

#include <string>
#include "Wildcard.hpp"

/// Will represent an IP address. This class is wildcardable.
class Address : public IWildcard<Address> 
{
	public:
		virtual ~Address(){}
		Address(std::string);
		std::string		getAddr() const;
		virtual bool	compare(const Address&) const;
		virtual bool	compare(const char* c) const;
		virtual bool	operator==(const Address& c) const {return this->compare(c);}
		virtual bool	operator==(const char* c) const {return this->compare(c);}
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

