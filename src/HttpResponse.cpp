#include <assert.h>
#include "Http/HttpResponse.h"

HttpResponse::KeyValue     HttpResponse::ResponseStatus[] =
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

HttpResponse::HttpResponse() : _responseStatus(200), _contentLength(0), _mimeType("text/html"), _currentStream(NULL), _sendMode(false), _headerInStream(false)
{
    //"text/html" => probable default value (in case of an error, directory.
    // if it's a file, we will change this.
}

HttpResponse::~HttpResponse()
{
}

const char*             HttpResponse::getResponseStatusMessage(int key)
{
    for (int i = 0; ResponseStatus[i].value; i++)
        if (ResponseStatus[i].key == key)
            return ResponseStatus[i].value;
    return NULL;
}

void                    HttpResponse::setResponseStatus(int status)
{
    this->_responseStatus = status; 
}

int                     HttpResponse::getResponseStatus() const
{
    return this->_responseStatus;
}

std::string     HttpResponse::getResponseValue() const
{
    return std::string(getResponseStatusMessage(this->_responseStatus));
}

void                    HttpResponse::appendStream(zAPI::IResponseStream* stream)
{
    this->_streams.push(stream);
    this->_contentLength += stream->getSize();
}

std::queue<zAPI::IResponseStream*>&   HttpResponse::getStreams()
{
    return this->_streams;
}

std::iostream&          HttpResponse::getCurrentStream()
{
    assert(this->_currentStream != NULL);
    return *(this->_currentStream);
}

void                    HttpResponse::setCurrentStream(std::iostream* stream)
{
    this->_currentStream = stream;
}

void                    HttpResponse::setError(ErrorResponseStream* error)
{
    this->_responseStatus = error->getStatus();
    this->clearStreams();
    this->appendStream(error);
    this->setHeaderOption("Content-Length", error->getSize());
}

void                    HttpResponse::clearStreams()
{
    zAPI::IResponseStream*    s;

    while (this->_streams.empty() == false)
    {
        s = this->_streams.front();
        this->_streams.pop();
        delete s;
    }
}

size_t                  HttpResponse::getContentLength() const
{
    return this->_contentLength;
}

void					HttpResponse::skipToSend(zAPI::IResponseStream* stream, bool flushStreams, bool headerIncluded)
{
	if (flushStreams)
		this->clearStreams();
	if (stream)
		this->appendStream(stream);
	this->_sendMode = true;
	this->_headerInStream = headerIncluded;
}

bool					HttpResponse::isInSendMode() const
{
	return this->_sendMode;
}

bool					HttpResponse::headerInStream() const
{
	return this->_headerInStream;
}

