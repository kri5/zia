#include "priorityTest2.h"

#include <iostream>

PriorityTest2::PriorityTest2()
{

}

PriorityTest2::~PriorityTest2()
{

}

zAPI::IModule::ChainStatus   PriorityTest2::onPreSend(zAPI::IHttpRequest* req,
                                            zAPI::IHttpResponse* res)
{
    return zAPI::IModule::CONTINUE;
}

zAPI::IModule::ChainStatus   PriorityTest2::onPostSend(zAPI::IHttpRequest* req,
                                            zAPI::IHttpResponse* res)
{
    return zAPI::IModule::CONTINUE;
}

size_t              PriorityTest2::onProcessContent(zAPI::IHttpRequest* req,
                                                    zAPI::IHttpResponse* res,
                                                    char* buf, size_t size,
                                                    const std::vector<zAPI::ISendResponse*>& tab,
                                                    unsigned int index)
{
    int     ret;

    std::cout << "priority 2 [onProcessContent] tab.size() == " << tab.size() << std::endl;
    if (tab.size() == index + 1)
    {
        std::cout << "module 2 is reading" << std::endl;
       ret = res->getCurrentStream()->read(buf, size);
    }
    else
    {
        ret = tab[index + 1]->onProcessContent(req, res, buf, size, tab, index + 1);
    }
    std::cout << "prio 2 [buf[0]] == [" << buf[0] << "]" << std::endl;
    for (int i = 0; i < size; ++i)
    {
        if (buf[i] == 'b')
        {
            buf[i] = 'c';
        }
    }
    return ret;
}

int                 PriorityTest2::getPriority(zAPI::IModule::Event event) const
{
    return 20;
}

extern "C" zAPI::IModule* create()
{
    return new PriorityTest2;
}

extern "C" void             destroy(zAPI::IModule* module)
{
    delete module;
}

extern "C" std::string      name()
{
    return "test priority 2";
}

extern "C" int              version()
{
    return 1;
}
