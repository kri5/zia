#ifndef MAINTENANCE_H__
# define MAINTENANCE_H__

#include "Modules/AbstractModule.h"
#include "API/IWorkflow.h"

class   Maintenance : public AbstractModule, public zAPI::IWorkflow
{
    public:
        Maintenance(){}
        virtual ~Maintenance(){}
        zAPI::IModule::ChainStatus      call(zAPI::IModule::Event, zAPI::IHttpRequest*, zAPI::IHttpResponse*);
        zAPI::IModule::ChainStatus      onBegin(zAPI::IHttpRequest*, zAPI::IHttpResponse*);
        zAPI::IModule::ChainStatus      onEnd(zAPI::IHttpRequest*, zAPI::IHttpResponse*);
        zAPI::IModule::ChainStatus      onError(zAPI::IHttpRequest*, zAPI::IHttpResponse*);
        int                             getPriority(zAPI::IModule::Event) const;
};

#endif //MAINTENANCE_H__

