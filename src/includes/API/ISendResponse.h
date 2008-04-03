#ifndef __ISENDRESPONSE_H__
# define __ISENDRESPONSE_H__

#include "API/IModule.h"
#include "API/IHttpRequest.h"
#include "API/IHttpResponse.h"

#include <fstream>

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
    class ISendResponse : public IModule
    {
        public:

            /**
             *  The interface virtual destructor
             */
            virtual ~ISendResponse() {};

            /**
             *  The setInput event.
             *  This method is kind of special because it aims to build a chain of module.
             *  on progress
             */
            virtual void    setInput(IModule*) = 0;

            virtual ChainStatus onPreSend(IHttpRequest*, IHttpResponse*) = 0;
            virtual size_t      onProcessContent(IHttpRequest*, IHttpResponse*, char*, size_t) = 0;
            virtual ChainStatus onPostSend(IHttpRequest*, IHttpResponse*) = 0;
    };
}

#endif /* !__ISENDRESPONSE_H__ */


