#include "Stream/ResponseStreamFile.h"
#include "RootConfig.hpp"
#include <iostream>

ResponseStreamFile::ResponseStreamFile(IFile* file) : _file(file)
{
    _file->open();
}

ResponseStreamFile::~ResponseStreamFile()
{
    delete this->_file;
}

size_t                  ResponseStreamFile::read(char* buff, size_t size)
{
    return this->_file->read(buff, size);
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

