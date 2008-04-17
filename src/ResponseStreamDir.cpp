#include "Stream/ResponseStreamDir.h"

ResponseStreamDir::ResponseStreamDir(const HttpRequest* req)
{
    _stream = new std::stringstream();
    //everything's done here :
    _browser = new DirectoryBrowser(*req, _stream);
    _good = _browser->get();
}

ResponseStreamDir::~ResponseStreamDir()
{
    delete this->_browser;
    delete this->_stream;
}

bool            ResponseStreamDir::good() const
{
    return this->_good;
}

size_t              ResponseStreamDir::read(char* buff, size_t size)
{
    this->_stream->read(buff, size);
    return this->_stream->gcount();
}

bool            ResponseStreamDir::completed() const
{
    return this->_stream->eof();
}

//zAPI::IResponseStream::Type   ResponseStreamDir::getType() const
//{
//    return this->_type;
//}

size_t                  ResponseStreamDir::getSize() const
{
    return this->_stream->str().length();
}

