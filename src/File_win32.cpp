#include "File_win32.h"
#include "ZException.hpp"
#include "Time.h"

#include "MemoryManager.hpp"

File::File(const std::string& filename, const char* path) : _filename(filename), _time(NULL), _stream(NULL), _closed(false)
{
	if (path)
	{
		_filePath = path;
		_filePath += "/";
		_filePath += filename;
	}
	else
		_filePath = filename;
	if (GetFileAttributesEx(_filePath.c_str(), GetFileExInfoStandard, &_attr) == 0)
		throw ZException<File>(INFO, File::Error::CantGetAttributes, "Probably because file does not exist");
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
		Logger::getInstance() << Logger::Warning << "Asked for file greater than 4G !" << Zia::Newline;
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
	this->_stream = new std::ifstream(this->_filePath.c_str(), std::ios_base::binary);
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
