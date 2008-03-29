#ifndef TOTO_H__
# define TOTO_H__

#include "API/IModule.h"
#include "API/ISendResponse.h"
#include "AbstractModule.h"

class Toto : public AbstractModule<ISendResponse>
{
    public:
        Toto();
        ~Toto();

        void            setInput(IModule*);

        size_t          call(IModule::Event, IHttpRequest*, IHttpResponse*, char*, size_t);
        ChainStatus     call(IModule::Event, IHttpRequest*, IHttpResponse*);
        ChainStatus     onPreSend(IHttpRequest*, IHttpResponse*);
        size_t          onProcessContent(IHttpRequest*, IHttpResponse*, char*, size_t);
        ChainStatus     onPostSend(IHttpRequest*, IHttpResponse*);
};

#endif //TOTO_H__

