#include "HttpResponse.h"

HttpResponse::HttpResponse()
{
    ;
}

HttpResponse::~HttpResponse()
{
    ;
}

void    HttpResponse::setProtocol(std::string& protocol)
{
    this->_protocol = protocol;
}

void    HttpResponse::setResponseCode(std::string& code)
{
    this->_responseCode = code;
}

void    HttpResponse::setResponseValue(std::string& value)
{
    this->_responseValue = value;
}

void    HttpResponse::appendOption(HttpResponse::Option opt, std::string& value)
{
    this->_options[opt] = value;
}
