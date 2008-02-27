#include "File_win32.h"
#include "ZException.hpp"
#include "Time.h"

#include "MemoryManager.hpp"

File::File(const std::string& filename, const char* path) : _filename(filename), _time(NULL)
{
	std::string			filePath;

	if (path)
	{
		filePath = path;
		filePath += "/";
		filePath += filename;
	}
	else
		filePath = filename;
	if (GetFileAttributesEx(filePath.c_str(), GetFileExInfoStandard, &_attr) == 0)
		throw ZException<File>(INFO, File::Error::CantGetAttributes, "Probably because file does not exist");
}

File::~File()
{
	delete this->_time;
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