#include <iostream>

#include "toto.h"
#include "Utils/Buffer.h"


static zAPI::IModule* _glInput = NULL;

Toto::Toto()
{
}

Toto::~Toto()
{
}

void                    Toto::setInput(IModule* mod)
{
    _glInput = mod;
}
        
size_t                  Toto::call(zAPI::IModule::Event, zAPI::IHttpRequest* req, zAPI::IHttpResponse* res, char* buff, size_t size)
{
    return this->onProcessContent(req, res, buff, size);
}

zAPI::IModule::ChainStatus    Toto::call(zAPI::IModule::Event event, zAPI::IHttpRequest* req, zAPI::IHttpResponse* res)
{
    if (event == zAPI::IModule::onPreSendEvent)
        return (this->onPreSend(req, res));
    else if (event == zAPI::IModule::onPostSendEvent)
        return (this->onPostSend(req, res));
	return zAPI::IModule::ERRORMODULE;
}

zAPI::IModule::ChainStatus    Toto::onPreSend(zAPI::IHttpRequest* req, zAPI::IHttpResponse* res)
{
    /*Buffer* buff = new Buffer(512);
    if (req->setParam("totoBuffer", buff) == false)
        std::cout << "Diantre !!!" << std::endl;*/
	return zAPI::IModule::CONTINUE;
}

size_t                  Toto::onProcessContent(zAPI::IHttpRequest* req, zAPI::IHttpResponse* res, char* buff, size_t size)
{
    if (_glInput != NULL)
    {
        size_t retSize = _glInput->call(onProcessContentEvent, req, res, buff, size);
        for (unsigned int i = 0; i < retSize; ++i)
        {
            if (buff[i] == 'W')
                buff[i] = 'Z';
        }
        return retSize;
    }
    else
    {
        std::iostream& stream = res->getCurrentStream();
        stream.read(buff, size);
        size_t  retSize = stream.gcount();
        for (unsigned int i = 0; i < retSize; ++i)
        {
            if (buff[i] == 'W')
                buff[i] = 'Z';
        }
        return retSize;
    }
}

zAPI::IModule::ChainStatus    Toto::onPostSend(zAPI::IHttpRequest* req, zAPI::IHttpResponse* res)
{
    /*Buffer* buff = static_cast<Buffer*>(req->getParam("totoBuffer"));
    if (buff)
        delete buff;*/
	return IModule::CONTINUE;
}

extern "C" zAPI::IModule* create()
{
    return new Toto;
}

extern "C" void destroy(zAPI::IModule* i)
{
    delete i;
}

extern "C" std::string name()
{
    return "Toto";
}

extern "C" int  version()
{
    return 1;
}
