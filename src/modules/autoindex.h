#ifndef AUTOINDEX_H__
# define AUTOINDEX_H__

#include "API/IModule.h"
#include "API/IBuildResponse.h"
#include "API/IHttpRequest.h"
#include "API/IHttpResponse.h"

class   AutoIndex : public zAPI::IModule, public zAPI::IBuildResponse
{
    IModule::ChainStatus    onPreBuild(zAPI::IHttpRequest* request, zAPI::IHttpResponse* response);
    IModule::ChainStatus    onPostBuild(zAPI::IHttpRequest* request, zAPI::IHttpResponse* response);
    int                     getPriority(zAPI::IModule::Event) const;
};

#endif

