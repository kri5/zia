#include "Buffer.h"
#include "Worker.h"

#include "MemoryManager.hpp"

Worker::~Worker()
{
    delete &(this->_socket);
}

/// Launch a new thread that will handle the new client connection
void          Worker::create(ClientSocket& socket, const std::vector<const Vhost*>& vhosts)
{
    Worker* w;
    try
    {
        w = new Worker(socket, vhosts);
    }
    catch (std::exception& ex)
    {
        std::cerr << ex.what()<< std::endl;
    }
    catch (...)
    {
        std::cerr << "Unknown exception" << std::endl;
    }
    w->run();
}

/// Here we are in the first threaded method
void          Worker::code()
{
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
                buff.flush();
            }
            parser.parse();
        }
        HttpRequest* req = parser.getRequest();
		req->setConfig(Vhost::getVhost(this->_vhosts, req->getOption(HttpRequest::Host)));
        this->request(*req);
    }
    catch (HttpError& e) // HttpError thrown (404, 500, ...)
    {
        sendResponse(e.getResponse());
    }
}


void                    Worker::sendResponse(HttpResponse& response)
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
   _socket << "Content-Type: " << response.getMimetype() <<  "\r\n";
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

/// Generate the data to be sent using the HttpResponse object
/// and send it to the client through a write.
void                  Worker::sendResponseFile(HttpResponse& response)
{
    IFile& file = response.getFileContent();
    std::ostringstream           str;

    str << response.getResponseStatus();
    _socket << "HTTP/1.1 " << str.str() << " " << response.getResponseValue() << "\r\n";
    _socket << "Server: ziahttpd/0.1 (Unix)  (Gentoo!)\r\n";
    str.str("");
    str << file.getSize();
    _socket << "Content-Length: " << str.str() << "\r\n";
    _socket << "Connection: close\r\n";
    _socket << "Content-Type: " << response.getMimetype() <<  "\r\n";
    _socket << "\r\n";

    char buf[1024];
    std::streamsize     ret;
    file.open();
    while (file.good() && !file.eof())
    {
        ret = file.get(buf, sizeof(buf));
        _socket.send(buf, ret);
    }
    _socket.close(true);
    file.close();
}

/// Transform a request into a response by loading the file the client want, etc...
void         Worker::request(HttpRequest& request)
{
    std::string full = request.getConfig()->getParam("DocumentRoot") + request.getUri();

    File* fileinfo;
    try
    {
        //throw ZException<File>(INFO);
        fileinfo = new File(request.getUri(), request.getConfig()->getParam("DocumentRoot").c_str());
    }
    catch (ZException<File>& e)
    {
        throw HttpError(404, request); 
    }

    if (!fileinfo->isDirectory())
    {
        std::string filepath(request.getConfig()->getParam("DocumentRoot") + request.getUri());

        HttpResponse* rep = new HttpResponse();
        rep->setMimetype(RootConfig::getInstance().getConfig()->getMimeType(fileinfo->getExtension()));
        rep->setResponseStatus(200); // Optional because 200 is set by default
        rep->setContentLength(fileinfo->getSize());
        rep->setContent(fileinfo);
        this->sendResponseFile(*rep);
        delete rep;
        delete fileinfo;
        return ;
    }
    else
    {
        DirectoryBrowser d(request);
        HttpResponse& rep = d.getResponse();
        this->sendResponse(rep);
        delete fileinfo;
        return ;
    }

    throw HttpError(500, request);
}



