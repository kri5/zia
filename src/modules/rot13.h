#ifndef ROT13_H__
# define ROT13_H__

#include "API/IModule.h"
#include "API/ISendResponse.h"
#include "Modules/AbstractModule.h"

class Rot13 : public AbstractModule, public zAPI::ISendResponse
{
    public:
        Rot13();
        ~Rot13();

        size_t          call(zAPI::IHttpRequest*, zAPI::IHttpResponse*, char*, size_t, IModule**, unsigned int);
        ChainStatus     call(zAPI::IModule::Event, zAPI::IHttpRequest*, zAPI::IHttpResponse*);
        ChainStatus     onPreSend(zAPI::IHttpRequest*, zAPI::IHttpResponse*);
        size_t          onProcessContent(zAPI::IHttpRequest*, zAPI::IHttpResponse*, char*, size_t, IModule**, unsigned int);
        ChainStatus     onPostSend(zAPI::IHttpRequest*, zAPI::IHttpResponse*);
};

#endif //TOTO_H__

