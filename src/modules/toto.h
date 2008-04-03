#ifndef TOTO_H__
# define TOTO_H__

#include "API/IModule.h"
#include "API/ISendResponse.h"
#include "AbstractModule.h"

class Toto : public AbstractModule<zAPI::ISendResponse>
{
    public:
        Toto();
        ~Toto();

        void            setInput(zAPI::IModule*);

        size_t          call(zAPI::IModule::Event, zAPI::IHttpRequest*, zAPI::IHttpResponse*, char*, size_t);
        ChainStatus     call(zAPI::IModule::Event, zAPI::IHttpRequest*, zAPI::IHttpResponse*);
        ChainStatus     onPreSend(zAPI::IHttpRequest*, zAPI::IHttpResponse*);
        size_t          onProcessContent(zAPI::IHttpRequest*, zAPI::IHttpResponse*, char*, size_t);
        ChainStatus     onPostSend(zAPI::IHttpRequest*, zAPI::IHttpResponse*);
};

#endif //TOTO_H__

