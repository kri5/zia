#include "Modules/ModuleManager.h"
#include "Modules/IDynLib.h"
#include "API/IModuleInfo.h"
#include "Modules/ModuleInfo.h"
#include "Modules/DynLib.h"
#include "RootConfig.hpp"
#include "FileSystem/IFileSystem.h"
#include "FileSystem/FileSystem.h"
#include "File/IFile.h"
#include <assert.h>

#include "API/IServerEvent.h"
#include "API/IModuleEvent.h"
#include "API/IWorkflow.h"
#include "API/INetwork.h"
#include "API/IReceiveRequest.h"
#include "API/IBuildResponse.h"
#include "API/ISendResponse.h"

ModuleManager::ModuleManager()
{
    //_modules = new std::list<IModuleInfo*>[NumberOfHooks];
}

ModuleManager::~ModuleManager()
{
    for (std::list<RefCounter<zAPI::IModuleInfo*> >::iterator it = _moduleInstances.begin(); it != _moduleInstances.end(); ++it)
        delete (*it).ptr;
    for (std::list<RefCounter<ModuleStuff> >::iterator it = this->_modules.begin(); it != this->_modules.end(); ++it)
        delete[] (*it).ptr.hooks;
    delete[] this->_taskModulesList;
}

void            ModuleManager::init(unsigned int nbTasks)
{
    this->_taskModulesList = new RefCounter<ModuleStuff>*[nbTasks];
    for (unsigned int i = 0; i < nbTasks; ++i)
        this->_taskModulesList[i] = NULL;
}

//Will add a hook for a module. (on the new set of modules)
void            ModuleManager::pushModule(zAPI::IModule::Hook hook, RefCounter<zAPI::IModuleInfo*>* mi)
{
    this->_modules.back().ptr.hooks[hook].push_back(mi);
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
    zAPI::IModuleInfo* mi = new ModuleInfo(library, filename);
    this->_moduleInstances.push_back(RefCounter<zAPI::IModuleInfo*>(mi));
    this->_moduleInstances.back().count = 0;
    RefCounter<zAPI::IModuleInfo*>* refCountPtr = &(this->_moduleInstances.back()); //Haha ste loose y a qu'un seul niveau de template :'(

    zAPI::IModule* ptr = mi->getInstance();
    // Checking if the module inherit from IModule.
    if (!dynamic_cast<zAPI::IModule*>(ptr))
    {
        delete ptr;
        delete library;
        Logger::getInstance() << Logger::Error << "Module does not inherit from IModule" << Logger::Flush; 
        return false;
    }

    // Identifying which "event" class the module implement.
    if (dynamic_cast<zAPI::IServerEvent*>(ptr))
        this->pushModule(zAPI::IModule::ServerEventHook, refCountPtr);
    if (dynamic_cast<zAPI::IModuleEvent*>(ptr))
        this->pushModule(zAPI::IModule::ModuleEventHook, refCountPtr);
    if (dynamic_cast<zAPI::IWorkflow*>(ptr))
        this->pushModule(zAPI::IModule::WorkflowHook, refCountPtr);
    if (dynamic_cast<zAPI::INetwork*>(ptr))
        this->pushModule(zAPI::IModule::NetworkHook, refCountPtr);
    if (dynamic_cast<zAPI::IReceiveRequest*>(ptr))
        this->pushModule(zAPI::IModule::ReceiveRequestHook, refCountPtr);
    if (dynamic_cast<zAPI::IBuildResponse*>(ptr))
        this->pushModule(zAPI::IModule::BuildResponseHook, refCountPtr);
    if (dynamic_cast<zAPI::ISendResponse*>(ptr))
        this->pushModule(zAPI::IModule::SendResponseHook, refCountPtr);

    Logger::getInstance() << Logger::Info << "Module " << mi->getName() << " version " << mi->getVersion() << " loaded." << Logger::Flush;
    ModuleManager::getInstance().call(zAPI::IModule::ModuleEventHook, zAPI::IModule::onLoadModuleEvent, mi, &zAPI::IModuleEvent::onLoadModule);
    return true;
}

void                    ModuleManager::removeFromHooks(zAPI::IModuleInfo* mi)
{
    std::cout << "Removing from hooks" << std::endl;
    const std::vector<zAPI::IModule::Hook>& hooks = mi->getSupportedHooks();
    std::vector<zAPI::IModule::Hook>::const_iterator it = hooks.begin();
    std::vector<zAPI::IModule::Hook>::const_iterator ite = hooks.end();

    std::list<RefCounter<zAPI::IModuleInfo*>*>::iterator       itMod;
    std::list<RefCounter<zAPI::IModuleInfo*>*>::iterator       iteMod;

    for (; it != ite; ++it)
    {
        itMod = this->_modules.back().ptr.hooks[*it].begin();
        iteMod = this->_modules.back().ptr.hooks[*it].end();

        for (; itMod != iteMod; ++itMod)
        {
            if ((*itMod)->ptr == mi)
            {
                (*itMod)->count--;
                this->_modules.back().ptr.hooks[*it].erase(itMod);
                break ;
            }
        }
    }
}

void                    ModuleManager::unload(const std::string& filename)
{
    std::cout << "Unloading " << filename<< std::endl;
    std::list<RefCounter<zAPI::IModuleInfo*> >::iterator it = this->_moduleInstances.begin();
    std::list<RefCounter<zAPI::IModuleInfo*> >::iterator ite = this->_moduleInstances.end();

    for (; it != ite; ++it)
    {
        if ((*it).ptr->getFileName() == filename)
        {
            this->call(zAPI::IModule::ModuleEventHook, zAPI::IModule::onUnloadModuleEvent, (*it).ptr, &zAPI::IModuleEvent::onUnloadModule);
            std::cout << "counting refs : " << (*it).count << std::endl;
            this->removeFromHooks((*it).ptr);
            std::cout << "counting refs : " << (*it).count << std::endl;
            return ;
        }
    }
    Logger::getInstance() << Logger::Warning << "No such module to unload" << Logger::Flush;
}

bool        ModuleManager::isLoaded(const std::string& fileName) const
{
    if (this->_modules.size() <= 0)
        return false;
    const std::list<RefCounter<zAPI::IModuleInfo*>*>*           list = this->_modules.back().ptr.hooks;
    for (int i = 0; i < zAPI::IModule::NumberOfHooks; ++i)
    {
        std::list<RefCounter<zAPI::IModuleInfo*>*>::const_iterator    it = list[i].begin();
        std::list<RefCounter<zAPI::IModuleInfo*>*>::const_iterator    ite = list[i].end();

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
    if (RootConfig::isSet("ModulesDir") == false)
    {
        Logger::getInstance() << Logger::Warning << "No modules dir specified. No module will be loaded" << Logger::Flush;
        return ;
    }
    //Retreiving modules in ModulesDir
    IFileSystem* fs = new FileSystem(RootConfig::getParamChar("ModulesDir"));
    std::list<IFile*>* files = fs->getFileList("so");
    if (files != NULL)
    {
        //Creating a new modules list
        ModuleStuff     newStuff;
        std::list<RefCounter<zAPI::IModuleInfo*>*>* newList = new std::list<RefCounter<zAPI::IModuleInfo*>*>[zAPI::IModule::NumberOfHooks];
        newStuff.hooks = newList;
        //duplicating old list, to apply changes from the old one to the new one
        if (this->_modules.size() > 0)
        {
            for (int i = 0; i < zAPI::IModule::NumberOfHooks; ++i)
                newList[i] = this->_modules.back().ptr.hooks[i];
        }
        //flag to see if some changes occured.
        bool    firstChange = true;

        //files list iterators
        std::list<IFile*>::iterator it;
        std::list<IFile*>::iterator ite = files->end();

        //modules instance iterators

        std::list<RefCounter<zAPI::IModuleInfo*> >::iterator mIt = this->_moduleInstances.begin();
        std::list<RefCounter<zAPI::IModuleInfo*> >::iterator mIte = this->_moduleInstances.end();
        //First we look if some modules doesn't exists anymore.
        for (; mIt != mIte; ++mIt)
        {
            for (it = files->begin(); it != ite; ++it)
            {
                if ((*it)->getFullFileName() == (*mIt).ptr->getFileName())
                    break;
            }
            if (it == ite) //module wasn't in dir anymore : unloading
            {
                if (firstChange)
                {
                    this->_modules.push_back(newStuff);
                    this->_modules.back().count = 0;
                    firstChange = false;
                }
                Logger::getInstance() << Logger::Info << (*mIt).ptr->getFileName() << " is missing from ModulesDir. Unloading module." << Logger::Flush;
                this->unload((*mIt).ptr->getFileName());
                if ((*mIt).count == 0)
                {
                    mIt = this->_moduleInstances.erase(mIt);
                    mIte = this->_moduleInstances.end();
                }
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
                    this->_modules.push_back(newStuff);
                    this->_modules.back().count = 0;
                    firstChange = false;
                }
                this->load((*it)->getFullFileName());
            }
        }
        if (firstChange == true)
            delete[] newList;
        else //if a new list has been added, we push our new processContent list :
        {
            std::list<RefCounter<zAPI::IModuleInfo*>*>  nList;
            this->getSortedList(zAPI::IModule::SendResponseHook, zAPI::IModule::onProcessContentEvent, nList);

            std::list<RefCounter<zAPI::IModuleInfo*>*>::reverse_iterator          it = nList.rbegin();
            std::list<RefCounter<zAPI::IModuleInfo*>*>::reverse_iterator          ite = nList.rend();
            for (; it != ite; ++it)
            {
                this->_modules.back().ptr.tab.push_back(dynamic_cast<zAPI::ISendResponse*>((*it)->ptr->getInstance()));
            }
        }
    }
    delete fs;
}

void        ModuleManager::removeModuleList(unsigned int reqId)
{
    std::list<RefCounter<zAPI::IModuleInfo*>*>* list = this->_taskModulesList[reqId]->ptr.hooks;

    for (int i = 0; i < zAPI::IModule::NumberOfHooks; ++i)
    {
        std::list<RefCounter<zAPI::IModuleInfo*>*>::iterator   it = list[i].begin();
        std::list<RefCounter<zAPI::IModuleInfo*>*>::iterator   ite = list[i].end();

        for (; it != ite; ++it)
        {
            //Decrementing the number of references to a zAPI::IModuleInfo
            (*it)->count--;
            if ((*it)->count == 0)
            {
                //FIXME: check if the iterator can't be passed, or if deleting the moduleInfo can't be 
                //enought considering no more ModuleList will be using it anymore
                this->unload((*it)->ptr->getFileName());
            }
        }

        std::list<RefCounter<ModuleStuff> >::iterator        mIt = this->_modules.begin();
        std::list<RefCounter<ModuleStuff> >::iterator        mIte = this->_modules.end();
        for (; mIt != mIte; ++it)
        {
            if (&(*mIt) == this->_taskModulesList[reqId])
            {
                delete[] (*mIt).ptr.hooks;
                this->_modules.erase(mIt);
            }
        }
    }
}

void                   ModuleManager::getSortedList(zAPI::IModule::Hook hook, zAPI::IModule::Event event,
                                                    std::list<RefCounter<zAPI::IModuleInfo*>*>& nList)
{
    std::list<RefCounter<zAPI::IModuleInfo*>*>::iterator  it = this->_modules.back().ptr.hooks[hook].begin();
    std::list<RefCounter<zAPI::IModuleInfo*>*>::iterator  ite = this->_modules.back().ptr.hooks[hook].end();

    for (; it != ite; ++it)
    {
        int prio = (*it)->ptr->getInstance()->getPriority(event);
        std::list<RefCounter<zAPI::IModuleInfo*>*>::iterator  nIt = nList.begin();
        std::list<RefCounter<zAPI::IModuleInfo*>*>::iterator  nIte = nList.end();
        bool    inserted = false;

        if (nIt == nIte)
            nList.push_back((*it));
        else
        {
            for (; nIt != nIte; ++nIt)
            {
                int nPrio = (*nIt)->ptr->getInstance()->getPriority(event);
                if (prio < nPrio)
                {
                    nList.insert(nIt, (*it));
                    inserted = true;
                    break;
                }
            }
            if (!inserted)
                nList.push_back((*it));
        }
    }
}

size_t                  ModuleManager::processContent(zAPI::IHttpRequest* req, zAPI::IHttpResponse* res, char* buff, size_t size)
{
    if (this->_taskModulesList[req->getRequestId()] == NULL)
        return res->getCurrentStream()->read(buff, size);

    if (this->_taskModulesList[req->getRequestId()]->ptr.hooks[zAPI::IModule::SendResponseHook].size() > 0)
    {
        std::vector<zAPI::ISendResponse*>& tab = this->_taskModulesList[req->getRequestId()]->ptr.tab;
        size_t ret = tab.front()->onProcessContent(req, res, buff, size, tab, 0);
        return ret;
    }
    return res->getCurrentStream()->read(buff, size);
}

