#ifndef __WORKER_H__
#define __WORKER_H__

#include "IThread.h"
#include "ClientSocket.h"
#include "Vhost.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpError.h"

class Worker : public IThread
{
public:
    static void             create(ClientSocket& socket, Vhost& vhost);

private:
    Worker(ClientSocket& socket, Vhost& vhost) : _socket(socket), _vhost(vhost) { }
    virtual ~Worker() { }
    void                    code();

protected:
    void                    sendResponse(HttpResponse& response);
    virtual HttpResponse&   request(HttpRequest& request); 
    ClientSocket&           _socket;
    Vhost&                  _vhost;
};

#endif // WORKER_H
