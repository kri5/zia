#ifndef ABSTRACTMODULE_H__
# define ABSTRACTMODULE_H__

#include "API/IModule.h"

class   AbstractModule : public IModule
{
    public:
        virtual ChainStatus     call(Event){return CONTINUE;}
        virtual ChainStatus     call(Event, IModule*){return CONTINUE;}
        virtual IClientSocket*  call(Event, SOCKET){return NULL;}
        virtual ChainStatus     call(Event, char*, size_t){return CONTINUE;}
        virtual ChainStatus     call(Event, IHttpRequest*, IHttpResponse*){return CONTINUE;}
};

#endif //ABSTRACTMODULE_H__
