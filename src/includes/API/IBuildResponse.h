#ifndef __IBUILDRESPONSE_H__
# define __IBUILDRESPONSE_H__

#include "IModule.h"
#include "IHttpRequest.h"
#include "IHttpResponse.h"

class IBuildResponse : public IModule
{
    public:
        virtual ~IBuildResponse(){};
        virtual ChainStatus     postBuild(IHttpRequest&, IHttpResponse&) = 0;
};

#endif /* !__IBUILDRESPONSE_H__ */
