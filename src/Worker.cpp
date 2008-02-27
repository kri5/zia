#include "Buffer.h"
#include "Worker.h"

/// Launch a new thread that will handle the new client connection
void          Worker::create(ClientSocket& socket, const std::vector<const Vhost*>& vhosts)
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
            sockRet = this->_socket.recv(tmp, 1024);
            std::cout << sockRet << std::endl;
            if (sockRet <= 0)
            {
                this->_socket.close(false);
                return ;
            }
            buff.push(tmp, sockRet);
            while (buff.hasEOL())
            {
                line = buff.getLine();
                parser.feed(line);
                delete[] line;
            }
            parser.parse();
            std::cout << "Done: " << parser.done() << std::endl;
            std::cout << "Tmp: " << tmp << std::endl;
        }
        parser.getRequest()->print();
		const Config* cfg = Vhost::getVhost(this->_vhosts, parser.getRequest()->getOption(HttpRequest::Host));
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
    std::stringstream status;
    status << response.getResponseStatus();
    std::stringstream len;
    len << response.getContentLength();
    _socket << "HTTP/1.1 " << status.str() << " " << response.getResponseValue() << "\r\n";
    _socket << "Server: ziahttpd/0.1 (Unix)  (Gentoo!)\r\n";
    _socket << "Content-Length: " << len.str() << "\r\n";
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
    const Config* cfg = Vhost::getVhost(this->_vhosts, request.getOption(HttpRequest::Host));
    std::string full = cfg->getParam("DocumentRoot") + request.getUri();

    File* fileinfo;
    try
    {
        //throw ZException<File>(INFO);
        fileinfo = new File(request.getUri(), cfg->getParam("DocumentRoot").c_str());
    }
    catch (ZException<File>& e)
    {
        throw HttpError(404, request); 
    }

    if (!fileinfo->isDirectory())
    {
        std::cout << "[i] Giving a file" << std::endl;
        std::string filepath(cfg->getParam("DocumentRoot") + request.getUri());
        std::ifstream* data = new std::ifstream(filepath.c_str(), std::ios_base::binary);

        std::istream* is = new std::istream(data->rdbuf());

        HttpResponse* rep = new HttpResponse();
        rep->setResponseStatus(200); // Optional because 200 is set by default
        rep->setContentLength(fileinfo->getSize());
        rep->setContent(is);
        delete fileinfo;
        return *rep;
    }
    else
    {
        std::cout << "[i] Giving a directory listing" << std::endl;
        DirectoryBrowser d(request);
        HttpResponse& rep = d.getResponse();
        return rep;
    }

    throw HttpError(500, request);
}



