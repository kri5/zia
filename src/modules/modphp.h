#ifndef MODPHP_H
#define MODPHP_H

#include "API/IModule.h"
#include "API/ISendResponse.h"
#include "API/IBuildResponse.h"

class ModPHP : public zAPI::IModule, public zAPI::ISendResponse, public zAPI::IBuildResponse
{
    public:
        ModPHP();
        ~ModPHP();

        int                             getPriority(zAPI::IModule::Event event) const;
        zAPI::IModule::ChainStatus      onPreSend(zAPI::IHttpRequest*, zAPI::IHttpResponse*);
        size_t                          onProcessContent(zAPI::IHttpRequest*, zAPI::IHttpResponse*, char*,
                                        size_t, const std::vector<zAPI::ISendResponse*>&, unsigned int);
        zAPI::IModule::ChainStatus      onPostSend(zAPI::IHttpRequest*, zAPI::IHttpResponse*);

        zAPI::IModule::ChainStatus      onPreBuild(zAPI::IHttpRequest* request, zAPI::IHttpResponse* response);
        zAPI::IModule::ChainStatus      onPostBuild(zAPI::IHttpRequest* request, zAPI::IHttpResponse* response);
    private:
        char**                          createEnv(zAPI::IHttpRequest*) const;
};

#endif // MODPHP_H
