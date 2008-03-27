#ifndef IMODULE_H
#define IMODULE_H

#include <string>

#include "API/IHttpRequest.h"
#include "API/IHttpResponse.h"
#include "Sockets/IClientSocket.h"

#ifndef WIN32
    typedef int SOCKET;
#endif

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
            ERROR
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

        virtual ChainStatus     call(Event) = 0;
        virtual ChainStatus     call(Event, IModule*) = 0;
        virtual IClientSocket*  call(Event, SOCKET) = 0;
        virtual ChainStatus     call(Event, char*, size_t) = 0;
        virtual ChainStatus     call(Event, IHttpRequest*, IHttpResponse*) = 0;
        //FIXME: onProcessContent

};

typedef IModule*    create_t();
typedef void        destroy_t(IModule*);
typedef std::string name_t();

#endif // IMODULE_H
