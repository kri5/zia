#include "HttpResponse.h"

HttpResponse::keyvalue     HttpResponse::ResponseStatus[] =
{
    // Informational
    { 100, "Continue" },
    { 101, "Switching Protocols" },
    // Successful
    { 200, "OK" },
    { 201, "Created" },
    { 202, "Accepted" },
    { 203, "Non-Authoritative Information" },
    { 204, "No Content" },
    { 205, "Reset Content" },
    { 206, "Partial Content" },
    // Redirection
    { 300, "Multiple Choices" },
    { 301, "Moved Permanently" },
    { 302, "Found" },
    { 303, "See Other" },
    { 304, "Not Modified" },
    { 305, "Use Proxy" },
    { 306, "(Unused)" },
    { 307, "Temporary Redirect" },
    // Client Error
    { 400, "Bad Request" },
    { 401, "Unauthorized" },
    { 402, "Payment Required" },
    { 403, "Forbidden" },
    { 404, "Not Found" },
    { 405, "Method Not Allowed" },
    { 406, "Not Acceptable" },
    { 407, "Proxy Authentication Required" },
    { 408, "Request Timeout" },
    { 409, "Conflict" },
    { 410, "Gone" },
    { 411, "Length Required" },
    { 412, "Precondition Failed" },
    { 413, "Request Entity Too Large" },
    { 414, "Request-URI Too Long" },
    { 415, "Unsupported Media Type" },
    { 416, "Requested Range Not Satisfiable" },
    { 417, "Expectation Failed" },
    // Server Error
    { 500, "Internal Server Error" },
    { 501, "Not Implemented" },
    { 502, "Bad Gateway" },
    { 503, "Service Unavailable" },
    { 504, "Gateway Timeout" },
    { 505, "HTTP Version Not Supported" },

    { -1, NULL }
};

HttpResponse::HttpResponse()
{
    ;
}

HttpResponse::~HttpResponse()
{
    ;
}

const char*     HttpResponse::getResponseStatusMessage(int key)
{
    for (int i = 0; ResponseStatus[i].value; i++)
        if (ResponseStatus[i].key == key) return ResponseStatus[i].value;
    return NULL;
}

void            HttpResponse::setProtocol(std::string& protocol)
{
    this->_protocol = protocol;
}

void            HttpResponse::setResponseValue(std::string& value)
{
    this->_responseValue = value;
}

void            HttpResponse::appendOption(HttpResponse::Option opt, std::string& value)
{
    this->_options[opt] = value;
}

void            HttpResponse::setContent(std::iostream content)
{
    ;
}

void            HttpResponse::setResponseStatus(int status)
{
   this->_responseStatus = status; 
}
