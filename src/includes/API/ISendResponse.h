#ifndef ISENDRESPONSE_H__
# define ISENDRESPONSE_H__

#include "IModule.h"
#include "IHttpRequest.h"
#include "IHttpResponse.h"

namespace zAPI
{
    /**
     *  This hook interface forces you to implements for event methods.
     *  <ul>
     *      <li>The first one (setInput) will be called to construct the module chain</li>
     *      <li>The second one (onPreSend) will be called before send any of the content data</li>
     *      <li>The third one (onProcessContent) will be called n times for processing the content to send</li>
     *      <li>The forth one (onPostSend) will be called after the entire content has been sent</li>
     *  </ul>
     */
    /// Base interface for modules who wants to hook on response sending events.
    class ISendResponse
    {
        public:

            /**
             *  The interface virtual destructor
             */
            virtual ~ISendResponse() {}

            /**
             *  This method will be called before sending the response content
             *  @param request the current request
             *  @param response the current response
             */
            virtual IModule::ChainStatus    onPreSend(IHttpRequest* request, IHttpResponse* response) = 0;

            /**
             *  This method will be called while sending the response body (also nowed as content).
             *  You'll have a std::vector of modules passed by the server, you will use this vector to read
             *  on the next module on the chain except when you're at the top of the chain. In that case, you'll
             *  have to read on the response stream (res->getCurrentStream->read(...)).
             *  @param request the current request
             *  @param response the current response
             *  @param buff the buffer
             *  @param buffSize the buffer size
             *  @param modules the modules vector
             *  @param index your place in the vector
             */
            virtual size_t                  onProcessContent(IHttpRequest* request, IHttpResponse* response, char* buff, size_t buffSize, 
                                                            const std::vector<zAPI::ISendResponse*>& modules, unsigned int index) = 0;

            /**
             *  This method will be called after all the response content has been sent
             *  @param request the current request
             *  @param response the current response
             */
            virtual IModule::ChainStatus    onPostSend(IHttpRequest* request, IHttpResponse* response) = 0;
    };
}

#endif /* !ISENDRESPONSE_H__ */


