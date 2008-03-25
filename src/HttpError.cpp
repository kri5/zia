#include <sstream>

#include <iostream>
#include <string>
#include "Http/HttpError.h"

HttpError::HttpError(int status, HttpRequest& request) : _request(request), _status(status)
{
    _message = HttpResponse::getResponseStatusMessage(_status);
    _content = new std::stringstream;
    setContent();
}

HttpError::HttpError(int status, HttpRequest* request) : _request(*request), _status(status)
{
    this->_message = HttpResponse::getResponseStatusMessage(_status);
    _content = new std::stringstream;
    setContent();
}

void    HttpError::setContent()
{
    _content->str("");
    *_content << "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n";
    *_content << "<html><head>\n";
    *_content << "<title>Error " << _status << " " << _message << "</title>\n";
    *_content << "</head><body>\n<h1><b>" << _status << "</b> " << _message << "</h1>\n";
    *_content << "<hr>\n<address>ZiaHTTPD Server at http://" << 
        this->_request.getOption("Host").substr(0, this->_request.getOption("Host").find(":")) << 
        this->_request.getUri() << " Port ";
    if (this->_request.getOption("Host").find(":") != std::string::npos)
        *_content << this->_request.getOption("Host").substr(this->_request.getOption("Host").find(":") + 1);
    else
        *_content << this->_request.getConfig()->getDefaultPort();
    *_content << " </address>\n";
    *_content << "</body></html>\n";
    appendOption("Content-Length", _content->str().size());
    appendOption("Content-Type", "text/html");
    this->_request.print(); 
}

HttpError::~HttpError() throw()
{
    delete this->_content;
}

std::iostream&      HttpError::getContent()
{
    return *this->_content;
}

bool                HttpError::completed() const
{
    return this->_content->eof();
}
