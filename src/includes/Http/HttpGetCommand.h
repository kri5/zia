#ifndef __HTTPGETCOMMAND_H__
# define __HTTPGETCOMMAND_H__

#include "Http/IHttpCommand.h"
#include "Http/HttpRequest.h"
#include "Http/HttpResponse.h"
#include "Utils/Buffer.h"
#include "API/IClientSocket.h"

class HttpGetCommand : public IHttpCommand
{
    public:
        HttpGetCommand();
        ~HttpGetCommand();
        void    execute(HttpRequest*, HttpResponse*);
        void    manageRequestBody(HttpRequest*, Buffer*,
                                  zAPI::IClientSocket*);
};

#endif /* !__HTTPGETCOMMAND_H__ */
