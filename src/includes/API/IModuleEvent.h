#ifndef __IMODULEEVENT_H__
# define __IMODULEEVENT_H__

#include "API/IModuleInfo.h"
#include "API/IModule.h"

/**
 *  This hook interface forces you to implement two event methods.
 *  The first one (onLoadModule) will be called after a new module has been loaded.
 *  The second one (onUnload) will be called after a module has been unloaded.
 */
class IModuleEvent : public IModule
{
    public:
        /**
         *  The interface virtual destructor
         */
        virtual ~IModuleEvent() {};

        /**
         *  The loadModule event
         *  This method will be called after a module has been loaded
         *  @param module the module that has just been loaded
         *  @return a ChainStatus
         */
        virtual ChainStatus     onLoadModule(IModuleInfo* module) = 0;

        /**
         *  The unloadModule event
         *  This method will be called after a module has been unloaded
         *  @param module the module that just has been uloaded
         *  @return a ChainStatus
         */
        virtual ChainStatus     onUnloadModule(IModuleInfo* module) = 0;
};

#endif /* !__IMODULEEVENT_H__ */
