#include "HookTests.h"
#include <iostream>

HookTests::HookTests()
{

}

HookTests::~HookTests()
{

}

int                             HookTests::getPriority(zAPI::IModule::Event) const
{
    return 50;
}

zAPI::IModule::ChainStatus    HookTests::onPreBuild(zAPI::IHttpRequest*, zAPI::IHttpResponse*)
{
    ;
}

zAPI::IModule::ChainStatus    HookTests::onPostBuild(zAPI::IHttpRequest*, zAPI::IHttpResponse*)
{
    ;
}

zAPI::IClientSocket*          HookTests::onAccept(SOCKET)
{
    ;
}

zAPI::IModule::ChainStatus    HookTests::onReceive(char*, size_t)
{
    ;
}

zAPI::IModule::ChainStatus    HookTests::onSend(char*, size_t)
{
    ;
}

zAPI::IModule::ChainStatus    HookTests::onPreReceive(zAPI::IHttpRequest*, zAPI::IHttpResponse*)
{
    ;
}

zAPI::IModule::ChainStatus    HookTests::onPostReceive(zAPI::IHttpRequest*, zAPI::IHttpResponse*)
{
    ;
}

void                          HookTests::setInput(zAPI::IModule*)
{
    ;
}

zAPI::IModule::ChainStatus    HookTests::onPreSend(zAPI::IHttpRequest*, zAPI::IHttpResponse*)
{
    ;
}

size_t                        HookTests::onProcessContent(zAPI::IHttpRequest*, zAPI::IHttpResponse*, char*, size_t, IModule**, unsigned int)
{
    ;
}

zAPI::IModule::ChainStatus    HookTests::onPostSend(zAPI::IHttpRequest*, zAPI::IHttpResponse*)
{
    ;
}

zAPI::IModule::ChainStatus  HookTests::onServerStart()
{
    std::cout << "Server starting" << std::endl;
}

zAPI::IModule::ChainStatus  HookTests::onServerStop()
{
    std::cout << "Server stopping" << std::endl;
}

zAPI::IModule::ChainStatus  HookTests::onBegin(zAPI::IHttpRequest*, zAPI::IHttpResponse*)
{
    std::cout << "begging request worflow" << std::endl;
}

zAPI::IModule::ChainStatus  HookTests::onEnd(zAPI::IHttpRequest*, zAPI::IHttpResponse*)
{
    ;
}

zAPI::IModule::ChainStatus  HookTests::onFailure(zAPI::IHttpRequest*, zAPI::IHttpResponse*)
{
    ;
}

zAPI::IModule::ChainStatus  HookTests::onLoadModule(zAPI::IModuleInfo*)
{
    ;
}

zAPI::IModule::ChainStatus  HookTests::onUnloadModule(zAPI::IModuleInfo*)
{
    ;
}


extern "C" zAPI::IModule* create()
{
    return new HookTests;
}

extern "C" void destroy(zAPI::IModule* i)
{
    delete i;
}

extern "C" std::string name()
{
    return "HookTests";
}

extern "C" int  version()
{
    return 1;
}

