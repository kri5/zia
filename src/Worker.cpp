#include "Worker.h"

/// Launch a new thread that will handle the new client connection
void          Worker::create(ClientSocket& socket, const std::vector<Vhost*>& vhosts)
{
    (new Worker(socket, vhosts))->run();
}

/// Here we are in the first threaded method
void          Worker::code()
{
    try
    {
        HttpRequest* req = new HttpRequest();
        //TODO Filling request object here by reading what the client send to us
        sendResponse(this->request(*req));
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

    std::ostringstream oss;
    oss << "Hello from pid " << this->pid() << std::endl;

    std::string msg = oss.str();

    _socket.send(msg.c_str(), msg.size());
    _socket.close(true);
    HttpResponse* toto = new HttpResponse();
    return *toto;
}

