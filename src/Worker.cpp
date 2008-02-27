#include "Buffer.h"
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
        HttpParser      parser;
        Buffer          buff(1024);
        char            tmp[1024];
        char*           line;
        int             sockRet;

        while (parser.done() == false)
        {
            sockRet = _socket.recv(tmp, 1024);
            buff.push(tmp, sockRet);
            while (buff.hasEOL())
            {
                line = buff.getLine();
                std::cout << "line == " << line << std::endl;
                parser.feed(line);
                delete[] line;
            }
        }
        parser.getRequest()->print();
        //sendResponse(this->request(*req));
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
    
    std::string full = "~/www/" + request.getUri();

    File fileinfo(request.getUri(), "~/www/");
    std::stringstream ss;
    ss << fileinfo.getSize();
    std::ifstream data(full.c_str(), std::ios_base::binary);

    _socket << "HTTP/1.1 200 OK\r\n";
    _socket << "Server: ziahttpd/0.1 (Unix)  (Gentoo!)\r\n";
    _socket << "Content-Length: " << ss.str() << "\r\n";
    _socket << "Connection: close\r\n";
    _socket << "Content-Type: text/html\r\n";
    _socket << "\r\n";


    char buf[4096];

    while (data.good() && !data.eof())
    {
        data.read(buf, sizeof(buf));
        _socket.send(buf, data.gcount());
    }
    data.close();

    _socket.close(true);
    HttpResponse* toto = new HttpResponse();
    return *toto;
}

