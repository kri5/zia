#ifndef MODULES_UNIX_H
#define MODULES_UNIX_H

#include <iostream>
#include <list>
#include <string>
#include "IModuleManager.h"
#include "API/IModule.h"
#include "Logger.hpp"
#include "Singleton.hpp"

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
       virtual bool        load(std::string filename); 

    private:
        ModuleManager();
        virtual ~ModuleManager();
        std::list<IModule*>   _modules;

        friend class Singleton<ModuleManager>;
};

#endif // MODULES_UNIX_H
