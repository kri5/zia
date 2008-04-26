#ifndef __IHTTPCOMMAND_H__
# define __IHTTPCOMMAND_H__

#include "Http/HttpRequest.h"
#include "Http/HttpResponse.h"
#include "Utils/Buffer.h"
#include "API/IClientSocket.h"

class IHttpCommand
{
    public:
        virtual ~IHttpCommand() {};
        virtual void    execute(HttpRequest*, HttpResponse*) = 0;
        virtual void    manageRequestBody(HttpRequest*, Buffer*, 
                                          zAPI::IClientSocket*) = 0;
};

#endif /* !__IHTTPCOMMAND_H__ */
