#ifndef MODPERL_H
#define MODPERL_H

#include "API/IModule.h"
#include "API/ISendResponse.h"
#include "API/IBuildResponse.h"

#include <EXTERN.h>
#include <perl.h>

class ModPerl : public zAPI::IModule, public zAPI::ISendResponse, public zAPI::IBuildResponse
{
    public:
        ModPerl();
        ~ModPerl();

        int                             getPriority(zAPI::IModule::Event event) const;
        zAPI::IModule::ChainStatus      onPreSend(zAPI::IHttpRequest*, zAPI::IHttpResponse*);
        size_t                          onProcessContent(zAPI::IHttpRequest*, zAPI::IHttpResponse*, char*,
                                        size_t, const std::vector<zAPI::ISendResponse*>&, unsigned int);
        zAPI::IModule::ChainStatus      onPostSend(zAPI::IHttpRequest*, zAPI::IHttpResponse*);

        zAPI::IModule::ChainStatus      onPreBuild(zAPI::IHttpRequest* request, zAPI::IHttpResponse* response);
        zAPI::IModule::ChainStatus      onPostBuild(zAPI::IHttpRequest* request, zAPI::IHttpResponse* response);
    private:
        void                            init();

        PerlInterpreter                 *_perlInterpreter;
};

#endif // MODPERL_H
