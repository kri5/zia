#include <sstream>
#include <iostream>

#include "maintenance.h"
#include "Stream/ResponseStreamGeneric.h"

zAPI::IModule::ChainStatus      Maintenance::call(zAPI::IModule::Event event, zAPI::IHttpRequest* req, zAPI::IHttpResponse* res)
{
    if (event == zAPI::IModule::onBeginEvent)
        return (this->onBegin(req, res));
    else if (event == zAPI::IModule::onEndEvent)
        return (this->onEnd(req, res));
    else if (event == zAPI::IModule::onErrorEvent)
        return (this->onError(req, res));
    return zAPI::IModule::ERRORMODULE;
}

zAPI::IModule::ChainStatus      Maintenance::onBegin(zAPI::IHttpRequest* req, zAPI::IHttpResponse* res)
{
    res->setHeaderOption("Connection", "close"); //prevents from keep-alive.
    std::stringstream* str = new std::stringstream;
    *str << "Server is currently unavailable for maintenance. Please come back later";
    res->skipToSend(new ResponseStreamGeneric(str), true, false);
    res->setResponseStatus(503);
}

zAPI::IModule::ChainStatus      Maintenance::onEnd(zAPI::IHttpRequest*, zAPI::IHttpResponse*)
{
    return zAPI::IModule::CONTINUE;
}

zAPI::IModule::ChainStatus      Maintenance::onError(zAPI::IHttpRequest*, zAPI::IHttpResponse*)
{
    return zAPI::IModule::CONTINUE;
}

int                             Maintenance::getPriority(zAPI::IModule::Event) const
{
    return 100; //First priority whatever the event is.
}

extern "C" zAPI::IModule* create()
{
    return new Maintenance;
}

extern "C" void destroy(zAPI::IModule* i)
{
    delete i;
}

extern "C" std::string name()
{
    return "Maintenance module. Server is now unavailable to requests (503: service unavailable will be sent)";
}

extern "C" int  version()
{
    return 1;
}
