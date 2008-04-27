#ifndef __HTTPERRORCOMMAND_H__
# define __HTTPERRORCOMMAND_H__

#include "Http/IHttpCommand.h"
#include "Http/HttpRequest.h"
#include "Http/HttpResponse.h"
#include "API/IClientSocket.h"
#include "Utils/Buffer.h"

class HttpErrorCommand : public IHttpCommand
{
    public:
        HttpErrorCommand(int);
        ~HttpErrorCommand();
        void    execute(HttpRequest*, HttpResponse*);
        void    manageRequestBody(HttpRequest*, Buffer*,
                                  zAPI::IClientSocket*);
    private:
        int     _error;
};

#endif /* !__HTTPERRORCOMMAND_H__ */
