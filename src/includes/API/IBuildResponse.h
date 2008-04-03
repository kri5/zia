#ifndef __IBUILDRESPONSE_H__
# define __IBUILDRESPONSE_H__

#include "API/IModule.h"
#include "API/IHttpRequest.h"
#include "API/IHttpResponse.h"

namespace zAPI
{
    /**
     *  This hook interface forces you to implement two event methods.
     *  The first one (onPreBuild) will be called before building the reponse header.
     *  The second one (onPostBuild) will be called after building the response header.
     *
     *  Both methods receive two parameters : a IHttpRequest* and a IHttpResponse*.
     */
    /// Interface used for response hook
    class IBuildResponse : public virtual IModule
    {
        public:

            /**
             *  The interface virtual destructor
             */
            virtual ~IBuildResponse(){};

            /**
             *  The preBuild event.
             *  This module method will be called before building the response header
             *  @param request a IHttpRequest
             *  @param reponse a IHttpResponse
             *  @return a ChainStatus
             */
            virtual ChainStatus     onPreBuild(IHttpRequest* request, IHttpResponse* response) = 0;

            /**
             *  The postBuild event.
             *  This method will be called after the reponse header has been built
             *  @param request a IHttpRequest
             *  @param response a IHttpResponse
             *  @return a ChainStatus
             */
            virtual ChainStatus     onPostBuild(IHttpRequest* request, IHttpResponse* response) = 0;
    };
}

#endif /* !__IBUILDRESPONSE_H__ */
