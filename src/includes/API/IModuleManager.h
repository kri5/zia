#ifndef IMODULES_H
#define IMODULES_H

#include <iostream>
#include "API/IModule.h"
#include "API/IHttpRequest.h"
#include "API/IHttpResponse.h"
#include "Sockets/IClientSocket.h"

class   IModuleManager
{
    public:
        enum    Hook
        {
            ServerEventHook,
            ModuleEventHook,
            WorkflowHook,
            NetworkHook,
            ReceiveRequestHook,
            BuildResponseHook,
            SendResponseHook,
            NumberOfHooks
        };
        virtual ~IModuleManager() {};
        virtual bool    load(std::string filename) = 0;

        virtual IModule::ChainStatus    call(Hook, IModule::Event) = 0;
        virtual IModule::ChainStatus    call(Hook, IModule::Event, IModule*) = 0;
        virtual IClientSocket*          call(Hook, IModule::Event, SOCKET) = 0;
        virtual IModule::ChainStatus    call(Hook, IModule::Event, char*, size_t) = 0;
        virtual IModule::ChainStatus    call(Hook, IModule::Event, IHttpRequest*, IHttpResponse*) = 0;
};

#endif // IMODULES_H

