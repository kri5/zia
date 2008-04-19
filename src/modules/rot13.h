#ifndef ROT13_H__
# define ROT13_H__

#include "API/IModule.h"
#include "API/ISendResponse.h"
#include "Modules/AbstractModule.h"

class Rot13 : public zAPI::IModule, public zAPI::ISendResponse
{
    public:
        Rot13();
        ~Rot13();

        int             getPriority(zAPI::IModule::Event event) const;
        ChainStatus     onPreSend(zAPI::IHttpRequest*, zAPI::IHttpResponse*);
        size_t          onProcessContent(zAPI::IHttpRequest*, zAPI::IHttpResponse*, char*, size_t, std::vector<ISendResponse*>, unsigned int);
        ChainStatus     onPostSend(zAPI::IHttpRequest*, zAPI::IHttpResponse*);
};

#endif //TOTO_H__

