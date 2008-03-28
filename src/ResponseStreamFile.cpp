#include "Stream/ResponseStreamFile.h"
#include "RootConfig.hpp"
#include <iostream>

ResponseStreamFile::ResponseStreamFile(IFile* file) : _file(file), _type(FileStream)
{
}

ResponseStreamFile::~ResponseStreamFile()
{
    delete this->_file;
}

std::iostream&      ResponseStreamFile::getContent()
{
    return *(this->_file->getStream());
}

bool                ResponseStreamFile::completed() const
{
    return this->_file->eof();
}

IResponseStream::Type   ResponseStreamFile::getType() const
{
    return this->_type;
}

size_t                  ResponseStreamFile::getSize() const
{
    return this->_file->getSize();
}

bool                    ResponseStreamFile::good() const
{
    return this->_file->good();
}

