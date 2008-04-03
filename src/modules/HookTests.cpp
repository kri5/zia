#include "HookTests.h"


extern "C" IModule* create()
{
    return new HookTests;
}

extern "C" void destroy(IModule* i)
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

HookTests::HookTests()
{

}

HookTests::~HookTests()
{

}
