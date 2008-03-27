#ifndef __IHTTPREQUEST_H__
# define __IHTTPREQUEST_H__

#include "IHttpTransaction.h"

#include <string>
#include <map>

class IHttpRequest : public IHttpTransaction
{
    public:
        virtual ~IHttpRequest() {};

        virtual void                                        setUri(std::string) = 0;
        virtual std::string                                 getUri() const = 0;
        virtual void                                        setUriArgument(std::string, std::string&) = 0;
        virtual std::string                                 getUriArgument(std::string) const = 0;

        virtual void                                        setBodyArguments(std::map<std::string, std::string>) = 0;
        virtual void                                        setBodyArgument(std::string, std::string) = 0;
        virtual const std::map<std::string, std::string>&   getBodyArguments() const = 0;
        virtual std::string                                 getBodyArgument(std::string) const = 0;

        virtual bool                                        setParam(std::string, void*) = 0;
        virtual void*                                       getParam(std::string) const = 0;
};

#endif /* !__IHTTPREQUEST_H__ */
