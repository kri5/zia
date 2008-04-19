#ifndef IMODULE_H
# define IMODULE_H

#ifndef _WIN32
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
             * Priorites must be set in a range of 0 - 100 (0 being the first loaded module).
             * If a module doesn't use the specified event, the return value must be -1,
             * so the server can discard it.
             * @param event the event to get priority from
             * @return a priority
             */
            virtual int                     getPriority(Event event) const = 0;

    };

    /*
     * This defines a function pointer to the right type.
     * This enhance code appearence by preventing you to cast a void* function pointer at every call
     */
    /// Defines a function pointer for create(), which return a IModule*
    typedef IModule*    (*create_t)();
    /// Defines a function pointer for destroy(), which takes a IModule* as a param.
    typedef void        (*destroy_t)(IModule*);
    /// Defines a function pointer for name(), which return the module name as a string
    typedef std::string (*name_t)();
    /// define a function pointer for version(), which return the api's version the module is maid for, as an integer.
    typedef float       (*version_t)();
}

#endif // IMODULE_H


