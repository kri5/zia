#ifndef __IWORKFLOW_H__
# define __IWORKFLOW_H__

#include "API/IModule.h"
#include "API/IHttpRequest.h"
#include "API/IHttpResponse.h"

class IWorkflow : public IModule
{
    public:
        virtual ~IWorkflow() {};
        virtual ChainStatus     onBegin(IHttpRequest*, IHttpResponse*) = 0;
        virtual ChainStatus     onEnd(IHttpRequest*, IHttpResponse*) = 0;
        virtual ChainStatus     onFailure(IHttpRequest*, IHttpResponse*) = 0;

};

#endif /* !__IWORKFLOW_H__ */
