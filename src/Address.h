#ifndef ADDRESS_H__
# define ADDRESS_H__

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string>
#include "Wildcard.hpp"

/// Will represent an IP address. This class is wildcardable.
class Address : public IWildcard<Address> 
{
	public:
		virtual ~Address(){}
		Address(std::string);
		std::string			getAddr() const;
		virtual in_addr_t	getInAddr() const;
		virtual bool		compare(const Address&) const;
		virtual bool		compare(const char* c) const;
		virtual bool		operator==(const Address& c) const {return this->compare(c);}
		virtual bool		operator==(const char* c) const {return this->compare(c);}
	private:
		std::string			_addr;
};

/// Wildcard address.
class AddressWildcard : public Address, public Wildcard<Address>
{
	public:
		AddressWildcard();
		in_addr_t			getInAddr() const{return INADDR_ANY;};
};

#endif //ADDRESS_H__

