#include "Modules/ModuleManager.h"
#include "API/IDynLib.h"
#include "API/IModuleInfo.h"
#include "Modules/ModuleInfo.h"
#include "Modules/DynLib.h"
#include <assert.h>

ModuleManager::ModuleManager()
{
    _modules = new std::list<IModuleInfo*>[NumberOfHooks];
}

ModuleManager::~ModuleManager()
{
    for (std::list<IModuleInfo*>::iterator it = _moduleInstances.begin(); it != _moduleInstances.end(); ++it)
        delete *it;
    delete[] this->_modules;
}

void            ModuleManager::pushModule(IModuleManager::Hook hook, IModuleInfo* mi)
{
    this->_modules[hook].push_back(mi);
    mi->addSupportedHook(hook);
}

bool            ModuleManager::load(const std::string& filename)
{
    IDynLib* library = new DynLib();

    // Loading the module
    if (!library->load(filename))
    {
		Logger::getInstance() << Logger::Error << "Loading module " << filename << ": " << library->lastError() << Logger::Flush;
         return false;
    }

    // If any of these is null, a symbol is missing.
    if (!library->sym("name") || !library->sym("create") || !library->sym("destroy") || !library->sym("version"))
    {
        Logger::getInstance() << Logger::Error << "Can't load symbol: " 
            << library->lastError() << Logger::Flush;
        delete library;
        return false;
    }

    // Creating a ModuleInfo object.
    IModuleInfo* mi = new ModuleInfo(library, filename);
    this->_moduleInstances.push_back(mi);

    // Identifying which "event" class the module implement.
    IModule* ptr = mi->getInstance();
    if (dynamic_cast<IServerEvent*>(ptr))
        this->pushModule(ServerEventHook, mi);
    if (dynamic_cast<IModuleEvent*>(ptr))
        this->pushModule(ModuleEventHook, mi);
    if (dynamic_cast<IWorkflow*>(ptr))
        this->pushModule(WorkflowHook, mi);
    if (dynamic_cast<INetwork*>(ptr))
        this->pushModule(NetworkHook, mi);
    if (dynamic_cast<IReceiveRequest*>(ptr))
        this->pushModule(ReceiveRequestHook, mi);
    if (dynamic_cast<IBuildResponse*>(ptr))
        this->pushModule(BuildResponseHook, mi);
    if (dynamic_cast<ISendResponse*>(ptr))
        this->pushModule(SendResponseHook, mi);

    Logger::getInstance() << Logger::Info << "Module " << mi->getName() << " version " << mi->getVersion() << " loaded." << Logger::Flush;
    ModuleManager::getInstance().call(IModuleManager::ModuleEventHook, IModule::onLoadModuleEvent, mi);
    return true;
}

void                    ModuleManager::removeFromHooks(IModuleInfo* mi)
{
    const std::vector<IModuleManager::Hook>& hooks = mi->getSupportedHooks();
    std::vector<IModuleManager::Hook>::const_iterator it = hooks.begin();
    std::vector<IModuleManager::Hook>::const_iterator ite = hooks.end();

    std::list<IModuleInfo*>::iterator       itMod;
    std::list<IModuleInfo*>::iterator       iteMod;

    for (; it != ite; ++it)
    {
        itMod = this->_modules[*it].begin();
        iteMod = this->_modules[*it].end();

        for (; itMod != iteMod; ++itMod)
        {
            if (*itMod == mi)
            {
                this->_modules[*it].erase(itMod);
                break ;
            }
        }
    }
}

void                    ModuleManager::unload(const std::string& filename)
{
    std::list<IModuleInfo*>::iterator   it = this->_moduleInstances.begin();
    std::list<IModuleInfo*>::iterator   ite = this->_moduleInstances.end();

    for (; it != ite; ++it)
    {
        if ((*it)->getFileName() == filename)
        {
            this->call(IModuleManager::ModuleEventHook, IModule::onUnloadModuleEvent, *it);
            this->removeFromHooks((*it));
            return ;
        }
    }
    Logger::getInstance() << Logger::Warning << "No such module to unload" << Logger::Flush;
}

void                    ModuleManager::initProcessContent() const
{
    std::list<IModuleInfo*>::const_iterator        it = this->_modules[SendResponseHook].begin();
    std::list<IModuleInfo*>::const_iterator        ite = this->_modules[SendResponseHook].end();
    
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
    if (this->_modules[SendResponseHook].size() > 0)
        return (this->_modules[SendResponseHook].back()->getInstance()->call(IModule::onProcessContentEvent, req, res, buff, size));
    res->getCurrentStream().read(buff, size);
    return res->getCurrentStream().gcount();
}

IModule::ChainStatus     ModuleManager::call(Hook hook, IModule::Event event)
{
    std::list<IModuleInfo*>::iterator        it = this->_modules[hook].begin();
    std::list<IModuleInfo*>::iterator        ite = this->_modules[hook].end();
    IModule::ChainStatus                    res = IModule::CONTINUE;

    for (; it != ite; ++it)
    {
        res = (*it)->getInstance()->call(event);
        if (res != IModule::CONTINUE)
            break ;
    }
    return res;
}

IModule::ChainStatus     ModuleManager::call(Hook hook, IModule::Event event, IModuleInfo* mod)
{
    std::list<IModuleInfo*>::iterator        it = this->_modules[hook].begin();
    std::list<IModuleInfo*>::iterator        ite = this->_modules[hook].end();
    IModule::ChainStatus                    res = IModule::CONTINUE;

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

IModule::ChainStatus     ModuleManager::call(Hook hook, IModule::Event event, const char* buff, size_t size)
{
    std::list<IModuleInfo*>::iterator        it = this->_modules[hook].begin();
    std::list<IModuleInfo*>::iterator        ite = this->_modules[hook].end();
    IModule::ChainStatus                    res = IModule::CONTINUE;

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
    std::list<IModuleInfo*>::iterator        it = this->_modules[hook].begin();
    std::list<IModuleInfo*>::iterator        ite = this->_modules[hook].end();
    IModule::ChainStatus                    res = IModule::CONTINUE;

    for (; it != ite; ++it)
    {
        res = (*it)->getInstance()->call(event, httpReq, httpRes);
        if (res != IModule::CONTINUE)
            break ;
    }
    return res;
}

