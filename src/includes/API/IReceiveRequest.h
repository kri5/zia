#ifndef __IRECEIVEREQUEST_H__
# define __IRECEIVEREQUEST_H__

#include "IModule.h"
#include "IHttpRequest.h"
#include "IHttpResponse.h"

class IReceiveRequest : public IModule
{
    public:
        virtual ~IReceiveRequest() {};
        virtual ChainStatus preReceive(IHttpRequest&, IHttpResponse&) = 0;
        virtual ChainStatus postReceive(IHttpRequest&, IHttpResponse&) = 0;
};

#endif /* !__IRECEIVEREQUEST_H__ */
