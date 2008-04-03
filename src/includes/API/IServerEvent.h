#ifndef __ISERVEREVENT_H__
# define __ISERVEREVENT_H__

#include "API/IModule.h"

/// Base interface for modules who wants to hook on server events.
class IServerEvent : public IModule
{
    public:
        virtual ~IServerEvent() {};
        virtual ChainStatus     onServerStart() = 0;
        virtual ChainStatus     onServerStop() = 0;

};

#endif /* !__ISERVEREVENT_H__ */
