#ifndef __HTTPTRACECOMMAND_H__
# define __HTTPTRACECOMMAND_H__

#include "Http/IHttpCommand.h"
#include "Http/HttpRequest.h"
#include "Http/HttpResponse.h"
#include "API/IClientSocket.h"
#include "Utils/Buffer.h"

class HttpTraceCommand : public IHttpCommand
{
  public:
    HttpTraceCommand();
      ~HttpTraceCommand();
      void  manageRequestBody(HttpRequest*, Buffer*, zAPI::IClientSocket*);
      void  execute(HttpRequest*, HttpResponse*);
};

#endif /* !__HTTPTRACECOMMAND_H__ */
