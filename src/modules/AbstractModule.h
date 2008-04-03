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
        virtual zAPI::IModule::ChainStatus      call(zAPI::IModule::Event){return zAPI::IModule::CONTINUE;}
        virtual zAPI::IModule::ChainStatus      call(zAPI::IModule::Event, zAPI::IModuleInfo*){return zAPI::IModule::CONTINUE;}
        virtual zAPI::IClientSocket*            call(zAPI::IModule::Event, SOCKET){return NULL;}
        virtual zAPI::IModule::ChainStatus      call(zAPI::IModule::Event, const char*, size_t){return zAPI::IModule::CONTINUE;}
        virtual zAPI::IModule::ChainStatus      call(zAPI::IModule::Event, zAPI::IHttpRequest*, zAPI::IHttpResponse*){return zAPI::IModule::CONTINUE;}
        virtual size_t                          call(zAPI::IModule::Event, zAPI::IHttpRequest*, zAPI::IHttpResponse*, char*, size_t){return 0;}
};

#endif //ABSTRACTMODULE_H__
