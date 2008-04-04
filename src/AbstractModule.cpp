#include "Modules/AbstractModule.h"


AbstractModule::AbstractModule()
{
    ;
}

AbstractModule::~AbstractModule()
{
    ;
}

zAPI::IModule::ChainStatus    AbstractModule::call(zAPI::IModule::Event)
{
    return zAPI::IModule::CONTINUE;
}

zAPI::IModule::ChainStatus    AbstractModule::call(zAPI::IModule::Event, zAPI::IModuleInfo*)
{
    return zAPI::IModule::CONTINUE;
}

zAPI::IClientSocket*          AbstractModule::call(zAPI::IModule::Event, SOCKET)
{
    return NULL;
}

zAPI::IModule::ChainStatus    AbstractModule::call(zAPI::IModule::Event, const char*, size_t)
{
    return zAPI::IModule::CONTINUE;
}

zAPI::IModule::ChainStatus    AbstractModule::call(zAPI::IModule::Event, zAPI::IHttpRequest*, zAPI::IHttpResponse*)
{
    return zAPI::IModule::CONTINUE;
}

size_t                        AbstractModule::call(zAPI::IHttpRequest*, zAPI::IHttpResponse*, char*, size_t, IModule**, unsigned int)
{
    return zAPI::IModule::CONTINUE;
}

