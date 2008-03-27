#include "Modules/ModuleManager.h"
#include <dlfcn.h>

ModuleManager::ModuleManager()
{
    _modules = new std::list<ModuleInfo*>[NumberOfHooks];
}

ModuleManager::~ModuleManager()
{
    for (std::list<ModuleInfo*>::iterator it = _moduleInstances.begin(); it != _moduleInstances.end(); ++it)
        delete *it;
    delete[] this->_modules;
}

bool            ModuleManager::load(std::string filename)
{
    // Loading the module
    void* handle = dlopen(filename.c_str(), RTLD_LAZY);
    if (!handle)
    {
        Logger::getInstance() << Logger::Error << dlerror() << '\n' << Logger::Flush;
        return false;
    }

    // If any of these is null, a symbol is missing.
    if (!dlsym(handle, "name") || !dlsym(handle, "create") || !dlsym(handle, "destroy"))
    {
        Logger::getInstance() << Logger::Error << "Can't load symbol: "
            << dlerror() << Logger::Flush;
        return false;
    }

    // Creating a ModuleInfo object.
    ModuleInfo* mi = new ModuleInfo(handle);
    this->_moduleInstances.push_back(mi);

    // Identifying which "event" class the module implement.
    IModule* ptr = mi->getInstance();
    if (dynamic_cast<IServerEvent*>(ptr))
        this->_modules[ServerEventHook].push_back(mi);
    if (dynamic_cast<IModuleEvent*>(ptr))
        this->_modules[ModuleEventHook].push_back(mi);
    if (dynamic_cast<IWorkflow*>(ptr))
        this->_modules[WorkflowHook].push_back(mi);
    if (dynamic_cast<INetwork*>(ptr))
        this->_modules[NetworkHook].push_back(mi);
    if (dynamic_cast<IReceiveRequest*>(ptr))
        this->_modules[ReceiveRequestHook].push_back(mi);
    if (dynamic_cast<IBuildResponse*>(ptr))
        this->_modules[BuildResponseHook].push_back(mi);
    if (dynamic_cast<ISendResponse*>(ptr))
        this->_modules[SendResponseHook].push_back(mi);

    Logger::getInstance() << Logger::Info << "Module " << mi->getName() << " loaded." << Logger::Flush;
    return true;
}
