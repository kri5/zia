#ifndef __HTTPCOMMANDBUILDER_H__
# define __HTTPCOMMANDBUILDER_H__

#include "Http/IHttpCommand.h"
#include "API/IHttpResponse.h"
#include "API/IHttpRequest.h"

class HttpCommandBuilder
{
    public:
        HttpCommandBuilder();
        ~HttpCommandBuilder();
        IHttpCommand*        buildCommand(zAPI::IHttpRequest*);
};

#endif /* !__HTTPCOMMANDBUILDER_H__ */
