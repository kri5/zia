#include "priorityTest1.h"

#include <iostream>

PriorityTest1::PriorityTest1()
{

}

PriorityTest1::~PriorityTest1()
{

}

zAPI::IModule::ChainStatus   PriorityTest1::onPreSend(zAPI::IHttpRequest* req,
                                            zAPI::IHttpResponse* res)
{
    return zAPI::IModule::CONTINUE;
}

zAPI::IModule::ChainStatus   PriorityTest1::onPostSend(zAPI::IHttpRequest* req,
                                            zAPI::IHttpResponse* res)
{
    return zAPI::IModule::CONTINUE;
}

size_t              PriorityTest1::onProcessContent(zAPI::IHttpRequest* req,
                                                    zAPI::IHttpResponse* res,
                                                    char* buf, size_t size,
                                                    const std::vector<zAPI::ISendResponse*>& tab,
                                                    unsigned int index)
{
    int     ret;

    std::cout << "priority 1 [onProcessContent] tab.size() == " << tab.size() << std::endl;
    if (tab.size() == index + 1)
    {
        std::cout << "module 1 is reading" << std::endl;
       ret = res->getCurrentStream()->read(buf, size);
    }
    else
    {
        ret = tab[index + 1]->onProcessContent(req, res, buf, size, tab, index + 1);
    }
    std::cout << "prio 1 [buf[0]] == [" << buf[0] << "]" << std::endl;
    for (int i = 0; i < size; ++i)
    {
        if (buf[i] == 'a')
        {
            buf[i] = 'b';
        }
    }
    return ret;
}

int                 PriorityTest1::getPriority(zAPI::IModule::Event event) const
{
    return 20;
}

extern "C" zAPI::IModule* create()
{
    return new PriorityTest1;
}

extern "C" void             destroy(zAPI::IModule* module)
{
    delete module;
}

extern "C" std::string      name()
{
    return "test priority 1";
}

extern "C" int              version()
{
    return 1;
}
