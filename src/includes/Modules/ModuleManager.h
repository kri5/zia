#ifndef MODULES_UNIX_H
#define MODULES_UNIX_H

#include <iostream>
#include <list>
#include <string>
#include "API/IModuleManager.h"
#include "API/IModule.h"
#include "API/IModuleInfo.h"
#include "Logger.hpp"
#include "Singleton.hpp"
#include "API/IModule.h"
#include "API/IHttpRequest.h"
#include "API/IHttpResponse.h"
#include "API/IClientSocket.h"

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
    template <typename T>
        struct      RefCounter
        {
            RefCounter(T _ptr) : ptr(_ptr), count(1){}
            T               ptr;
            unsigned int    count;
        };
    typedef std::list<RefCounter<IModuleInfo*> >     ModuleList;
    public:
        void                    init(unsigned int);
        bool                    load(const std::string& filename); 
        void                    unload(const std::string& filename); 
        void                    initProcessContent() const;
        void                    scanModuleDir();
        bool                    isLoaded(const std::string&) const;

        size_t                  processContent(IHttpRequest*, IHttpResponse*, char*, size_t);

        IModule::ChainStatus    call(Hook, IModule::Event);
        IModule::ChainStatus    call(Hook, IModule::Event, IModuleInfo*);
        IClientSocket*          call(Hook, IModule::Event, SOCKET);
        IModule::ChainStatus    call(Hook, IModule::Event, const char*, size_t);
        IModule::ChainStatus    call(Hook, IModule::Event, IHttpRequest*, IHttpResponse*);
    private:
        ModuleManager();
        virtual ~ModuleManager();

        void                    pushModule(IModuleManager::Hook, RefCounter<IModuleInfo*>*);
        void                    removeFromHooks(IModuleInfo*);
        void                    removeModuleList(unsigned int);

        std::list<RefCounter<std::list<RefCounter<IModuleInfo*>*>*> >   _modules;               // A list of a hook indexed array of IModuleInfo list
        std::list<RefCounter<IModuleInfo*> >                            _moduleInstances;       // Every instance of modules
        RefCounter<std::list<RefCounter<IModuleInfo*>*>*>**             _taskModulesList;       // taskId indexed array of a hook indexed array of ModuleInfo list

        friend class Singleton<ModuleManager>;
};

#endif // MODULES_UNIX_H
