#include "Worker.h"

/// Launch a new thread that will handle the new client connection
void          Worker::create(ClientSocket& socket, const std::vector<Vhost*>& vhosts)
{
    (new Worker(socket, vhosts))->run();
}

/// Here we are in the first threaded method
void          Worker::code()
{
    Logger::getInstance() << Logger::Info << "Thread #" << this->pid() << " started." << Logger::Flush;
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
    Logger::getInstance() << Logger::Info << "Thread #" << this->pid() << " ended." << Logger::Flush;
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

    // TEST ETIX, DON'T REMOVE
    
    std::string path("/home/chouquette/");
    std::string file("test.png");
    std::string full = path + file;

    File fileinfo(file, path.c_str());
    std::ifstream data(full.c_str(), std::ios_base::binary);

    _socket << "HTTP/1.1 200 OK\r\n";
    _socket << "Server: ziahttpd/0.1 (Unix)  (Gentoo!)\r\n";
    _socket << "Content-Length: " << fileinfo.getSize() << "\r\n";
    _socket << "Connection: close\r\n";
    _socket << "Content-Type: image/png\r\n";
    _socket << "\r\n";


    char buf[512];

    while (data.good() && !data.eof())
    {
        //memset(buf, 0, sizeof(buf));
        data.read(buf, sizeof(buf));
        _socket.write(buf, data.gcount());
        //buf[data.gcount()] = 0;
        //_socket << buf;
    }
    _socket.flush();
    //_socket << flush;
    data.close();

    _socket.close(true);
    HttpResponse* toto = new HttpResponse();
    return *toto;
}

