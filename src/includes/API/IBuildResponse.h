#ifndef __IBUILDRESPONSE_H__
# define __IBUILDRESPONSE_H__

#include "API/IModule.h"
#include "API/IHttpRequest.h"
#include "API/IHttpResponse.h"

/// Interface used for response hook
class IBuildResponse : public IModule
{
    public:

        /**
         *  The interface virtual destructor
         */
        virtual ~IBuildResponse(){};

        /**
         *  The preBuild hook.
         *  This module method will be called before sending the response header
         *  @param request a IHttpRequest
         *  @param reponse a IHttpResponse
         *  @return a ChainStatus
         */
        virtual ChainStatus     onPreBuild(IHttpRequest* request, IHttpResponse* response) = 0;

        /**
         *  The postBuild hook.
         *  This method will be called after the reponse header has been sent
         *  @param request a IHttpRequest
         *  @param response a IHttpResponse
         *  @return a ChainStatus
         */
        virtual ChainStatus     onPostBuild(IHttpRequest* request, IHttpResponse* response) = 0;
};

#endif /* !__IBUILDRESPONSE_H__ */
