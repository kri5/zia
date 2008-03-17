#include <sstream>

#include "Http/HttpError.h"

HttpError::HttpError(int status, HttpRequest& request) : _request(request)
{
    this->_status = status;
    this->_message = HttpResponse::getResponseStatusMessage(_status);
    _content = new std::stringstream;
}

HttpError::HttpError(int status, HttpRequest* request) : _request(*request)
{
    this->_status = status;
    this->_message = HttpResponse::getResponseStatusMessage(_status);
    _content = new std::stringstream;
}

HttpError::~HttpError() throw()
{
    delete this->_content;
}

std::iostream&      HttpError::getContent()
{
    this->_content->str("");
    *this->_content << "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n";
    *this->_content << "<html><head>\n";
    *this->_content << "<title>Error " << _status << " " << _message << "</title>\n";
    *this->_content << "</head><body>\n<h1><b>" << _status << "</b> " << _message << "</h1>\n";
    *this->_content << "<hr>\n<address>ZiaHTTPD Server at _url_ Port _port_</address>\n";
    *this->_content << "</body></html>\n";
    this->appendOption("ContentLength", this->_content->str().size());
    return *this->_content;
}

bool                HttpError::completed() const
{
    return this->_content->eof();
}
