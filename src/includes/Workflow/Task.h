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
        void    execute(unsigned int);
        void    clear(bool clearBuffers = true);
        /// Will totaly reinitialize the task (new client, new potential vhosts, and call init())
        void    init(ClientSocket*, const std::vector<Vhost*>*);
        /// Will just recreate a new HttpRequest and set HttpResponse instance to NULL
        void    init();
        bool    isFree() const;

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
        /// Will be set to false if browser sent more than one request in a keepalive socket.
        bool            _freeTask;
    public://FIXME
        unsigned int    _taskId;
        bool            poped;

        const std::vector<Vhost*>*    _vhosts;
};

#endif //TASK_H__

