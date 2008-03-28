#ifndef __IHTTPTRANSACTION_H__
# define __IHTTPTRANSACTION_H__

#include <string>
#include <map>

class IHttpTransaction
{
    public:
        virtual ~IHttpTransaction() {};

        virtual void                                        setHeaderOption(const std::string&, const std::string&) = 0;
        virtual std::string                                 getHeaderOption(const std::string&) const = 0;
        virtual const std::map<std::string, std::string>&   getHeaderOptions() const = 0;
        virtual bool                                        headerOptionIsSet(const std::string&) const = 0;

        virtual void                                        setProtocol(const std::string&) = 0;
        virtual const std::string&                          getProtocol() const = 0;
};

# endif /* !__IHTTPTRANSACTION_H__ */
