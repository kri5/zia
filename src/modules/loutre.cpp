#include "loutre.h"
#include "Stream/ResponseStreamGeneric.h"
#include <iostream>
#include <sstream>

Loutre::Loutre()
{
}

Loutre::~Loutre()
{
}

zAPI::IModule::ChainStatus      Loutre::call(Event event, zAPI::IHttpRequest* req, zAPI::IHttpResponse* res)
{
    if (event == onPreBuildEvent)
       return (this->onPreBuild(req, res));
    else if (event == onPostBuildEvent)
        return (this->onPostBuild(req, res));
}

zAPI::IModule::ChainStatus      Loutre::onPreBuild(zAPI::IHttpRequest* req, zAPI::IHttpResponse* res)
{
    std::stringstream*  str = new std::stringstream;
    zAPI::IResponseStream*    stream = new ResponseStreamGeneric(str);
    *str << "<h1>Les loutres vous aiment !</h1>";
    res->setHeaderOption("Loutre-Mode", "On");
    res->appendStream(stream);
    return CONTINUE;
}

zAPI::IModule::ChainStatus      Loutre::onPostBuild(zAPI::IHttpRequest* req, zAPI::IHttpResponse* res)
{
    return CONTINUE;
}

extern "C" zAPI::IModule* create()
{
    return new Loutre;
}

extern "C" void destroy(zAPI::IModule* i)
{
    delete i;
}

extern "C" std::string name()
{
    return "Loutromodule, mustelidae edition.";
}

extern "C" int  version()
{
    return 1;
}
