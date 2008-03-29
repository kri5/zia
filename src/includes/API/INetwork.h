#ifndef __INETWORK_H__
# define __INETWORK_H__

#include "API/IModule.h"
#include "API/IClientSocket.h"

class INetwork : public IModule
{
    public:
        virtual ~INetwork() {};
        virtual IClientSocket*  onAccept(SOCKET) = 0;
        virtual ChainStatus     onReceive(char*, size_t) = 0;
        virtual ChainStatus     onSend(char*, size_t) = 0;
};

#endif /* !__INETWORK_H__ */
