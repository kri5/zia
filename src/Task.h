#ifndef TASK_H__
# define TASK_H__

#include <vector>

#include "ClientSocket.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "Vhost.h"
#include "Buffer.h"
#include "HttpError.h"

class Task
{
    public:
        Task(ClientSocket*, const std::vector<const Vhost*>);
        virtual ~Task();
        void    execute();

    private:
        bool            parseRequest();
        bool            buildResponse();
        bool            sendResponse();
        void            sendError();
        void            sendBuffer();

        HttpRequest*    _req;
        HttpResponse*   _res;
        ClientSocket*   _socket;
        Buffer*         _writeBuffer;
        Buffer*         _readBuffer;
        ITime*          _time; //FIXME

        const std::vector<const Vhost*>&    _vhosts;
};

#endif //TASK_H__
