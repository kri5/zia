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
//#include "API/IModuleInfo.h"

/** 
 * Every interface that will be used by modules must inherit from this one.
 * Here we set the module entry point
 */
namespace zAPI
{
    class   IModuleInfo;

    /// Common interface for all the modules.
    class   IModule
    {
        public:

            /**
             * The interface virtual destructor 
             */
            virtual ~IModule() {};

            /**
             *  This enum defines the different behaviours of the workflow.
             *  Almost each hook method will return a ChainStatus that will 
             *  eventually affect the request processing
             */
            enum    ChainStatus
            {
                CONTINUE,         /**< Standard behaviour */  
                BREAK,            /**< No more module will be called for this event */  
                SKIPTONEXTHOOK,   /**< Skips to the next hook */  
                SKIPTONEXTEVENT,  /**< Skips to the next event */  
                STOP,             /**< Stops the workflow (assumes that the reponse has been sent by the module) */    
                ERRORMODULE       /**< Internal module error (will send a Http 500 error) */  
            };

            enum    Hook
            {
                ServerEventHook,
                ModuleEventHook,
                WorkflowHook,
                NetworkHook,
                ReceiveRequestHook,
                BuildResponseHook,
                SendResponseHook,
                NumberOfHooks
            };

            /**
             *  This enum defines the different events (contained in hooks)
             */
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
                onPreBuildEvent,
                onPostBuildEvent,
                onPreSendEvent,
                onProcessContentEvent,
                onPostSendEvent,
                onFailureEvent
            };

            /**
             *  Calls the right event method deppending on the event passed in parameter
             *  @param event the event being called
             *  @return ChainStatus
             */
            virtual IModule::ChainStatus    call(Event) = 0;

            /**
             *  Calls the right event method deppending on the event passed in parameter
             *  @param event the event being called
             *  @param moduleInfo a IModuleInfo
             *  @return ChainStatus
             */
            virtual IModule::ChainStatus    call(Event, IModuleInfo*) = 0;

            /**
             *  Calls the right event method deppending on the event passed in parameter
             *  @param event the event being called
             *  @param socket the currently used socket
             *  @return the new IClientSocket that will be used for this request
             */
            virtual IClientSocket*          call(Event, SOCKET) = 0;

            /**
             *  Calls the right event method deppending on the event passed in parameter
             *  @param event the event being called
             *  @param buf the buffer containing some of the response content data
             *  @param length the buffer's length
             *  @return ChainStatus
             */
            virtual IModule::ChainStatus    call(Event, const char*, size_t) = 0;

            /**
             *  Calls the right event method deppending on the event passed in parameter
             *  @param event the event being called
             *  @param request the current request
             *  @param response the current response
             *  @return ChainStatus
             */
            virtual IModule::ChainStatus    call(Event, IHttpRequest*, IHttpResponse*) = 0;

            /**
             *  Calls the right event method deppending on the event passed in parameter
             *  This one is only for calling processContent without breaking API's conception.
             *  @param request the current request
             *  @param response the current response
             *  @param buf a buffer containing some of the response content data
             *  @param length the buffer's length
             *  @param tab the IModule subscribe to processContent event
             *  @param index the current module in the IModule tab, to know which module to read in
             *  @return ChainStatus
             */
            virtual size_t                  call(IHttpRequest*, IHttpResponse*, char*, size_t, IModule**, unsigned int) = 0;

    };

    typedef IModule*    create_t();
    typedef void        destroy_t(IModule*);
    typedef std::string name_t();
    typedef int         version_t();
}

#endif // IMODULE_H

