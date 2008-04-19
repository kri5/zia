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

            virtual IModule::ChainStatus    onPreSend(IHttpRequest*, IHttpResponse*) = 0;
            virtual size_t                  onProcessContent(IHttpRequest* req, IHttpResponse* res, char* buff, size_t buffSize, const std::vector<zAPI::ISendResponse*>& modules, unsigned int index) = 0;
            virtual IModule::ChainStatus    onPostSend(IHttpRequest*, IHttpResponse*) = 0;
    };
}

#endif /* !ISENDRESPONSE_H__ */


