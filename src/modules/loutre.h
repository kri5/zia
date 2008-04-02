#ifndef LOUTRE_H__
# define LOUTRE_H__

#include "API/IModule.h"
#include "API/ISendResponse.h"
#include "AbstractModule.h"

class Loutre : public AbstractModule<ISendResponse>
{
    public:
        Loutre();
        ~Loutre();

        void            setInput(IModule*);

        size_t          call(IModule::Event, IHttpRequest*, IHttpResponse*, char*, size_t);
        ChainStatus     call(IModule::Event, IHttpRequest*, IHttpResponse*);
        ChainStatus     onPreSend(IHttpRequest*, IHttpResponse*);
        size_t          onProcessContent(IHttpRequest*, IHttpResponse*, char*, size_t);
        ChainStatus     onPostSend(IHttpRequest*, IHttpResponse*);
};

#endif //TOTO_H__

