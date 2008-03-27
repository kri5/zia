#include "Modules/ModuleManager.h"
#include <dlfcn.h>
#include <assert.h>

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

void                    ModuleManager::initProcessContent() const
{
    std::list<ModuleInfo*>::const_iterator        it = this->_modules[SendResponseHook].begin();
    std::list<ModuleInfo*>::const_iterator        ite = this->_modules[SendResponseHook].end();
    
    IModule*        mod;
    IModule*        prevMod = NULL;
    ISendResponse*  modRes;
    for (; it != ite; ++it)
    {
        mod = (*it)->getInstance();
        modRes = dynamic_cast<ISendResponse*>(mod);
        assert(modRes != NULL);
        modRes->setInput(prevMod);
        prevMod = mod;
    }
}

size_t                  ModuleManager::processContent(IHttpRequest* req, IHttpResponse* res, char* buff, size_t size)
{
    return (this->_modules[SendResponseHook].back()->getInstance()->call(IModule::onProcessContentEvent, req, res, buff, size));
}

IModule::ChainStatus     ModuleManager::call(Hook hook, IModule::Event event)
{
    std::list<ModuleInfo*>::iterator        it = this->_modules[hook].begin();
    std::list<ModuleInfo*>::iterator        ite = this->_modules[hook].end();
    IModule::ChainStatus                    res;

    for (; it != ite; ++it)
    {
        res = (*it)->getInstance()->call(event);
        if (res != IModule::CONTINUE)
            break ;
    }
    return res;
}

IModule::ChainStatus     ModuleManager::call(Hook hook, IModule::Event event, IModule* mod)
{
    std::list<ModuleInfo*>::iterator        it = this->_modules[hook].begin();
    std::list<ModuleInfo*>::iterator        ite = this->_modules[hook].end();
    IModule::ChainStatus                    res;

    for (; it != ite; ++it)
    {
        res = (*it)->getInstance()->call(event, mod);
        if (res != IModule::CONTINUE)
            break ;
    }
    return res;
}

IClientSocket*  ModuleManager::call(Hook hook, IModule::Event event, SOCKET sock)
{
    size_t      size = this->_modules[hook].size();

    if (size == 0)
        return NULL;
    else if (size > 1)
        Logger::getInstance() << Logger::Warning << "Can't have more than one module hooked to accept(). Will be using the first one." << Logger::Flush;
    return (*(this->_modules[hook].begin()))->getInstance()->call(event, sock);
}

IModule::ChainStatus     ModuleManager::call(Hook hook, IModule::Event event, char* buff, size_t size)
{
    std::list<ModuleInfo*>::iterator        it = this->_modules[hook].begin();
    std::list<ModuleInfo*>::iterator        ite = this->_modules[hook].end();
    IModule::ChainStatus                    res;

    for (; it != ite; ++it)
    {
        res = (*it)->getInstance()->call(event, buff, size);
        if (res != IModule::CONTINUE)
            break ;
    }
    return res;
}

IModule::ChainStatus     ModuleManager::call(Hook hook, IModule::Event event, IHttpRequest* httpReq, IHttpResponse* httpRes)
{
    std::list<ModuleInfo*>::iterator        it = this->_modules[hook].begin();
    std::list<ModuleInfo*>::iterator        ite = this->_modules[hook].end();
    IModule::ChainStatus                    res;

    for (; it != ite; ++it)
    {
        res = (*it)->getInstance()->call(event, httpReq, httpRes);
        if (res != IModule::CONTINUE)
            break ;
    }
    return res;
}

