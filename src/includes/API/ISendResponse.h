#ifndef __ISENDRESPONSE_H__
# define __ISENDRESPONSE_H__

#include "API/IModule.h"
#include "API/IHttpRequest.h"
#include "API/IHttpResponse.h"

#include <fstream>

class ISendResponse : public IModule
{
    public:
        virtual ~ISendResponse() {};
        virtual void    setInput(IModule*) = 0;
        virtual void    setInput(std::iostream&) = 0;

        virtual ChainStatus onPreSend(IHttpRequest&, IHttpRequest&) = 0;
        virtual size_t      onProcessContent(char*, size_t) = 0;
        virtual ChainStatus onPostSend(IHttpRequest&, IHttpResponse&) = 0;
};

#endif /* !__ISENDRESPONSE_H__ */
