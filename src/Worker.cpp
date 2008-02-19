#include "Worker.h"


void          Worker::create(ClientSocket& socket, Vhost& vhost)
{
    (new Worker(socket, vhost))->run();
}

void          Worker::code()
{
    // Insert your blocking read here
    // then parse the request, send your
    // response and you're done.
}

