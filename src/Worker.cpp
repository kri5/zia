#include "Worker.h"


void          Worker::create(ClientSocket& socket, Vhost& vhost)
{
    (new Worker(socket, vhost))->run();
}

void          Worker::code()
{
    try
    {
        HttpRequest* req = new HttpRequest();
        // Filling request object here
        req = req;
        //sendResponse();
    }
    catch (HttpError& e)
    {

    }
}

void                  Worker::sendResponse(HttpResponse& response)
{
    response = response;
    // Sending the response through a blocking write
}

HttpResponse&         Worker::request(HttpRequest& request)
{
    request = request;
    HttpResponse* toto = new HttpResponse();
    return *toto;
}

