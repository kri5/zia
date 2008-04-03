#ifndef __IMODULEEVENT_H__
# define __IMODULEEVENT_H__

#include "API/IModuleInfo.h"
#include "API/IModule.h"

/// Interface to implement to hook a modules for modules evenement
class IModuleEvent : public IModule
{
    public:
        virtual ~IModuleEvent() {};
        virtual ChainStatus     onLoadModule(IModuleInfo*) = 0;
        virtual ChainStatus     onUnloadModule(IModuleInfo*) = 0;
};

#endif /* !__IMODULEEVENT_H__ */
