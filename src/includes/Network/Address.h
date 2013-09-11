#ifndef ADDRESS_H__
# define ADDRESS_H__

#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <string>

#include "zia.h"

#include "Wildcard.hpp"

/// Will represent an IP address. This class is wildcardable.
class Address : public IWildcard<Address> {
public:
  virtual ~Address() {}
  Address(std::string &);
  Address(std::string &&);
  std::string getAddr() const;
  virtual unsigned long getInAddr() const;
  virtual bool compare(const Address &) const;
  virtual bool compare(const char *c) const;
  virtual bool operator==(const Address &c) const { return this->compare(c); }
  virtual bool operator==(const char *c) const { return this->compare(c); }
private:
  std::string _addr;
};

/// Wildcard address.
class AddressWildcard : public Address, public Wildcard<Address> {
public:
  AddressWildcard();
  unsigned long getInAddr() const { return INADDR_ANY; }
};

#endif //ADDRESS_H__

