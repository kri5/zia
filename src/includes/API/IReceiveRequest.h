#ifndef __IRECEIVEREQUEST_H__
# define __IRECEIVEREQUEST_H__

#include "API/IModule.h"
#include "API/IHttpRequest.h"
#include "API/IHttpResponse.h"

class IReceiveRequest : public IModule
{
    public:
        virtual ~IReceiveRequest() {};
        virtual ChainStatus onPreReceive(IHttpRequest&, IHttpResponse&) = 0;
        virtual ChainStatus onPostReceive(IHttpRequest&, IHttpResponse&) = 0;
};

#endif /* !__IRECEIVEREQUEST_H__ */
