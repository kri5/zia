#ifndef __IBUILDRESPONSE_H__
# define __IBUILDRESPONSE_H__

#include "API/IModule.h"
#include "API/IHttpRequest.h"
#include "API/IHttpResponse.h"

class IBuildResponse : public IModule
{
    public:
        virtual ~IBuildResponse(){};
        virtual ChainStatus     onPostBuild(IHttpRequest&, IHttpResponse&) = 0;
};

#endif /* !__IBUILDRESPONSE_H__ */
