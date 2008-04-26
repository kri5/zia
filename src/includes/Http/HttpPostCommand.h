#ifndef __HTTPPOSTCOMMAND_H__
# define __HTTPPOSTCOMMAND_H__

#include "Http/IHttpCommand.h"
#include "Http/HttpRequest.h"
#include "Http/HttpResponse.h"
#include "Utils/Buffer.h"
#include "API/IClientSocket.h"

class HttpPostCommand : public IHttpCommand
{
    public:
        HttpPostCommand();
        ~HttpPostCommand();
        void    execute(HttpRequest*, HttpResponse*);
        void    manageRequestBody(HttpRequest*, Buffer*,
                                  zAPI::IClientSocket*);
};


#endif /* !__HTTPPOSTCOMMAND_H__ */
