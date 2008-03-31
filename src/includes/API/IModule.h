#ifndef IMODULE_H
# define IMODULE_H

#ifndef WIN32
typedef int SOCKET;
#else
#pragma warning(disable: 4190) //disable extern "C" warnings
#endif

#include <string>

#include "API/IHttpRequest.h"
#include "API/IHttpResponse.h"
#include "API/IClientSocket.h"


/// Every interface that will be used by modules must inherit from this one.
/// Here we set the module entry point, the destroy symbol and some
/// informations used to correctly register the module.
class   IModule
{
    public:
        virtual ~IModule() {};

        enum    ChainStatus
        {
            CONTINUE,
            BREAK,
            SKIPTONEXTHOOK,
            SKIPTONEXTEVENT,
            STOP,
            ERRORMODULE
        };
        enum    Event
        {
            onServerStartEvent,
            onServerStopEvent,
            onLoadModuleEvent,
            onUnloadModuleEvent,
            onBeginEvent,
            onEndEvent,
            onAcceptEvent,
            onReceiveEvent,
            onSendEvent,
            onPreReceiveEvent,
            onPostReceiveEvent,
            onPostBuild,
            onPreSendEvent,
            onProcessContentEvent,
            onPostSendEvent
        };
        
        virtual IModule::ChainStatus    call(Event) = 0;
        virtual IModule::ChainStatus    call(Event, IModule*) = 0;
        virtual IClientSocket*          call(Event, SOCKET) = 0;
        virtual IModule::ChainStatus    call(Event, char*, size_t) = 0;
        virtual IModule::ChainStatus    call(Event, IHttpRequest*, IHttpResponse*) = 0;
        virtual size_t                  call(Event, IHttpRequest*, IHttpResponse*, char*, size_t) = 0;

};

typedef IModule*    create_t();
typedef void        destroy_t(IModule*);
typedef std::string name_t();
typedef int         version_t();


#endif // IMODULE_H

