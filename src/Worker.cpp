#include "Worker.h"


void          Worker::create(ClientSocket& socket, Vhost& vhost)
{
    (new Worker(socket, vhost))->run();
}

void          Worker::code()
{


}

