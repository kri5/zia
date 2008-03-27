#ifndef __IHTTPTRANSACTION_H__
# define __IHTTPTRANSACTION_H__

#include <string>
#include <map>

class IHttpTransaction
{
    public:
        virtual ~IHttpTransaction() {};

        virtual void                                        setHeaderOption(std::string, std::string&) = 0;
        virtual void                                        setHeaderOptions(std::map<std::string, std::string>) = 0;
        virtual std::string                                 getHeaderOption(std::string) const = 0;
        virtual const std::map<std::string, std::string>&   getHeaderOpions() const = 0;

        virtual void                                        setProtocol(std::string&) = 0;
        virtual std::string&                                getProtocol() const = 0;
};

# endif /* !__IHTTPTRANSACTION_H__ */