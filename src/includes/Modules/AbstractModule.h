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
        virtual zAPI::IModule::ChainStatus  call(zAPI::IModule::Event);
        virtual zAPI::IModule::ChainStatus  call(zAPI::IModule::Event, zAPI::IModuleInfo*);
        virtual zAPI::IClientSocket*        call(zAPI::IModule::Event, SOCKET);
        virtual zAPI::IModule::ChainStatus  call(zAPI::IModule::Event, const char*, size_t);
        virtual zAPI::IModule::ChainStatus  call(zAPI::IModule::Event, zAPI::IHttpRequest*, zAPI::IHttpResponse*);
        virtual size_t                      call(zAPI::IHttpRequest*, zAPI::IHttpResponse*, char*, size_t, IModule**, unsigned int);
};

#endif //ABSTRACTMODULE_H__
