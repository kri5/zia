#ifndef IMODULEMANAGER_H
# define IMODULEMANAGER_H

#include <iostream>
#include <string>
#include "IModule.h"
#include "IHttpRequest.h"
#include "IHttpResponse.h"
#include "IClientSocket.h"
#include "IModuleInfo.h"

namespace zAPI
{
    /// Will manage module, and fire events. This contains every modules instances.
    class   IModuleManager
    {
        public:
            virtual ~IModuleManager() {}
            /**
             *  Will load a module
             *  @param filename The module's filename
             *  @return A boolean to check if module was properly loaded.
             */
            virtual bool    load(const std::string& filename) = 0;
            /**
             *  Will unload a module
             *  @param filename The module's filename
             */
            virtual void    unload(const std::string& filename) = 0;

            /**
             *  Will launch the chaining operation, which means : creating the IModule**,
             *  call the first module in the chain, and forwardin parameters to it.
             *  @params req The current request.
             *  @params res The current response
             *  @params buf The buffer beeing sent, and which modules may have to modify
             *  @params size The buffer's size (and also the size to read)
             */
            virtual size_t  processContent(IHttpRequest* req, IHttpResponse* res, char* buff, size_t size) = 0;

            /**
             *  Every call method is a entry point for calling modules methods.
             *  The only thing to do for the ModuleManager at this point, is to call
             *  modules registered for given Hook and Event, and forwarding arguments to
             *  module's call method.
             *
             *  Every call method, except the one for onAccept, which returns a IClientSocket*, return
             *  a ChainStatus. A ChainStatus is returned by every module's call method.
             *  This let you know if you should call next module or not.
             *  When every module has been called, of if one module stoped the chain, you should return
             *  a global ChainStatus.
             *  This value allows you to know if you should skip to another event
             *  another hook, or anything defined in IModule::ChainStatus documentation
             *
             *  The call method which returns a IClientSocket is a way to allow modules to instantiate a new ClientSocket
             *  such as a SSL one, which will be used everywhere else by the server, using some differents send/recv methods.
             */
            virtual IModule::ChainStatus    call(IModule::Hook, IModule::Event) = 0;
            virtual IModule::ChainStatus    call(IModule::Hook, IModule::Event, IModuleInfo*) = 0;
            virtual IClientSocket*          call(IModule::Hook, IModule::Event, SOCKET) = 0;
            virtual IModule::ChainStatus    call(IModule::Hook, IModule::Event, const char*, size_t) = 0;
            virtual IModule::ChainStatus    call(IModule::Hook, IModule::Event, IHttpRequest*, IHttpResponse*) = 0;
    };
}

#endif // IMODULEMANAGER_H

