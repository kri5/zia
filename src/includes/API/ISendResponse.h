#ifndef __ISENDRESPONSE_H__
# define __ISENDRESPONSE_H__

#include "IModule.h"
#include "IHttpRequest.h"
#include "IHttpResponse.h"

#include <fstream>

class ISendResponse : public IModule
{
    public:
        virtual ~ISendResponse() {};
        virtual void    setInput(IModule*) = 0;
        virtual void    setInput(std::iostream&) = 0;

        virtual ChainStatus preSend(IHttpRequest&, IHttpRequest&) = 0;
        virtual size_t      processContent(char*, size_t) = 0;
        virtual ChainStatus postSend(IHttpRequest&, IHttpResponse&) = 0;
};

#endif /* !__ISENDRESPONSE_H__ */
