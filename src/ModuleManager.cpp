#include "Modules/ModuleManager.h"
#include "API/IDynLib.h"
#include "API/IModuleInfo.h"
#include "Modules/ModuleInfo.h"
#include "Modules/DynLib.h"
#include "RootConfig.hpp"
#include "FileSystem/IFileSystem.h"
#include "FileSystem/FileSystem.h"
#include "File/IFile.h"
#include <assert.h>

ModuleManager::ModuleManager()
{
    //_modules = new std::list<IModuleInfo*>[NumberOfHooks];
}

ModuleManager::~ModuleManager()
{
    for (std::list<RefCounter<IModuleInfo*> >::iterator it = _moduleInstances.begin(); it != _moduleInstances.end(); ++it)
        delete (*it).ptr;
    for (std::list<RefCounter<std::list<RefCounter<IModuleInfo*>*>*> >::iterator it = this->_modules.begin(); it != this->_modules.end(); ++it)
        delete[] (*it).ptr;
    delete[] this->_taskModulesList;
}

void            ModuleManager::init(unsigned int nbTasks)
{
    this->_taskModulesList = new RefCounter<std::list<RefCounter<IModuleInfo*>*>*>*[nbTasks];
}

//Will add a hook for a module. (on the new set of modules)
void            ModuleManager::pushModule(IModuleManager::Hook hook, RefCounter<IModuleInfo*>* mi)
{
    //ne pas s'endormir...
    //FIXME: je ne suis pas sur *DU TOUT* que le tableau indexe par hooks soit alloue a vrai dire ;/
    this->_modules.back().ptr[hook].push_back(mi);
    mi->ptr->addSupportedHook(hook);
    mi->count++;
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
    this->_moduleInstances.push_back(RefCounter<IModuleInfo*>(mi));
    RefCounter<IModuleInfo*>* refCountPtr = &(this->_moduleInstances.back()); //Haha ste loose y a qu'un seul niveau de template :'(

    // Identifying which "event" class the module implement.
    IModule* ptr = mi->getInstance();
    if (dynamic_cast<IServerEvent*>(ptr))
        this->pushModule(ServerEventHook, refCountPtr);
    if (dynamic_cast<IModuleEvent*>(ptr))
        this->pushModule(ModuleEventHook, refCountPtr);
    if (dynamic_cast<IWorkflow*>(ptr))
        this->pushModule(WorkflowHook, refCountPtr);
    if (dynamic_cast<INetwork*>(ptr))
        this->pushModule(NetworkHook, refCountPtr);
    if (dynamic_cast<IReceiveRequest*>(ptr))
        this->pushModule(ReceiveRequestHook, refCountPtr);
    if (dynamic_cast<IBuildResponse*>(ptr))
        this->pushModule(BuildResponseHook, refCountPtr);
    if (dynamic_cast<ISendResponse*>(ptr))
        this->pushModule(SendResponseHook, refCountPtr);

    Logger::getInstance() << Logger::Info << "Module " << mi->getName() << " version " << mi->getVersion() << " loaded." << Logger::Flush;
    ModuleManager::getInstance().call(IModuleManager::ModuleEventHook, IModule::onLoadModuleEvent, mi);
    return true;
}

void                    ModuleManager::removeFromHooks(IModuleInfo* mi)
{
    std::cout << "Removing from hooks" << std::endl;
    const std::vector<IModuleManager::Hook>& hooks = mi->getSupportedHooks();
    std::vector<IModuleManager::Hook>::const_iterator it = hooks.begin();
    std::vector<IModuleManager::Hook>::const_iterator ite = hooks.end();

    std::list<RefCounter<IModuleInfo*>*>::iterator       itMod;
    std::list<RefCounter<IModuleInfo*>*>::iterator       iteMod;

    for (; it != ite; ++it)
    {
        itMod = this->_modules.back().ptr[*it].begin();
        iteMod = this->_modules.back().ptr[*it].end();

        for (; itMod != iteMod; ++itMod)
        {
            if ((*itMod)->ptr == mi)
            {
                this->_modules.back().ptr[*it].erase(itMod);
                break ;
            }
        }
    }
}

void                    ModuleManager::unload(const std::string& filename)
{
    std::cout << "Unloading " << filename<< std::endl;
    std::list<RefCounter<IModuleInfo*> >::iterator it = this->_moduleInstances.begin();
    std::list<RefCounter<IModuleInfo*> >::iterator ite = this->_moduleInstances.end();

    for (; it != ite; ++it)
    {
        if ((*it).ptr->getFileName() == filename)
        {
            this->call(IModuleManager::ModuleEventHook, IModule::onUnloadModuleEvent, (*it).ptr);
            this->removeFromHooks((*it).ptr);
            return ;
        }
    }
    Logger::getInstance() << Logger::Warning << "No such module to unload" << Logger::Flush;
}

void                    ModuleManager::initProcessContent() const
{
    std::list<RefCounter<IModuleInfo*>*>::const_iterator         it = this->_modules.back().ptr[SendResponseHook].begin();
    std::list<RefCounter<IModuleInfo*>*>::const_iterator        ite = this->_modules.back().ptr[SendResponseHook].end();

    IModule*        mod;
    IModule*        prevMod = NULL;
    ISendResponse*  modRes;
    for (; it != ite; ++it)
    {
        mod = ((*it)->ptr)->getInstance();
        modRes = dynamic_cast<ISendResponse*>(mod);
        assert(modRes != NULL);
        modRes->setInput(prevMod);
        prevMod = mod;
    }
}

bool        ModuleManager::isLoaded(const std::string& fileName) const
{
    if (this->_modules.size() <= 0)
        return false;
    const std::list<RefCounter<IModuleInfo*>*>*           list = this->_modules.back().ptr;
    for (int i = 0; i < IModuleManager::NumberOfHooks; ++i)
    {
        std::list<RefCounter<IModuleInfo*>*>::const_iterator    it = list[i].begin();
        std::list<RefCounter<IModuleInfo*>*>::const_iterator    ite = list[i].end();

        for (; it != ite; ++it)
        {
            if (((*it)->ptr)->getFileName() == fileName)
                return true;
        }
    }
    return false;
}

void        ModuleManager::scanModuleDir()
{
    std::cout << "Scanning module dir" << std::endl;
    if (RootConfig::isSet("ModulesDir") == false)
    {
        Logger::getInstance() << Logger::Warning << "No modules dir specified. No module will be loaded" << Logger::Flush;
        return ;
    }
    //Retreiving modules in ModulesDir
    IFileSystem* fs = new FileSystem(RootConfig::getParamChar("ModulesDir"));
    std::list<IFile*>* files = fs->getFileList("so");
    //Creating a new modules list
    std::list<RefCounter<IModuleInfo*>*>* newList = new std::list<RefCounter<IModuleInfo*>*>[NumberOfHooks];
    //duplicating old list, to apply changes from the old one to the new one
    if (this->_modules.size() > 0)
    {
        for (int i = 0; i < IModuleManager::NumberOfHooks; ++i)
            newList[i] = this->_modules.back().ptr[i];
    }
    //flag to see if some changes occured.
    bool    firstChange = true;

    //files list iterators
    std::list<IFile*>::iterator it;
    std::list<IFile*>::iterator ite = files->end();

    //modules instance iterators
    
    std::list<RefCounter<IModuleInfo*> >::iterator mIt = this->_moduleInstances.begin();
    std::list<RefCounter<IModuleInfo*> >::iterator mIte = this->_moduleInstances.end();
     //First we look if some modules doesn't exists anymore.
    for (; mIt != mIte; ++mIt)
    {
        if (this->isLoaded((*mIt).ptr->getFileName()) == true)
        {
            if (firstChange)
            {
                this->_modules.push_back(newList);
                this->_modules.back().count = 0;
                firstChange = false;
            }
            Logger::getInstance() << Logger::Info << (*mIt).ptr->getFileName() << " is missing from ModulesDir. Unloading module." << Logger::Flush;
            this->unload((*mIt).ptr->getFileName());
        }
    }
    //Then we add new modules
    it = files->begin();
    for (; it != ite; ++it)
    {
        if (this->isLoaded((*it)->getFullFileName()) == false)
        {
            if (firstChange)
            {
                this->_modules.push_back(newList);
                this->_modules.back().count = 0;
                firstChange = false;
            }
            this->load((*it)->getFullFileName());
        }
    }
    if (firstChange == true)
        delete[] newList;
}

void        ModuleManager::removeModuleList(unsigned int reqId)
{
    std::cout << "Removing module list" << std::endl;
    std::list<RefCounter<IModuleInfo*>*>* list = this->_taskModulesList[reqId]->ptr;

    for (int i = 0; i < IModuleManager::NumberOfHooks; ++i)
    {
        std::list<RefCounter<IModuleInfo*>*>::iterator   it = list[i].begin();
        std::list<RefCounter<IModuleInfo*>*>::iterator   ite = list[i].end();

        for (; it != ite; ++it)
        {
            //Decrementing the number of references to a IModuleInfo
            (*it)->count--;
            if ((*it)->count == 0)
            {
                //FIXME: check if the iterator can't be passed, or if deleting the moduleInfo can't be 
                //enought considering no more ModuleList will be using it anymore
                this->unload((*it)->ptr->getFileName());
            }
        }

        std::list<RefCounter<std::list<RefCounter<IModuleInfo*>*>*> >::iterator        mIt = this->_modules.begin();
        std::list<RefCounter<std::list<RefCounter<IModuleInfo*>*>*> >::iterator        mIte = this->_modules.end();
        for (; mIt != mIte; ++it)
        {
            if (&(*mIt) == this->_taskModulesList[reqId])
            {
                this->_modules.erase(mIt);
            }
        }
    }
}

////////////////////////////////////////// ////////////////////////////////////////////
////////////////////////////////////////   ////////////////////////////////////////////
//////////////////////////////////////     ////////////////////////////////////////////
//////////////////////////////    CALL METHODS     ////////////////////////////////////
////////////////////////////////////       ////////////////////////////////////////////
/////////////////////////////////////   ///// Tomb's spinning top /////////////////////
///////////////////////////////////// /////////////////////////////////////////////////

size_t                  ModuleManager::processContent(IHttpRequest* req, IHttpResponse* res, char* buff, size_t size)
{
    if (this->_taskModulesList[req->getRequestId()]->ptr[SendResponseHook].size() > 0)
        return (this->_taskModulesList[req->getRequestId()]->ptr[SendResponseHook].back()->ptr->getInstance()->call
                (IModule::onProcessContentEvent, req, res, buff, size));
    res->getCurrentStream().read(buff, size);
    return res->getCurrentStream().gcount();
}

IModule::ChainStatus     ModuleManager::call(Hook hook, IModule::Event event)
{
    std::list<RefCounter<IModuleInfo*>*>::iterator          it = this->_modules.back().ptr[hook].begin();
    std::list<RefCounter<IModuleInfo*>*>::iterator          ite = this->_modules.back().ptr[hook].end();
    IModule::ChainStatus                                    res = IModule::CONTINUE;

    for (; it != ite; ++it)
    {
        res = (*it)->ptr->getInstance()->call(event);
        if (res != IModule::CONTINUE)
            break ;
    }
    return res;
}

IModule::ChainStatus     ModuleManager::call(Hook hook, IModule::Event event, IModuleInfo* mod)
{
    std::list<RefCounter<IModuleInfo*>*>::iterator          it = this->_modules.back().ptr[hook].begin();
    std::list<RefCounter<IModuleInfo*>*>::iterator          ite = this->_modules.back().ptr[hook].end();
    IModule::ChainStatus                                    res = IModule::CONTINUE;

    for (; it != ite; ++it)
    {
        res = (*it)->ptr->getInstance()->call(event, mod);
        if (res != IModule::CONTINUE)
            break ;
    }
    return res;
}

IClientSocket*  ModuleManager::call(Hook hook, IModule::Event event, SOCKET sock)
{
    size_t      size = this->_modules.back().ptr[hook].size();

    if (size == 0)
        return NULL;
    else if (size > 1)
        Logger::getInstance() << Logger::Warning << "Can't have more than one module hooked to accept(). Will be using the first one." << Logger::Flush;
    return (*(this->_modules.back().ptr[hook].begin()))->ptr->getInstance()->call(event, sock);
}

IModule::ChainStatus     ModuleManager::call(Hook hook, IModule::Event event, const char* buff, size_t size)
{
    std::list<RefCounter<IModuleInfo*>*>::iterator          it = this->_modules.back().ptr[hook].begin();
    std::list<RefCounter<IModuleInfo*>*>::iterator          ite = this->_modules.back().ptr[hook].end();
    IModule::ChainStatus                                    res = IModule::CONTINUE;

    for (; it != ite; ++it)
    {
        res = (*it)->ptr->getInstance()->call(event, buff, size);
        if (res != IModule::CONTINUE)
            break ;
    }
    return res;
}

IModule::ChainStatus     ModuleManager::call(Hook hook, IModule::Event event, IHttpRequest* httpReq, IHttpResponse* httpRes)
{
    std::list<RefCounter<IModuleInfo*>*>::iterator          it = this->_modules.back().ptr[hook].begin();
    std::list<RefCounter<IModuleInfo*>*>::iterator          ite = this->_modules.back().ptr[hook].end();
    IModule::ChainStatus                    res = IModule::CONTINUE;

    if (hook == IModuleManager::WorkflowHook) //setting pointer to current modules list
    {
        if (event == IModule::onBeginEvent)
        {
            this->_taskModulesList[httpReq->getRequestId()] = &(this->_modules.back());
            this->_taskModulesList[httpReq->getRequestId()]->count++;
            //std::cout << "Starting workflow : " << this->_taskModulesList[httpReq->getRequestId()]->count << std::endl;
            //std::cout << "request id == " << httpReq->getRequestId() << std::endl;
        }
        else //onError or onEnd : we decrement counter, and eventually remove modules list.
        {
            this->_taskModulesList[httpReq->getRequestId()]->count--;
            //std::cout << "Ending workflow : " << this->_taskModulesList[httpReq->getRequestId()]->count << std::endl;
            if (this->_taskModulesList[httpReq->getRequestId()]->count == 0 && 
                    this->_taskModulesList[httpReq->getRequestId()] != &(this->_modules.back()))
            {
                //std::cout << "Removing module list" << std::endl;
                this->removeModuleList(httpReq->getRequestId());
            }

        }
    }
    for (; it != ite; ++it)
    {
        res = (*it)->ptr->getInstance()->call(event, httpReq, httpRes);
        if (res != IModule::CONTINUE)
            break ;
    }
    return res;
}

