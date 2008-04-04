#include <iostream>

#include "rot13.h"
#include "Utils/Buffer.h"


Rot13::Rot13()
{
}

Rot13::~Rot13()
{
}

size_t                  Rot13::call(zAPI::IHttpRequest* req, zAPI::IHttpResponse* res, char* buff, size_t size, IModule** tab, unsigned int index)
{
    return this->onProcessContent(req, res, buff, size, tab, index);
}

zAPI::IModule::ChainStatus    Rot13::call(zAPI::IModule::Event event, zAPI::IHttpRequest* req, zAPI::IHttpResponse* res)
{
    if (event == zAPI::IModule::onPreSendEvent)
        return (this->onPreSend(req, res));
    else if (event == zAPI::IModule::onPostSendEvent)
        return (this->onPostSend(req, res));
	return zAPI::IModule::ERRORMODULE;
}

zAPI::IModule::ChainStatus    Rot13::onPreSend(zAPI::IHttpRequest* req, zAPI::IHttpResponse* res)
{
    res->setHeaderOption("Encrypt", "Rot13");
	return zAPI::IModule::CONTINUE;
}

size_t                  Rot13::onProcessContent(zAPI::IHttpRequest* req, zAPI::IHttpResponse* res, 
                                char* buff, size_t size,
                                IModule** tab, unsigned int index)
{
    size_t  ret;
    if (tab[index + 1] == NULL)
    {
        res->getCurrentStream().read(buff, size);
        ret = res->getCurrentStream().gcount();
    }
    else
        ret = tab[index + 1]->call(req, res, buff, size, tab, index + 1);
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
