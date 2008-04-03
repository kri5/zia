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

class IModuleInfo;

/** 
 * Every interface that will be used by modules must inherit from this one.
 * Here we set the module entry point,  */
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
            onPostBuild,
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
         *  @param event the event being called
         *  @param request the current request
         *  @param response the current response
         *  @param buf a buffer containing some of the response content data
         *  @param length the buffer's length
         *  @return ChainStatus
         */
        virtual size_t                  call(Event event, IHttpRequest* request, IHttpResponse* reponse, char* buf, size_t length) = 0;

};

typedef IModule*    create_t();
typedef void        destroy_t(IModule*);
typedef std::string name_t();
typedef int         version_t();


#endif // IMODULE_H

