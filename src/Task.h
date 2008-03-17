#ifndef TASK_H__
# define TASK_H__

#include <vector>

#include "ClientSocket.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "Vhost.h"
#include "Buffer.h"
#include "HttpError.h"
#include "IMutex.h"
#include "Mutex.h"

class Task
{
    public:
        Task();
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
        int             _timeoutDelay;

        const std::vector<const Vhost*>*    _vhosts;
};

#endif //TASK_H__

