#include "Network/Address.h"

#include "MemoryManager.hpp"

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
	return (this->_addr == right.getAddr());
}

bool		Address::compare(const char* str) const
{
	return (!strncmp(str, this->_addr.c_str(), strlen(this->_addr.c_str())));
}

unsigned long	Address::getInAddr() const
{
	return (inet_addr(this->_addr.c_str()));
}

