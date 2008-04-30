#ifndef TASK_H__
# define TASK_H__

#include <vector>

#include "API/IClientSocket.h"
#include "Http/HttpRequest.h"
#include "Http/HttpResponse.h"
#include "Network/Vhost.h"
#include "Utils/Buffer.h"
#include "Mutex/IMutex.h"
#include "Mutex/Mutex.h"
#include "Http/IHttpCommand.h"

class Pool;

class Task
{
    public:
        Task(Pool*, unsigned int);
        virtual ~Task();
        void    execute();
        void    clear(bool clearBuffers = true);
        /// Will totaly reinitialize the task (new client, new potential vhosts, and call init())
        void    init(zAPI::IClientSocket*, const std::vector<Vhost*>*);
        /// Will just recreate a new HttpRequest and set HttpResponse instance to NULL
        void    init();
        bool    isFree() const;

    private:
        enum    Status
        {
            Stacked,
            Started,
            ReceivingRequest,
            BuildingResponse,
            SendingHeader,
            SendingResponse,
            RolledBack,
            Done
        };
        bool            parseRequest();
        bool            buildResponse();
        bool            sendHeader();
        bool            sendResponse();
        bool            finalize(bool);
        bool            sendBuffer();
        bool            receiveDatas();
        bool            sendResponseStream();
        bool            checkHeaderInStream();
        bool            checkTimeout();

        HttpRequest*            _req;
        HttpResponse*           _res;
        IHttpCommand*           _command;
        zAPI::IClientSocket*    _socket;
        Buffer*                 _writeBuffer;
        Buffer*                 _readBuffer;
        ITime*                  _time;
        Pool*                   _pool;
        int                     _timeoutDelay;
        /// Will be set to false if browser sent more than one request in a keepalive socket.
        bool                    _freeTask;
        unsigned int            _taskId;
        Status                  _status;
        bool                    _headerInStreamParsed;
        bool                    _headerInStreamSent;

        const std::vector<Vhost*>*    _vhosts;
};

#endif //TASK_H__

