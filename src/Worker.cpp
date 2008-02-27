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
            parser.parse();
        }
        parser.getRequest()->print();
        
        HttpRequest* req = parser.getRequest();
        sendResponse(this->request(*req));
    }
    catch (HttpError& e) // HttpError thrown (404, 500, ...)
    {
        sendResponse(e.getResponse());
    }
    Logger::getInstance() << Logger::Info << "Thread #" << this->pid() << " ended." << Logger::Flush;
}

/// Generate the data to be sent using the HttpResponse object
/// and send it to the client through a write.
void                  Worker::sendResponse(HttpResponse& response)
{
    std::istream& r = response.getContent();
    std::stringstream ss;
    ss << response.getContentLength();
    _socket << "HTTP/1.1 200 OK\r\n";
    _socket << "Server: ziahttpd/0.1 (Unix)  (Gentoo!)\r\n";
    _socket << "Content-Length: " << ss.str() << "\r\n";
    _socket << "Connection: close\r\n";
    _socket << "Content-Type: text/html\r\n";
    _socket << "\r\n";

    char buf[1024];
    while (r.good() && !r.eof())
    {
        r.read(buf, sizeof(buf));
        _socket.send(buf, r.gcount());
    }
    _socket.close(true);
    delete &r;
    delete &response;
}

/// Transform a request into a response by loading the file the client want, etc...
HttpResponse&         Worker::request(HttpRequest& request)
{
    
    std::string full = "/home/etix/www/" + request.getUri();

    File* fileinfo;
    try
    {
        //throw ZException<File>(INFO);
        fileinfo = new File(request.getUri(), "/home/etix/www/");
    }
    catch (ZException<File>& e)
    {
        throw HttpError(404, request); 
    }

    if (!fileinfo->isDirectory())
    {
        std::cout << "[i] Giving a file" << std::endl;
        std::string filepath("/home/etix/www/" + request.getUri());
        std::ifstream* data = new std::ifstream(filepath.c_str(), std::ios_base::binary);

        std::istream* is = new std::istream(data->rdbuf());

        HttpResponse* rep = new HttpResponse();
        rep->setResponseStatus(200); // Optional because 200 is set by default
        rep->setContentLength(fileinfo->getSize());
        rep->setContent(is);
        return *rep;
    }

    HttpResponse* rep = new HttpResponse();

    if (fileinfo)
        delete fileinfo;
    return (*rep);
}



