#ifndef ABSTRACTMODULE_H__
# define ABSTRACTMODULE_H__

#include "API/IModule.h"
#include "API/IClientSocket.h"
#include "API/IHttpRequest.h"
#include "API/IHttpResponse.h"

template <typename T>
class   AbstractModule : public T
{
    public:
        virtual IModule::ChainStatus    call(IModule::Event){return IModule::CONTINUE;}
        virtual IModule::ChainStatus    call(IModule::Event, IModule*){return IModule::CONTINUE;}
        virtual IClientSocket*          call(IModule::Event, SOCKET){return NULL;}
        virtual IModule::ChainStatus    call(IModule::Event, char*, size_t){return IModule::CONTINUE;}
        virtual IModule::ChainStatus    call(IModule::Event, IHttpRequest*, IHttpResponse*){return IModule::CONTINUE;}
        virtual size_t                  call(IModule::Event, IHttpRequest*, IHttpResponse*, char*, size_t){return 0;}
};

#endif //ABSTRACTMODULE_H__
