#ifndef WORKER_H
#define WORKER_H

#include "IThread.h"
#include "ClientSocket.h"
#include "Vhost.h"

class Worker : public IThread
{
public:
    static void       create(ClientSocket& socket, Vhost& vhost);

private:
    Worker(ClientSocket& socket, Vhost& vhost) : _socket(socket), _vhost(vhost) { }
    ~Worker() { }
    void              code();

protected:
    ClientSocket&     _socket;
    Vhost&            _vhost;
};

#endif // WORKER_H
