#ifndef __WORKER_H__
#define __WORKER_H__

#include <fstream>
#include "IThread.h"
#include "ClientSocket.h"
#include "Vhost.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpParser.h"
#include "HttpError.h"
#include "Logger.hpp"
#include "File.h"

class Worker : public IThread
{
public:
    static void                     create(ClientSocket& socket, const std::vector<Vhost*>& vhosts);

private:
    Worker(ClientSocket& socket, const std::vector<Vhost*>& vhosts) : _socket(socket), _vhosts(vhosts) { }
    virtual ~Worker() { }
    void                            code();

protected:
    void                            sendResponse(HttpResponse& response);
    virtual HttpResponse&           request(HttpRequest& request); // This function had to be moved away (in another class) for visibility 
    ClientSocket&                   _socket;
    const std::vector<Vhost*>&      _vhosts;
};

#endif // WORKER_H
