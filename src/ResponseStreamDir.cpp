#include "Stream/ResponseStreamDir.h"

ResponseStreamDir::ResponseStreamDir(const HttpRequest* req) : _type(DirStream)
{
    _stream = new std::stringstream();
    //everything's done here :
    _browser = new DirectoryBrowser(*req, _stream);
    _browser->get();
}

ResponseStreamDir::~ResponseStreamDir()
{
    delete this->_browser;
    delete this->_stream;
}

std::iostream&  ResponseStreamDir::getContent()
{
    return *this->_stream;
}

bool            ResponseStreamDir::completed() const
{
    return this->_stream->eof();
}

IResponseStream::Type   ResponseStreamDir::getType() const
{
    return this->_type;
}

size_t                  ResponseStreamDir::getSize() const
{
    return this->_stream->str().length();
}

