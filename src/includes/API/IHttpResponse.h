#ifndef __IHTTPRESPONSE_H__
# define __IHTTPRESPONSE_H__

#include "IHttpTransaction.h"

class IHttpResponse// : public IHttpTransaction
{
    public:
        virtual ~IHttpResponse() {};

        virtual void                setResponseStatus(int) = 0;
        virtual int                 getResponseStatus() const = 0;

        virtual const std::string   getResponseValue() const = 0;
        static const char*          getResponseStatusMessage(int);

        virtual std::iostream&      getContent() = 0;
};

#endif /* !__IHTTPRESPONSE_H__ */
