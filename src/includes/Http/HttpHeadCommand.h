#ifndef __HTTPHEADCOMMAND_H__
# define __HTTPHEADCOMMAND_H__

#include "Http/IHttpCommand.h"
#include "Http/HttpRequest.h"
#include "Http/HttpResponse.h"
#include "Utils/Buffer.h"
#include "API/IClientSocket.h"

class HttpHeadCommand : public IHttpCommand
{
    public:
        HttpHeadCommand();
        ~HttpHeadCommand();
        void    execute(HttpRequest*, HttpResponse*);
        void    manageRequestBody(HttpRequest*, Buffer*,
                                  zAPI::IClientSocket*);
};

#endif /* !__HTTPHEADCOMMAND_H__ */
