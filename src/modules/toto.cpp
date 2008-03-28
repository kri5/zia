#include "toto.h"

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
    return this->processContent(req, res, buff, size);
}

IModule::ChainStatus    Toto::call(IModule::Event event, IHttpRequest* req, IHttpResponse* res)
{
    if (event == IModule::onPreSendEvent)
        return (this->onPreSendEvent(req, res));
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
    if (_glInput != NULL)
    {
        return _glInput->onProcessContent(req, res, buff, size);
    }
    else
    {
        std::iostream& stream = res->getContent();
        stream.read(buff, size);
        return stream.gcount();
    }
}

IModule::ChainStatus    Toto::onPostSend(IHttpRequest* req, IHttpResponse* res)
{
    Buffer* buff = req->getParam("totoBuffer");
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
