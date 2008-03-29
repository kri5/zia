#include <iostream>

#include "toto.h"
#include "Utils/Buffer.h"


static IModule* _glInput = NULL;

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
        
size_t                  Toto::call(IModule::Event, IHttpRequest* req, IHttpResponse* res, char* buff, size_t size)
{
    return this->onProcessContent(req, res, buff, size);
}

IModule::ChainStatus    Toto::call(IModule::Event event, IHttpRequest* req, IHttpResponse* res)
{
    if (event == IModule::onPreSendEvent)
        return (this->onPreSend(req, res));
    else if (event == IModule::onPostSendEvent)
        return (this->onPostSend(req, res));
}

IModule::ChainStatus    Toto::onPreSend(IHttpRequest* req, IHttpResponse* res)
{
    Buffer* buff = new Buffer(512);
    if (req->setParam("totoBuffer", buff) == false)
        std::cout << "Diantre !!!" << std::endl;
}

size_t                  Toto::onProcessContent(IHttpRequest* req, IHttpResponse* res, char* buff, size_t size)
{
    std::cout << "processing content via totoModule" << std::endl;
    if (_glInput != NULL)
    {
        //_glInput->call(onProcessContentEvent, req, res, buff, size);
        strcpy(buff, "Vive les loutres");
    }
    else
    {
        std::iostream& stream = res->getContent();
        stream.read(buff, size);
        strcpy(buff, "Vive les loutres");
        return stream.gcount();
    }
}

IModule::ChainStatus    Toto::onPostSend(IHttpRequest* req, IHttpResponse* res)
{
    Buffer* buff = static_cast<Buffer*>(req->getParam("totoBuffer"));
    if (buff)
        delete buff;
}

extern "C" IModule* create()
{
    return new Toto;
}

extern "C" void destroy(IModule* i)
{
    delete i;
}

extern "C" std::string name()
{
    return "Toto module version 1.0";
}
