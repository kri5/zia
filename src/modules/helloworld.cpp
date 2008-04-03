#include "helloworld.h"

helloworld::helloworld()
{

}

helloworld::~helloworld()
{

}

zAPI::IModule::ChainStatus    helloworld::call(zAPI::IModule::Event evt)
{
    if (evt == IModule::onServerStartEvent)
        this->onServerStart();
    else if (evt == IModule::onServerStopEvent)
        this->onServerStop();
	return IModule::CONTINUE;
}

zAPI::IModule::ChainStatus     helloworld::onServerStart()
{
    std::cout << "Server seems to be starting" << std::endl;
    return IModule::CONTINUE;
}

zAPI::IModule::ChainStatus     helloworld::onServerStop()
{
    std::cout << "Server seems to be stoping" << std::endl;
    return IModule::CONTINUE;
}

extern "C" zAPI::IModule* create()
{
    return new helloworld;
}

extern "C" void destroy(zAPI::IModule* i)
{
    delete i;
}

extern "C" std::string name()
{
    return "Hello world";
}

extern "C" int  version()
{
    return 42;
}
