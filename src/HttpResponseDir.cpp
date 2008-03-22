#include "Http/HttpResponseDir.h"

HttpResponseDir::HttpResponseDir(const HttpRequest* req) : HttpResponse() 
{
    _stream = new std::stringstream();
    //everything's done here :
    _browser = new DirectoryBrowser(*req, _stream);
    _browser->get();
    appendOption("Content-Length", _stream->str().length());
    appendOption("Content-Type", "text/html");
}

HttpResponseDir::~HttpResponseDir()
{
    delete this->_browser;
    delete this->_stream;
}

std::iostream&  HttpResponseDir::getContent()
{
    return *this->_stream;
}

bool            HttpResponseDir::completed() const
{
    return this->_stream->eof();
}
