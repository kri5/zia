#include <sstream>

#include <iostream>
#include <string>
#include "Stream/ErrorResponseStream.h"
#include "Http/HttpResponse.h"

ErrorResponseStream::ErrorResponseStream(int status, HttpRequest& request) : _request(request), _status(status)
{
    _message = HttpResponse::getResponseStatusMessage(_status);
    _content = new std::stringstream;
    setContent();
}

ErrorResponseStream::ErrorResponseStream(int status, HttpRequest* request) : _request(*request), _status(status)
{
    this->_message = HttpResponse::getResponseStatusMessage(_status);
    _content = new std::stringstream;
    setContent();
}

void    ErrorResponseStream::setContent()
{
    _content->str("");
    *_content << "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n";
    *_content << "<html><head>\n";
    *_content << "<title>Error " << _status << " " << _message << "</title>\n";
    *_content << "</head><body>\n<h1><b>" << _status << "</b> " << _message << "</h1>\n";
    *_content << "<hr>\n<address>ZiaHTTPD Server at http://" << 
        this->_request.getHeaderOption("Host").substr(0, this->_request.getHeaderOption("Host").find(":")) << 
        this->_request.getUri() << " Port ";
    if (this->_request.getHeaderOption("Host").find(":") != std::string::npos)
        *_content << this->_request.getHeaderOption("Host").substr(this->_request.getHeaderOption("Host").find(":") + 1);
    else
        *_content << this->_request.getConfig()->getParam("Port");
    *_content << " </address>\n";
    *_content << "</body></html>\n";
}

ErrorResponseStream::~ErrorResponseStream() throw()
{
    delete this->_content;
}

std::iostream&      ErrorResponseStream::getContent()
{
    return *this->_content;
}

bool                ErrorResponseStream::completed() const
{
    return this->_content->eof();
}

//zAPI::IResponseStream::Type   ErrorResponseStream::getType() const
//{
//    return this->_type;
//}

int                     ErrorResponseStream::getStatus() const
{
    return this->_status;
}

size_t                  ErrorResponseStream::getSize() const
{
    return this->_content->str().length();
}

bool                    ErrorResponseStream::good() const
{
    return true;
}

