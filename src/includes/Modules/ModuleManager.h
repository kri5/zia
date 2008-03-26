#ifndef MODULES_UNIX_H
#define MODULES_UNIX_H

#include <iostream>
#include <list>
#include <string>
#include "IModuleManager.h"
#include "API/IModule.h"
#include "Logger.hpp"
#include "Singleton.hpp"
#include "API/ModuleInfo.h"

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

    private:
        ModuleManager();
        virtual ~ModuleManager();
        std::list<ModuleInfo*>*        _modules;
        std::list<ModuleInfo*>         _moduleInstances;

        friend class Singleton<ModuleManager>;
};

#endif // MODULES_UNIX_H
