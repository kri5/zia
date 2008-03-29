#ifndef __IHTTPREQUEST_H__
# define __IHTTPREQUEST_H__

#include "API/IHttpTransaction.h"

#include <string>
#include <map>

class IHttpRequest : public virtual IHttpTransaction
{
    public:
        virtual ~IHttpRequest() {};

        virtual void                                        setUri(const std::string&, bool = true) = 0;
        virtual const std::string&                          getUri() const = 0;
        virtual void                                        setUriArgument(const std::string&, const std::string&) = 0;
        virtual const std::map<std::string, std::string>&   getUriArguments() const = 0;

        //virtual void                                        setBodyArguments(std::map<std::string, std::string>) = 0;
        virtual void                                        setBodyArgument(const std::string&, const std::string&) = 0;
        //virtual const std::map<std::string, std::string>&   getBodyArguments() const = 0;
        //virtual std::string                                 getBodyArgument(std::string) const = 0;

        virtual bool                                        setParam(const std::string&, void*) = 0;
        virtual void*                                       getParam(const std::string&) const = 0;
};

#endif /* !__IHTTPREQUEST_H__ */
