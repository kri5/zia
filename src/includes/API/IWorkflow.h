#ifndef IWORKFLOW_H__
# define IWORKFLOW_H__

#include "API/IModule.h"
#include "API/IHttpRequest.h"
#include "API/IHttpResponse.h"

namespace zAPI
{
    /**
     *  This hook interfaces forces you to implement tree event methods.<br>
     *  <ul>
     *      <li>The first one (onBegin) will be called at the begining of a transaction</li>
     *      <li>The second one (onEnd) will be called at the end of the transaction</li>
     *      <li>The third one (onFailure) will be called if somethinf failed during the transaction</li>
     *  </ul>
     */
    /// Base interface for modules who wants to hook on main workflow events.
    class IWorkflow : public virtual IModule
    {
        public:
            /**
             *  The interface virtual destructor
             */
            virtual ~IWorkflow() {};

            /**
             *  The begin event.<br>
             *  This method will be called at the begining of the transaction 
             *  (after the accept event of the INetwork hook interface).
             *  @param request the IHttpRequest of the transaction
             *  @param response the IHttpResponse of the transaction
             *  @return a ChainStatus
             */
            virtual ChainStatus     onBegin(IHttpRequest* request, IHttpResponse* response) = 0;

            /**
             *  The en event.<br>
             *  This method will be called at the end of the transaction
             *  (when all the data has been sent and just before the end of the task)
             *  @param request the IHttpRequest of the transaction
             *  @param response the IHttpResponse of the transaction
             *  @return a ChainStatus
             */
            virtual ChainStatus     onEnd(IHttpRequest* request, IHttpResponse* response) = 0;

            /**
             *  The failure event.<br>
             *  This method will be called when a module failed to do his job and returned an ERRORMODULE ChainStatus.
             *  @param request the IHttpRequest of the transaction
             *  @param response the IHttpResponse of the transaction
             *  @return a ChainStatus
             */
            virtual ChainStatus     onFailure(IHttpRequest*, IHttpResponse*) = 0;
    };
}

#endif /* !IWORKFLOW_H__ */
