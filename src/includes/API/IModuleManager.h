#ifndef IMODULEMANAGER_H
# define IMODULEMANAGER_H

#include <iostream>
#include <string>
#include "API/IModule.h"
#include "API/IHttpRequest.h"
#include "API/IHttpResponse.h"
#include "API/IClientSocket.h"
#include "API/IModuleInfo.h"

namespace zAPI
{
    /// Will manage module, and fire events. This contains every modules instances.
    class   IModuleManager
    {
        public:
            virtual ~IModuleManager() {};
            virtual bool    load(const std::string& filename) = 0;
            virtual void    unload(const std::string& filename) = 0;

            virtual size_t  processContent(IHttpRequest*, IHttpResponse*, char*, size_t) = 0;

            virtual IModule::ChainStatus    call(IModule::Hook, IModule::Event) = 0;
            virtual IModule::ChainStatus    call(IModule::Hook, IModule::Event, IModuleInfo*) = 0;
            virtual IClientSocket*          call(IModule::Hook, IModule::Event, SOCKET) = 0;
            virtual IModule::ChainStatus    call(IModule::Hook, IModule::Event, const char*, size_t) = 0;
            virtual IModule::ChainStatus    call(IModule::Hook, IModule::Event, IHttpRequest*, IHttpResponse*) = 0;
    };
}

#endif // IMODULES_H

