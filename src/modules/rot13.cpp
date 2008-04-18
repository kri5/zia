#include <iostream>

#include "rot13.h"
#include "Utils/Buffer.h"


Rot13::Rot13()
{
}

Rot13::~Rot13()
{
}

int                     Rot13::getPriority(zAPI::IModule::Event event) const
{
    switch (event)
    {
        case zAPI::IModule::onPreSendEvent:
            return 10;
        case zAPI::IModule::onProcessContentEvent:
            return 60;
        case zAPI::IModule::onPostSendEvent:
            return 4;
        default:
            return 10;
    }
}

zAPI::IModule::ChainStatus    Rot13::onPreSend(zAPI::IHttpRequest* req, zAPI::IHttpResponse* res)
{
    res->setHeaderOption("Encrypt", "Rot13");
	return zAPI::IModule::CONTINUE;
}

size_t                  Rot13::onProcessContent(zAPI::IHttpRequest* req, zAPI::IHttpResponse* res, 
                                char* buff, size_t size,
                                std::vector<zAPI::ISendResponse*> tab, unsigned int index)
{
    size_t  ret;
    if (tab.size() == index + 1)
    {
        ret = res->getCurrentStream()->read(buff, size);
    }
    else
        ret = tab[index + 1]->onProcessContent(req, res, buff, size, tab, index + 1);
    for (unsigned int i = 0; i < ret; ++i)
    {
        if ((buff[i] > 'a' && buff[i] < 'z') || (buff[i] > 'A' && buff[i] < 'Z'))
        {
            buff[i] += 13;
            if (buff[i] > 'z')
                buff[i] = 'a' + buff[i] - 'z';
            if (buff[i] > 'Z')
                buff[i] = 'A' + buff[i] - 'Z';
        }
    }
    return ret;
}

zAPI::IModule::ChainStatus    Rot13::onPostSend(zAPI::IHttpRequest* req, zAPI::IHttpResponse* res)
{
	return IModule::CONTINUE;
}

extern "C" zAPI::IModule* create()
{
    return new Rot13;
}

extern "C" void destroy(zAPI::IModule* i)
{
    delete i;
}

extern "C" std::string name()
{
    return "Rot13";
}

extern "C" int  version()
{
    return 1;
}
