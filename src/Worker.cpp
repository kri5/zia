#include "Worker.h"

/// Launch a new thread that will handle the new client connection
void          Worker::create(ClientSocket& socket, Vhost& vhost)
{
    (new Worker(socket, vhost))->run();
}

/// Here we are in the first threaded method
void          Worker::code()
{
    try
    {
        HttpRequest* req = new HttpRequest();
        //TODO Filling request object here by reading what the client send to us
        req = req;
        //sendResponse(this->request(req));
    }
    catch (HttpError& e) // HttpError thrown (404, 500, ...)
    {
        // sendResponse(e.getResponse())
    }
}

/// Generate the data to be sent using the HttpResponse object
/// and send it to the client through a write.
void                  Worker::sendResponse(HttpResponse& response)
{
    response = response;
    //TODO Sending the response through a blocking write (using stream ?)
}

/// Transform a request into a response by loading the file the client want, etc...
HttpResponse&         Worker::request(HttpRequest& request)
{
    request = request;
    HttpResponse* toto = new HttpResponse();
    return *toto;
}

