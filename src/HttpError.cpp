#include "HttpError.h"

HttpError::HttpError(int status, HttpRequest& request) : _request(request)
{
    this->_status = status;
    this->_message = HttpResponse::getResponseStatusMessage(_status);
}

HttpError::~HttpError() throw()
{

}

const char*         HttpError::what() const throw()
{
    return HttpResponse::getResponseStatusMessage(this->_status); 
}

HttpResponse&       HttpError::getResponse() const
{
    HttpResponse* response = new HttpResponse();
    response->setResponseStatus(_status);
    
    std::ostringstream* content = new std::ostringstream();
    *content << "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n";
    *content << "<html><head>\n";
    *content << "<title>Error " << _status << " " << _message << "</title>\n";
    *content << "</head><body>\n<h1><b>" << _status << "</b> " << _message << "</h1>\n";
    *content << "<hr>\n<address>ZiaHTTPD Server at _url_ Port _port_</address>\n";
    *content << "</body></html>\n";

    //response->setContent(*content);
    return *response;
}

