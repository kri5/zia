#include "Stream/ResponseStreamGeneric.h"

ResponseStreamGeneric::ResponseStreamGeneric(std::stringstream* stream) : _stream(stream)
{
}

ResponseStreamGeneric::~ResponseStreamGeneric()
{
	delete this->_stream;
}

std::iostream&	ResponseStreamGeneric::getContent()
{
	return *(this->_stream);
}

bool	ResponseStreamGeneric::completed() const
{
	return this->_stream->eof();
}

bool	ResponseStreamGeneric::good() const
{
	return this->_stream->good();
}

zAPI::IResponseStream::Type	ResponseStreamGeneric::getType() const
{
	return IResponseStream::Generic;
}

size_t					ResponseStreamGeneric::getSize() const
{
    return this->_stream->str().size();
}
