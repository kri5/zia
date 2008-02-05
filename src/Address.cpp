#include "Address.h"

Address::Address(std::string addr)
{
	_addr = addr;
}

std::string		Address::getAddr() const
{
	return this->_addr;
}

AddressWildcard::AddressWildcard() : Address("*")
{
}

bool		Address::compare(const Address& right) const
{
	//FIXME
	throw 0;
}
