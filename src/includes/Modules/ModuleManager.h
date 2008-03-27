#ifndef MODULES_UNIX_H
#define MODULES_UNIX_H

#include <iostream>
#include <list>
#include <string>
#include "API/IModuleManager.h"
#include "API/IModule.h"
#include "Logger.hpp"
#include "Singleton.hpp"
#include "API/ModuleInfo.h"
#include "API/IModule.h"
#include "API/IHttpRequest.h"
#include "API/IHttpResponse.h"
#include "Sockets/IClientSocket.h"

// API
#include "API/IServerEvent.h"
#include "API/IModuleEvent.h"
#include "API/IWorkflow.h"
#include "API/INetwork.h"
#include "API/IReceiveRequest.h"
#include "API/IBuildResponse.h"
#include "API/ISendResponse.h"

class   ModuleManager : public IModuleManager, public Singleton<ModuleManager>
{
    public:
        bool                         load(std::string filename); 

        IModule::ChainStatus    call(Hook, IModule::Event);
        IModule::ChainStatus    call(Hook, IModule::Event, IModule*);
        IClientSocket*          call(Hook, IModule::Event, SOCKET);
        IModule::ChainStatus    call(Hook, IModule::Event, char*, size_t);
        IModule::ChainStatus    call(Hook, IModule::Event, IHttpRequest*, IHttpResponse*);
    private:
        ModuleManager();
        virtual ~ModuleManager();

        std::list<ModuleInfo*>*        _modules;
        std::list<ModuleInfo*>         _moduleInstances;

        friend class Singleton<ModuleManager>;
};

#endif // MODULES_UNIX_H
