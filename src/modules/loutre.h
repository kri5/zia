#ifndef LOUTRE_H__
# define LOUTRE_H__

#include "API/IBuildResponse.h"
#include "Modules/AbstractModule.h"

class Loutre : public virtual AbstractModule, public zAPI::IBuildResponse
{
    public:
        Loutre();
        ~Loutre();
        zAPI::IModule::ChainStatus      call(Event, zAPI::IHttpRequest*, zAPI::IHttpResponse*);
        zAPI::IModule::ChainStatus      onPreBuild(zAPI::IHttpRequest*, zAPI::IHttpResponse*);
        zAPI::IModule::ChainStatus      onPostBuild(zAPI::IHttpRequest*, zAPI::IHttpResponse*);
};

#endif //LOUTRE_H__

