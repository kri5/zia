#include <unistd.h>

#include "File/File_unix.h"
#include "ZException.hpp"
#include "Time/Time.h"

#include "MemoryManager.hpp"

File::File(std::string filename, const char* path) : _name(filename), _modifTime(NULL), _stream(NULL), _closed(false), _errorCode(Error::None)
{
	if (path)
	{
		_filePath = path;
		_filePath += "/";
		_filePath += filename;
	}
	else
		_filePath = filename;

	if (stat(_filePath.c_str(), &_stat) < 0)
	{
        _errorCode = Error::NoSuchFile;
	}
    else if (access(_filePath.c_str(), R_OK) < 0)
    {
        _errorCode = Error::PermissionDenied;
    }
}

File::~File()
{
	if (this->_modifTime)
		delete this->_modifTime;
    if (this->_stream)
    {
        this->close();
        delete this->_stream;
    }
}

IFile::Error::Code     File::getError() const
{
    return this->_errorCode;
}

std::string		File::getFileName() const
{
	return this->_name;
}

std::string     File::getFullFileName() const
{
    return this->_filePath;
}

unsigned int		File::getSize() const
{
	return this->_stat.st_size;
}

ITime*		File::getModifDate()
{
	if (this->_modifTime == NULL)
		this->_modifTime = new Time(this->_stat.st_mtime);
	return this->_modifTime;
}

bool        File::isDirectory() const
{
    return S_ISDIR(this->_stat.st_mode);
}

std::string		File::getExtension() const
{
	return this->_name.substr(this->_name.rfind("."));
}

void                File::open()
{
    if (this->_stream == NULL)
        this->_stream = new std::fstream(this->_filePath.c_str(), std::ios_base::in | std::ios_base::binary);
}

void                File::close()
{
    if (this->_stream && this->_closed == false)
    {
        this->_closed = true;
        this->_stream->close();
    }
}

std::streamsize      File::get(char* buff, size_t len)
{
    if (this->_stream == NULL)
        return 0;
    this->_stream->read(buff, len);
    return this->_stream->gcount();
}

bool                File::good() const
{
    return (this->_stream != NULL && this->_stream->good());
}

bool                File::eof() const
{
    return (this->_stream == NULL || this->_stream->eof());
}

std::iostream*       File::getStream()
{
    this->open();
    return this->_stream;
}

