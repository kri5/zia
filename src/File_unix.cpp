#include <unistd.h>

#include "File/File_unix.h"
#include "ZException.hpp"
#include "Time/Time.h"

#include "MemoryManager.hpp"

File::File(const std::string& filename, const std::string& path) : _name(filename), _modifTime(nullptr), _stream(nullptr), _closed(false), _errorCode(Error::None)
{
  path.empty() ? _filePath = filename : _filePath = path + "/" + filename;
  if (stat(_filePath.c_str(), &_stat) < 0) {
    _errorCode = Error::NoSuchFile;
  } else if (access(_filePath.c_str(), R_OK) < 0) {
    _errorCode = Error::PermissionDenied;
  }
}

File::~File() {
  if (this->_modifTime)
    delete this->_modifTime;
  if (this->_stream) {
    this->close();
    delete this->_stream;
  }
}

IFile::Error::Code File::getError() const { return this->_errorCode; }

const std::string &File::getFileName() const { return this->_name; }

const std::string &File::getFullFileName() const { return this->_filePath; }

unsigned int File::getSize() const { return this->_stat.st_size; }

ITime *File::getModifDate() {
  if (this->_modifTime == nullptr)
    this->_modifTime = new Time(this->_stat.st_mtime);
  return this->_modifTime;
}

bool File::isDirectory() const { return S_ISDIR(this->_stat.st_mode); }

std::string File::getExtension() const {
  return this->_name.substr(this->_name.rfind("."));
}

void File::open() {
  if (this->_stream == nullptr)
    this->_stream = new std::fstream(this->_filePath.c_str(),
                                     std::ios_base::in | std::ios_base::binary);
}

void File::close() {
  if (this->_stream && this->_closed == false) {
    this->_closed = true;
    this->_stream->close();
  }
}

std::streamsize File::get(char *buff, size_t len) {
  if (this->_stream == nullptr)
    return 0;
  this->_stream->read(buff, len);
  return this->_stream->gcount();
}

bool File::good() const {
  return (this->_stream != nullptr && this->_stream->good());
}

bool File::eof() const {
  return (this->_stream == nullptr || this->_stream->eof());
}

std::iostream *File::getStream() {
  this->open();
  return this->_stream;
}

size_t File::read(char *buff, size_t size) {
  if (this->_stream == nullptr)
    return 0;
  this->_stream->read(buff, size);
  return this->_stream->gcount();
}
