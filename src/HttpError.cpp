#include <sstream>

#include <iostream>
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
    *_content << "<hr>\n<address>ZiaHTTPD Server at _url_ Port _port_</address>\n";
    *_content << "</body></html>\n";
    appendOption("Content-Length", _content->str().size());
    appendOption("Content-Type", "text/html");
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
