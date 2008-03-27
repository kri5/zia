#ifndef __IMODULEEVENT_H__
# define __IMODULEEVENT_H__

#include "API/IModule.h"

class IModuleEvent : public IModule
{
    public:
        virtual ~IModuleEvent() {};
        virtual ChainStatus     onLoadModule(IModule*) = 0;
        virtual ChainStatus     onUnloadModule(IModule*) = 0;
};

#endif /* !__IMODULEEVENT_H__ */
