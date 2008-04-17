#ifndef IRECEIVEREQUEST_H__
# define IRECEIVEREQUEST_H__

#include "IModule.h"
#include "IHttpRequest.h"
#include "IHttpResponse.h"

namespace zAPI
{
    /**
     *  This hook interface forces you to implements two event methods.<br><br>
     *  <ul>
     *      <li>The first one (onPreReceive) will be called before receiving any data on the IClientSocket.</li>
     *      <li>The second one (onPostReceive) will be called after receiving the datas.</li>
     *  </ul>
     */
    /// Describe requirements for implementing IReceiveRequest in a module
    class IReceiveRequest
    {
        public:
            /**
             *  The interface virtual destructor
             */
            virtual ~IReceiveRequest() {}

            /**
             *  The preReceive event.<br>
             *  This method will be called just before receiving datas from the IClientSocket.
             *  @param request the IHttpRequest of the transaction
             *  @param response the IHttpResponse of the transaction
             *  @return a ChainStatus
             */
            virtual IModule::ChainStatus    onPreReceive(IHttpRequest* request, IHttpResponse* response) = 0;

            /**
             *  The postReceive event.<br>
             *  This method will be called just after receiving some datas from the IClientSocket.
             *  @param request the IHttpRequest of the transaction
             *  @param response the IHttpResponse of the transaction
             *  @return a ChainStatus
             */
            virtual IModule::ChainStatus    onPostReceive(IHttpRequest* request, IHttpResponse* response) = 0;
    };
}

#endif /* !__IRECEIVEREQUEST_H__ */
