#ifndef __HTTPCOMMANDBUILDER_H__
# define __HTTPCOMMANDBUILDER_H__

#include "Http/IHttpCommand.h"
#include "API/IHttpResponse.h"
#include "Http/HttpRequest.h"

class HttpCommandBuilder
{
    public:
        HttpCommandBuilder();
        ~HttpCommandBuilder();
        IHttpCommand*        buildCommand(HttpRequest*);
};

#endif /* !__HTTPCOMMANDBUILDER_H__ */
