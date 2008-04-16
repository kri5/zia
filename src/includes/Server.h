#ifndef SERVER_H__
# define SERVER_H__

#include <map>
#include <vector>

#include "zia.h"
#include "Config.h"
#include "Network/NetworkID.h"
#include "Sockets/MainSocket.h"
#include "Network/Vhost.h"
#include "Workflow/Pool.h"
#include "API/IServerEvent.h"

class	Server
{
    public:
        struct	Error
        {
            enum	Code
            {
                Unknown,
                Bind,
                Select
            };
            static const char* Msg[];
        };
        Server(const std::map<const NetworkID*, std::vector<Vhost*> >&, Pool* pool);
        ~Server();
        void			run();

    private:
        int																_maxFd;
        const std::map<const NetworkID*, std::vector<Vhost*> >&	_toBind;
        std::vector<MainSocket*>										_sockets;
        Pool*                                                           _pool;
        const Config*													_rootCfg;

        void    checkSockets(int nbSockets, const struct pollfd* fds) const;
};

#endif //SERVER_H__

