#include "helloworld.h"

helloworld::helloworld()
{

}

helloworld::~helloworld()
{

}

IModule::ChainStatus    helloworld::call(IModule::Event evt)
{
    if (evt == IModule::onServerStartEvent)
        this->onServerStart();
    else if (evt == IModule::onServerStopEvent)
        this->onServerStop();
	return IModule::CONTINUE;
}

IModule::ChainStatus     helloworld::onServerStart()
{
    std::cout << "Server seems to be starting" << std::endl;
    return IModule::CONTINUE;
}

IModule::ChainStatus     helloworld::onServerStop()
{
    std::cout << "Server seems to be stoping" << std::endl;
    return IModule::CONTINUE;
}

extern "C" IModule* create()
{
    return new helloworld;
}

extern "C" void destroy(IModule* i)
{
    delete i;
}

extern "C" std::string name()
{
    return "Hello world version 45";
}
