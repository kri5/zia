#ifndef IMODULE_H
# define IMODULE_H

#ifndef WIN32
typedef int SOCKET;
#else
#pragma warning(disable: 4190) //disable extern "C" warnings
#endif

#include <string>

#include "IHttpRequest.h"
#include "IHttpResponse.h"
#include "IClientSocket.h"

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
            virtual ~IModule() {}

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
                onErrorEvent,
                onAcceptEvent,
                onReceiveEvent,
                onSendEvent,
                onPreReceiveEvent,
                onPostReceiveEvent,
                onPreBuildEvent,
                onPostBuildEvent,
                onPreSendEvent,
                onProcessContentEvent,
                onPostSendEvent
            };

            /**
             * Is called by the ModuleManager when the module is loaded to get the priority
             * of the event passed in parameter.
             * @param event the event to get priority from
             * @return a priority
             */
            virtual int                     getPriority(Event event) const = 0;

            /**
             *  Calls the right event method deppending on the event passed in parameter
             *  @param event the event being called
             *  @return ChainStatus
             */
            virtual IModule::ChainStatus    call(Event event) = 0;

            /**
             *  Calls the right event method deppending on the event passed in parameter
             *  @param event the event being called
             *  @param moduleInfo a IModuleInfo
             *  @return ChainStatus
             */
            virtual IModule::ChainStatus    call(Event event, IModuleInfo* moduleInfo) = 0;

            /**
             *  Calls the right event method deppending on the event passed in parameter
             *  @param event the event being called
             *  @param socket the currently used socket
             *  @return the new IClientSocket that will be used for this request
             */
            virtual IClientSocket*          call(Event event, SOCKET socket) = 0;

            /**
             *  Calls the right event method deppending on the event passed in parameter
             *  @param event the event being called
             *  @param buf the buffer containing some of the response content data
             *  @param length the buffer's length
             *  @return ChainStatus
             */
            virtual IModule::ChainStatus    call(Event event, const char* buf, size_t length) = 0;

            /**
             *  Calls the right event method deppending on the event passed in parameter
             *  @param event the event being called
             *  @param request the current request
             *  @param response the current response
             *  @return ChainStatus
             */
            virtual IModule::ChainStatus    call(Event event, IHttpRequest* request, IHttpResponse* response) = 0;

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
            virtual size_t                  call(IHttpRequest* request, IHttpResponse* response, char* buf, size_t length, IModule** tab, unsigned int index) = 0;

    };

    /*
     * This defines a function pointer to the right type.
     * This enhance code appearence by preventing you to cast a void* function pointer at every call
     */
    /// Defines a pointer to create() function, which return a IModule*
    typedef IModule*    create_t();
    /// Defines a pointer to destroy() function, which takes a IModule* as a param.
    typedef void        destroy_t(IModule*);
    /// Defines a pointer to name() function, which return the module name as a string
    typedef std::string name_t();
    /// define a pointer to version() function, which return the api's version the module is maid for, as an integer.
    typedef float       version_t();
}

#endif // IMODULE_H

