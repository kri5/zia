#ifndef MODULES_UNIX_H
#define MODULES_UNIX_H

#include <iostream>
#include <list>
#include <string>
#include "Logger.hpp"
#include "Singleton.hpp"

// API
#include "API/IModuleManager.h"
#include "API/IModule.h"
#include "API/IModuleInfo.h"
#include "API/IModule.h"
#include "API/IHttpRequest.h"
#include "API/IHttpResponse.h"
#include "API/IClientSocket.h"


class   ModuleManager : public zAPI::IModuleManager, public Singleton<ModuleManager>
{
    template <typename T>
        struct      RefCounter
        {
            RefCounter(T _ptr) : ptr(_ptr), count(1){}
            T               ptr;
            unsigned int    count;
        };
    typedef std::list<RefCounter<zAPI::IModuleInfo*> >     ModuleList;
    public:
        void                    init(unsigned int);
        bool                    load(const std::string& filename); 
        void                    unload(const std::string& filename); 
        void                    scanModuleDir();
        bool                    isLoaded(const std::string&) const;

        size_t                  processContent(zAPI::IHttpRequest*, zAPI::IHttpResponse*, char*, size_t);

        zAPI::IModule::ChainStatus    call(zAPI::IModule::Hook, zAPI::IModule::Event);
        zAPI::IModule::ChainStatus    call(zAPI::IModule::Hook, zAPI::IModule::Event, zAPI::IModuleInfo*);
        zAPI::IClientSocket*          call(zAPI::IModule::Hook, zAPI::IModule::Event, SOCKET);
        zAPI::IModule::ChainStatus    call(zAPI::IModule::Hook, zAPI::IModule::Event, const char*, size_t);
        zAPI::IModule::ChainStatus    call(zAPI::IModule::Hook, zAPI::IModule::Event, zAPI::IHttpRequest*, zAPI::IHttpResponse*);
    private:
        ModuleManager();
        virtual ~ModuleManager();

        void                    pushModule(zAPI::IModule::Hook, RefCounter<zAPI::IModuleInfo*>*);
        void                    removeFromHooks(zAPI::IModuleInfo*);
        void                    removeModuleList(unsigned int);

        std::list<RefCounter<std::list<RefCounter<zAPI::IModuleInfo*>*>*> >   _modules;          // A list of a hook indexed array of IModuleInfo list
        std::list<RefCounter<zAPI::IModuleInfo*> >                            _moduleInstances;  // Every instance of modules
        RefCounter<std::list<RefCounter<zAPI::IModuleInfo*>*>*>**             _taskModulesList;  // taskId indexed array of a hook indexed array of ModuleInfo list

        friend class Singleton<ModuleManager>;
};

#endif // MODULES_UNIX_H
