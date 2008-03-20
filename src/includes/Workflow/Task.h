#ifndef TASK_H__
# define TASK_H__

#include <vector>

#include "Sockets/ClientSocket.h"
#include "Http/HttpRequest.h"
#include "Http/HttpResponse.h"
#include "Http/HttpError.h"
#include "Network/Vhost.h"
#include "Utils/Buffer.h"
#include "Mutex/IMutex.h"
#include "Mutex/Mutex.h"

class Pool;

class Task
{
    public:
        Task(Pool*);
        virtual ~Task();
        void    execute();
        void    clear();
        void    init(ClientSocket*, const std::vector<const Vhost*>*);

    private:
        bool            parseRequest();
        bool            buildResponse();
        bool            sendResponse();
        bool            finalize(bool);
        void            sendError();
        bool            sendBuffer();
        bool            checkTimeout();

        HttpRequest*    _req;
        HttpResponse*   _res;
        ClientSocket*   _socket;
        Buffer*         _writeBuffer;
        Buffer*         _readBuffer;
        ITime*          _time;
        Pool*           _pool;
        int             _timeoutDelay;

        const std::vector<const Vhost*>*    _vhosts;
};

#endif //TASK_H__

