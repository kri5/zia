#ifndef TASK_H__
# define TASK_H__

#include <vector>

#include "Sockets/ClientSocket.h"
#include "Http/HttpRequest.h"
#include "Http/HttpResponse.h"
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
        void    clear(bool clearBuffers = true);
        /// Will totaly reinitialize the task (new client, new potential vhosts, and call init())
        void    init(ClientSocket*, const std::vector<const Vhost*>*);
        /// Will just recreate a new HttpRequest and set HttpResponse instance to NULL
        void    init();

    private:
        bool            parseRequest();
        bool            buildResponse();
        bool            sendHeader();
        bool            sendResponse();
        bool            finalize(bool);
        bool            sendBuffer();
        bool            receiveDatas();
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

