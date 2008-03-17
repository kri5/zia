#include "Http/HttpResponseFile.h"
#include "RootConfig.hpp"
#include <iostream>

HttpResponseFile::HttpResponseFile(IFile* file) : HttpResponse(), _file(file)
{
    appendOption("ContentLength", file->getSize());
    appendOption("MimeType", 
    RootConfig::getInstance().getConfig()->getMimeType
           (file->getExtension()));
}

HttpResponseFile::~HttpResponseFile()
{
    delete this->_file;
}

std::iostream&      HttpResponseFile::getContent()
{
    return *(this->_file->getStream());
}

bool                HttpResponseFile::completed() const
{
    return this->_file->eof();
}

