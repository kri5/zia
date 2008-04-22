#ifndef __PRIORITYTEST_H__
# define __PRIORITYTEST_H__

#include "API/IModule.h"
#include "API/ISendResponse.h"

class PriorityTest1 : public zAPI::IModule, public zAPI::ISendResponse
{
    public:
        PriorityTest1();
        ~PriorityTest1();
        
        int                         getPriority(zAPI::IModule::Event) const;
        zAPI::IModule::ChainStatus  onPreSend(zAPI::IHttpRequest*,
                                              zAPI::IHttpResponse*);
        zAPI::IModule::ChainStatus  onPostSend(zAPI::IHttpRequest*,
                                               zAPI::IHttpResponse*);
        size_t                      onProcessContent(zAPI::IHttpRequest*,
                                        zAPI::IHttpResponse*, char*, size_t,
                                        const std::vector<zAPI::ISendResponse*>&,
                                        unsigned int);
};

#endif /* !__PRIORITYTEST_H__ */
