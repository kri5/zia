#include "helloworld.h"

helloworld::helloworld()
{

}

helloworld::~helloworld()
{

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
