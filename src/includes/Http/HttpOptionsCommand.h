#ifndef __HTTPOPTIONSCOMMAND_H__
# define __HTTPOPTIONSCOMMAND_H__

#include "Http/IHttpCommand.h"
#include "Http/HttpRequest.h"
#include "Http/HttpResponse.h"
#include "API/IClientSocket.h"
#include "Utils/Buffer.h"

class HttpOptionsCommand : public IHttpCommand
{
    public:
        HttpOptionsCommand();
        ~HttpOptionsCommand();
        void    execute(HttpRequest*, HttpResponse*);
        void    manageRequestBody(HttpRequest*, Buffer*,
                                  zAPI::IClientSocket*);
};

#endif /* !__HTTPOPTIONSCOMMAND_H__ */
