#include <io.h>

#include "File/File_win32.h"
#include "ZException.hpp"
#include "Time/Time.h"
#include "FileSystem/FileSystem_win32.h"

#include "MemoryManager.hpp"

File::File(const std::string& filename, const char* path) : 
		_filename(filename), _time(NULL), _stream(NULL), _closed(false), _errorCode(Error::None)
{
//	std::cout << "path == " << path << " filename == " << filename << std::endl;
	if (path)
	{
		_filePath = path;
		_filePath += "/";
		_filePath += filename;
	}
	else
		_filePath = filename;
	if (GetFileAttributesEx(_filePath.c_str(), GetFileExInfoStandard, &_attr) == 0)
		_errorCode = Error::NoSuchFile;
	else
	{
		if ((_attr.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) //access can only be used on files.
			if (_access_s(_filePath.c_str(), FileSystem::Read) != 0) //Directory always has write/read access. Now is a good time to laugh at windows.
				_errorCode = Error::PermissionDenied;
	}
}

File::~File()
{
	delete this->_time;
	if (this->_stream)
	{
		this->close();
		delete this->_stream;
	}
}

std::string		File::getFileName() const
{
	return this->_filename;
}

unsigned int				File::getSize() const
{
	if (this->_attr.nFileSizeHigh != 0)
		Logger::getInstance() << Logger::Warning << "Asked for file greater than 4G !" << Logger::Flush;
	return this->_attr.nFileSizeLow;
}

ITime*		File::getModifDate()
{
	if (this->_time == NULL)
		this->_time = new Time(this->_attr.ftLastWriteTime);
	return this->_time;
}

bool		File::isDirectory() const
{
	return ((this->_attr.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);
}

std::string		File::getExtension() const
{
	return this->_filename.substr(this->_filename.rfind("."));
}

void			File::open()
{
	if (this->_stream == NULL)
		this->_stream = new std::fstream(this->_filePath.c_str(), std::ios_base::in | std::ios_base::binary);
}

void			File::close()
{
	if (this->_closed == false && this->_stream)
	{
		this->_stream->close();
		this->_closed = true;
	}
}

std::streamsize	File::get(char *buff, size_t len)
{
	this->_stream->read(buff, len);
	return this->_stream->gcount();
}

bool			File::good() const
{
	return this->_stream->good();
}

bool			File::eof() const
{
	return this->_stream->eof();
}

IFile::Error::Code	File::getError() const
{
	return this->_errorCode;
}

std::iostream*		File::getStream()
{
	this->open();
	return this->_stream;
}
