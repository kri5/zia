#include "Stream/ResponseStreamFile.h"
#include "RootConfig.hpp"
#include <iostream>

ResponseStreamFile::ResponseStreamFile(IFile* file) : _file(file)
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

size_t                  ResponseStreamFile::getSize() const
{
    return this->_file->getSize();
}

bool                    ResponseStreamFile::good() const
{
    return this->_file->good();
}

