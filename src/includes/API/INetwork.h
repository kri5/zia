#ifndef __INETWORK_H__
# define __INETWORK_H__

#include "IModule.h"
#include "Sockets/IClientSocket.h"

typedef int SOCKET;

class INetwork : IModule
{
    public:
        virtual ~INetwork() {};
        virtual IClientSocket*  onAccept(SOCKET) = 0;
        virtual ChainStatus     onReceive(char*, size_t) = 0;
        virtual ChainStatus     onSend(char*, size_t) = 0;
};

#endif /* !__INETWORK_H__ */
