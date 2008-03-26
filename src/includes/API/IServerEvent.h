#ifndef __ISERVEREVENT_H__
# define __ISERVEREVENT_H__

#include "IModule.h"

class IServerEvent : public IModule
{
    public:
        virtual ~IServerEvent() {};
        virtual ChainStatus     onServerStart() = 0;
        virtual ChainStatus     onServerStop() = 0;

};

#endif /* !__ISERVEREVENT_H__ */
