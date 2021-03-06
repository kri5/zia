#ifndef MODULES_UNIX_H
#define MODULES_UNIX_H

#include <iostream>
#include <list>
#include <string>
#include "Logger.hpp"
#include "Singleton.hpp"
#include "Modules/IModuleManager.h"

// API
#include "API/IModule.h"
#include "API/IModuleInfo.h"
#include "API/IModule.h"
#include "API/IHttpRequest.h"
#include "API/IHttpResponse.h"
#include "API/IClientSocket.h"
#include "API/ISendResponse.h"


class   ModuleManager : public Singleton<ModuleManager>
{
    template <typename T>
        struct      RefCounter
        {
            RefCounter(T _ptr) : ptr(_ptr), count(1){}
            T               ptr;
            unsigned int    count;
        };

    struct  ModuleStuff
    {
        std::list<RefCounter<zAPI::IModuleInfo*>*>*         hooks;
        std::vector<zAPI::ISendResponse*>                   tab;
        std::list<zAPI::IModuleInfo*>*                      list;
    };
    typedef std::list<RefCounter<zAPI::IModuleInfo*> >     ModuleList;
    public:
        void                    init(unsigned int);
        bool                    load(const std::string& filename); 
        void                    unload(const std::string& filename); 
        void                    scanModuleDir();
        bool                    isLoaded(const std::string&) const;

        size_t                  processContent(zAPI::IHttpRequest*, zAPI::IHttpResponse*, char*, size_t);



        template<typename T>
        zAPI::IModule::ChainStatus    call(zAPI::IModule::Hook, zAPI::IModule::Event event, zAPI::IModule::ChainStatus (T::*method)())
        {
        
            if (this->_modules.size() == 0 || this->_modules.back().ptr.list == NULL)
                return zAPI::IModule::CONTINUE;

            std::list<zAPI::IModuleInfo*>&  nList = this->_modules.back().ptr.list[event];

            std::list<zAPI::IModuleInfo*>::iterator          it = nList.begin();
            std::list<zAPI::IModuleInfo*>::iterator          ite = nList.end();
            zAPI::IModule::ChainStatus                                    res = zAPI::IModule::CONTINUE;

            for (; it != ite; ++it)
            {
                //res = (*it)->ptr->getInstance()->call(event);
                res = (dynamic_cast<T*>((*it)->getInstance())->*method)();
                if (res != zAPI::IModule::CONTINUE)
                    break ;
            }
            return res;
        }

        template<typename T>
        zAPI::IModule::ChainStatus    call(zAPI::IModule::Hook, zAPI::IModule::Event event, zAPI::IModuleInfo* mod, zAPI::IModule::ChainStatus (T::*method)(zAPI::IModuleInfo*))
        {
            if (this->_modules.size() == 0 || this->_modules.back().ptr.list == NULL)
                return zAPI::IModule::CONTINUE;

            //CHECKME: je pense que le refcounter n'est pas bien initialise... ou quelquechose du genre.
            std::list<zAPI::IModuleInfo*>&  nList = this->_modules.back().ptr.list[event];

            std::list<zAPI::IModuleInfo*>::iterator         it = nList.begin();
            std::list<zAPI::IModuleInfo*>::iterator         ite = nList.end();
            zAPI::IModule::ChainStatus                      res = zAPI::IModule::CONTINUE;

            for (; it != ite; ++it)
            {
                res = (dynamic_cast<T*>((*it)->getInstance())->*method)(mod);
                if (res != zAPI::IModule::CONTINUE)
                    break ;
            }
            return res;
        }

        template <typename T>
        zAPI::IClientSocket*          call(zAPI::IModule::Hook hook, SOCKET sock, const std::string& address, int port, zAPI::IConfig* config, zAPI::IClientSocket* (T::*method)(SOCKET, const std::string&, int, zAPI::IConfig*))
        {
            if (this->_modules.size() == 0 || this->_modules.back().ptr.list == NULL)
                return NULL;

            size_t      size = this->_modules.back().ptr.hooks[hook].size();

            if (size == 0)
                return NULL;
            else if (size > 1)
                Logger::getInstance() << Logger::Warning << "Can't have more than one module hooked to accept(). Will be using the first one." << Logger::Flush;
            return (dynamic_cast<T*>((*(this->_modules.back().ptr.hooks[hook].begin()))->ptr->getInstance())->*method)(sock, address, port, config);
        }

        template<typename T>
        zAPI::IModule::ChainStatus    call(zAPI::IModule::Hook, zAPI::IModule::Event event, const char* buff, size_t size, zAPI::IModule::ChainStatus (T::*method)(const char*, size_t))
        {
            if (this->_modules.size() == 0 || this->_modules.back().ptr.list == NULL)
                return zAPI::IModule::CONTINUE;

            std::list<zAPI::IModuleInfo*>&  nList = this->_modules.back().ptr.list[event];

            std::list<zAPI::IModuleInfo*>::iterator         it = nList.begin();
            std::list<zAPI::IModuleInfo*>::iterator         ite = nList.end();
            zAPI::IModule::ChainStatus                      res = zAPI::IModule::CONTINUE;

            for (; it != ite; ++it)
            {
                res = (dynamic_cast<T*>((*it)->getInstance())->*method)(buff, size);
                if (res != zAPI::IModule::CONTINUE)
                    break ;
            }
            return res;
        }

        template<typename T>
        zAPI::IModule::ChainStatus    call( zAPI::IModule::Hook hook, zAPI::IModule::Event event, 
                                            zAPI::IHttpRequest* httpReq, zAPI::IHttpResponse* httpRes, 
                                            zAPI::IModule::ChainStatus (T::*method)(zAPI::IHttpRequest*, zAPI::IHttpResponse*))
        {
            if (this->_modules.size() == 0 || this->_modules.back().ptr.list == NULL)
                return zAPI::IModule::CONTINUE;

            std::list<zAPI::IModuleInfo*>&  nList = this->_modules.back().ptr.list[event];

            std::list<zAPI::IModuleInfo*>::iterator          it = nList.begin();
            std::list<zAPI::IModuleInfo*>::iterator          ite = nList.end();

            zAPI::IModule::ChainStatus                                    res = zAPI::IModule::CONTINUE;

            if (hook == zAPI::IModule::WorkflowHook) //setting pointer to current modules list
            {
                if (event == zAPI::IModule::onBeginEvent)
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
                    if (this->_taskModulesList[httpReq->getRequestId()]->count == 0 && //if no more request use that module list, 
                            this->_taskModulesList[httpReq->getRequestId()] != &(this->_modules.back())) //and if it's not the last one. If it is, we don't want to delete it.
                    {
                        //std::cout << "Removing module list" << std::endl;
                        this->removeModuleList(httpReq->getRequestId());
                    }

                }
            }
            for (; it != ite; ++it)
            {
                res = (dynamic_cast<T*>((*it)->getInstance())->*method)(httpReq, httpRes);
                if (res != zAPI::IModule::CONTINUE)
                    break ;
            }
            return res;
        }

    private:
        ModuleManager();
        virtual ~ModuleManager();

        void                    pushModule(zAPI::IModule::Hook, RefCounter<zAPI::IModuleInfo*>*);
        void                    removeFromHooks(zAPI::IModuleInfo*);
        void                    removeModuleList(unsigned int);
        void                    createModuleStuff();
        void                    getSortedList(zAPI::IModule::Hook hook, zAPI::IModule::Event event,
                                            std::list<zAPI::IModuleInfo*>&);
        void                    makeLists(ModuleStuff&);

        std::list<RefCounter<ModuleStuff> >   _modules;          // A list of a hook indexed array of IModuleInfo list
        std::list<RefCounter<zAPI::IModuleInfo*> >                            _moduleInstances;  // Every instance of modules
        RefCounter<ModuleStuff>**             _taskModulesList;  // taskId indexed array of a hook indexed array of ModuleInfo list

        friend class Singleton<ModuleManager>;
};

#endif // MODULES_UNIX_H
