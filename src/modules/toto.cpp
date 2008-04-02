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
	return IModule::ERRORMODULE;
}

IModule::ChainStatus    Toto::onPreSend(IHttpRequest* req, IHttpResponse* res)
{
    /*Buffer* buff = new Buffer(512);
    if (req->setParam("totoBuffer", buff) == false)
        std::cout << "Diantre !!!" << std::endl;*/
	return IModule::CONTINUE;
}

size_t                  Toto::onProcessContent(IHttpRequest* req, IHttpResponse* res, char* buff, size_t size)
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

IModule::ChainStatus    Toto::onPostSend(IHttpRequest* req, IHttpResponse* res)
{
    /*Buffer* buff = static_cast<Buffer*>(req->getParam("totoBuffer"));
    if (buff)
        delete buff;*/
	return IModule::CONTINUE;
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
    return "Toto";
}

extern "C" int  version()
{
    return 1;
}
