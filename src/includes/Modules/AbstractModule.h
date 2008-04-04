#ifndef ABSTRACTMODULE_H__
# define ABSTRACTMODULE_H__

#include "API/IModule.h"
#include "API/IClientSocket.h"
#include "API/IHttpRequest.h"
#include "API/IHttpResponse.h"

class   AbstractModule : public virtual zAPI::IModule
{
    public:
        AbstractModule();
        virtual ~AbstractModule();
        zAPI::IModule::ChainStatus  call(zAPI::IModule::Event);
        zAPI::IModule::ChainStatus  call(zAPI::IModule::Event, zAPI::IModuleInfo*);
        zAPI::IClientSocket*        call(zAPI::IModule::Event, SOCKET);
        zAPI::IModule::ChainStatus  call(zAPI::IModule::Event, const char*, size_t);
        zAPI::IModule::ChainStatus  call(zAPI::IModule::Event, zAPI::IHttpRequest*, zAPI::IHttpResponse*);
        size_t                      call(zAPI::IModule::Event, zAPI::IHttpRequest*, zAPI::IHttpResponse*, char*, size_t);
};

#endif //ABSTRACTMODULE_H__
